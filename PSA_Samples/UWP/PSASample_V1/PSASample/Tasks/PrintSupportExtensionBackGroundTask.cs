using Windows.ApplicationModel.Background;
using Windows.Data.Xml.Dom;
using Windows.Foundation.Metadata;
using Windows.Graphics.Printing.PrintSupport;
using System.Collections.Generic;

namespace Tasks
{
    public sealed class PrintSupportExtensionBackGroundTask : IBackgroundTask
    {
        // Can be completed in OnTaskInstanceCanceled.
        public BackgroundTaskDeferral TaskInstanceDeferral { get; set; }
        private string PdrIppNamespaceWithAttribute(string attribute)
        {
            return "schemas.microsoft.com/windows/2018/04/printing/printschemakeywords/Ipp/" + attribute;
        }
        private string PdrPskNamespaceWithAttribute(string attribute)
        {
            return "schemas.microsoft.com/windows/2003/08/printing/printschemakeywords/" + attribute;
        }

        private string PdrContosoNamespaceWithAttribute(string attribute)
        {
            return "schemas.contoso.com/keywords/" + attribute;
        }

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            TaskInstanceDeferral = taskInstance.GetDeferral();
            taskInstance.Canceled += OnTaskInstanceCanceled;

            if (taskInstance.TriggerDetails is PrintSupportExtensionTriggerDetails extensionDetails)
            {
                var session = extensionDetails.Session;
                session.PrintTicketValidationRequested += OnSessionPrintTicketValidationRequested;
                session.PrintDeviceCapabilitiesChanged += OnSessionPrintDeviceCapabilitiesChanged;

                // PSA V2 Event Handlers
                if (ApiInformation.IsEventPresent("Windows.Graphics.Printing.PrintSupport.PrintSupportExtensionSession", "PrinterSelected"))
                {
                    session.PrinterSelected += OnPrinterSelected;
                }

                // Make sure to register all the event handlers before PrintSupportExtensionSession.Start is called.
                session.Start();
            }
        }

        private void OnTaskInstanceCanceled(IBackgroundTaskInstance sender, BackgroundTaskCancellationReason reason)
        {
            TaskInstanceDeferral.Complete();
        }

        private void OnSessionPrintTicketValidationRequested(PrintSupportExtensionSession sender, PrintSupportPrintTicketValidationRequestedEventArgs args)
        {
            // Not fully implemented yet.
            args.SetPrintTicketValidationStatus(WorkflowPrintTicketValidationStatus.Resolved);
            args.GetDeferral().Complete();
        }

        private void OnSessionPrintDeviceCapabilitiesChanged(PrintSupportExtensionSession sender, PrintSupportPrintDeviceCapabilitiesChangedEventArgs args)
        {
            var pdc = args.GetCurrentPrintDeviceCapabilities();

            // IMPORTANT:
            // If you are adding a custom option, please make sure to add a custom namespace schema as well.
            // If you try to add a custom option that is not defined in the current 'printschemakeywords', then the print stack will silently fail your print job...

            // Add the custom namesapce uri to the XML document.
            pdc.DocumentElement.SetAttribute("xmlns:contoso", "http://schemas.contoso.com/keywords");
            // Add the custom media type.

            AddCustomType(ref pdc, "psk:JobStapleAllDocuments", "http://schemas.contoso.com/keywords", "contoso:Constaple");
            AddCustomType(ref pdc, "psk:PageOutputQuality", "http://schemas.contoso.com/keywords", "contoso:LowQuality");
            AddCustomType(ref pdc, "psk:JobPageOrder", "http://schemas.contoso.com/keywords", "contoso:OddsThenEvens");
            AddCustomType(ref pdc, "psk:JobInputBin", "http://schemas.contoso.com/keywords", "contoso:InputBin");
            AddCustomType(ref pdc, "psk:JobOutputBin", "http://schemas.contoso.com/keywords", "contoso:OutputBin");
            AddCustomType(ref pdc, "psk:JobBindAllDocuments", "http://schemas.contoso.com/keywords", "contoso:ContosoOffset");
            AddCustomType(ref pdc, "psk:PageMediaType", "http://schemas.contoso.com/keywords", "contoso:ContosoMediaType");

            AddCustomMediaResolution(ref pdc, "psk:PageResolution", "http://schemas.contoso.com/keywords", "contoso:DPI900x900");
            AddCustomMediaSize(ref pdc, "psk:PageMediaSize", "http://schemas.contoso.com/keywords", "contoso:ContosoMediaSize", "98420", "98420", "4064,4064,68072,118872");

            // Things that don't work
            AddCustomType(ref pdc, "psk:DocumentCollate", "http://schemas.contoso.com/keywords", "contoso:ContosoDeCollate");
            AddCustomType(ref pdc, "psk:PageOrientation", "http://schemas.contoso.com/keywords", "contoso:Diagonal");
            AddCustomType(ref pdc, "psk:PageOutputColor", "http://schemas.contoso.com/keywords", "contoso:Custlor");
            AddCustomType(ref pdc, "psk:PageScaling", "http://schemas.contoso.com/keywords", "contoso:Stretch");
            AddCustomType(ref pdc, "psk:JobDuplexAllDocumentsContiguously", "http://schemas.contoso.com/keywords", "contoso:ContosoDuplex");


            AddCustomMediaSize(ref pdc, "psk:PageMediaSize", "http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords", "psk:NorthAmerica5x7", "127000", "177800", "4064, 4064, 118872, 169672");

            args.UpdatePrintDeviceCapabilities(pdc);

            if (ApiInformation.IsMethodPresent("Windows.Graphics.Printing.PrintSupport.PrintSupportPrintDeviceCapabilitiesChangedEventArgs", "GetCurrentPrintDeviceResources"))
            {
                // PdrLocale is immutable & only describes the language of the Print Device.
                var pdrLocale = args.ResourceLanguage;

                // PDR location is typically defined in C:\Windows\System32\spool\V4Dirs, but can exist in other locations.
                var pdr = args.GetCurrentPrintDeviceResources();

                // PDR is not always guarenteed & this function should not fail if PDR is an empty Xml Document
                if(!string.IsNullOrEmpty(pdrLocale) && pdr.ChildNodes.Count > 0)
                {
                    Windows.ApplicationModel.Resources.Core.ResourceContext defaultContextForCurrentView = Windows.ApplicationModel.Resources.Core.ResourceContext.GetForViewIndependentUse();
                    //Instead of getting language based off of user's/system's preferences, this changes preference to PDR's langauge.
                    defaultContextForCurrentView.QualifierValues["language"] = pdrLocale;

                    Windows.ApplicationModel.Resources.Core.ResourceMap resourceMap = Windows.ApplicationModel.Resources.Core.ResourceManager.Current.MainResourceMap.GetSubtree("ContosoMediaTypes");
                    foreach (var mediaTypeName in resourceMap.Keys)
                    {
                        if(!CustomTypeExistsInPDR(ref pdr, PdrContosoNamespaceWithAttribute(mediaTypeName)))
                        {
                            var localizedName = resourceMap.GetValue(mediaTypeName, defaultContextForCurrentView).ValueAsString;
                            AddCustomPdrString(ref pdr, PdrContosoNamespaceWithAttribute(mediaTypeName), localizedName);
                        }
                    }

                    resourceMap = Windows.ApplicationModel.Resources.Core.ResourceManager.Current.MainResourceMap.GetSubtree("IppMediaTypes");
                    foreach (var mediaTypeName in resourceMap.Keys)
                    {
                        if (!CustomTypeExistsInPDR(ref pdr, PdrIppNamespaceWithAttribute(mediaTypeName)))
                        {
                            var localizedName = resourceMap.GetValue(mediaTypeName, defaultContextForCurrentView).ValueAsString;
                            AddCustomPdrString(ref pdr, PdrIppNamespaceWithAttribute(mediaTypeName), localizedName);
                        }

                        if (!CustomTypeExistsInPDR(ref pdr, PdrPskNamespaceWithAttribute(mediaTypeName)))
                        {
                            var localizedName = resourceMap.GetValue(mediaTypeName, defaultContextForCurrentView).ValueAsString;
                            AddCustomPdrString(ref pdr, PdrPskNamespaceWithAttribute(mediaTypeName), localizedName);
                        }
                    }
                    // check if it exists in pdr
                    // add to pdr 
                    args.UpdatePrintDeviceResources(pdr);
                }
            }

            args.GetDeferral().Complete();
        }

        private void AddCustomType(ref XmlDocument pdc, string type, string namespaceUri, string mediaType)
        {
            // See linked below for the XML.Dom.Xmldocument class documentation.
            // https://docs.microsoft.com/en-us/uwp/api/windows.data.xml.dom.xmldocument
            //
            // NOTE: This example does not show error handling for simplicty. You should ensure that the properties being modified exists in the PDC, before trying to set its attributes.

            string searchString = "//" + type + "//*[@psf2:default='true']"; //*[@psf2:default='true']",
            //psk:JobDuplexAllDocumentsContiguously

            // Select the 'media-type-supported' XML section.
            var defaultPageMediaTypeNode = pdc.SelectSingleNodeNS(
                searchString,
                "xmlns:psk=\"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords\" xmlns:psf2=\"http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2\"");

            // Get the owner document so that we can add new elements to the currently selected XML section.
            var document = defaultPageMediaTypeNode.OwnerDocument;
            // Create the new XML element for our custom media type.
            var newNode = document.CreateElementNS(namespaceUri, mediaType);
            newNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:psftype", "Option");
            newNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:default", "false");

            var parent = defaultPageMediaTypeNode.ParentNode;
            parent.AppendChild(newNode);
        }

        private void AddCustomMediaSize(ref XmlDocument pdc, string type, string namespaceUri, string mediaType, string height, string width, string overall)
        {
            // "psk:DocumentCollate", "http://schemas.contoso.com/keywords", "contoso:ContosoDeCollate"

            // See linked below for the XML.Dom.Xmldocument class documentation.
            // https://docs.microsoft.com/en-us/uwp/api/windows.data.xml.dom.xmldocument
            //
            // NOTE: This example does not show error handling for simplicty. You should ensure that the properties being modified exists in the PDC, before trying to set its attributes.

            string searchString = "//" + type + "//*[@psf2:default='true']"; //*[@psf2:default='true']",

            // Select the 'media-type-supported' XML section.
            var defaultPageMediaTypeNode = pdc.SelectSingleNodeNS(
                searchString,
                "xmlns:psk=\"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords\" xmlns:psf2=\"http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2\"");

            

            // Get the owner document so that we can add new elements to the currently selected XML section.
            var document = defaultPageMediaTypeNode.OwnerDocument;
            // Create the new XML element for our custom media type.
            var newNode = document.CreateElementNS(namespaceUri, mediaType);
            newNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:psftype", "Option");
            newNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:default", "false");

            var mediaWidthNode = document.CreateElementNS("http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords", "psk:MediaSizeWidth");
            mediaWidthNode.SetAttributeNS("http://www.w3.org/2001/XMLSchema-instance", "xsi:type", "xsd:integer");
            mediaWidthNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:psftype", "ScoredProperty");
            mediaWidthNode.InnerText = height;

            var mediaHeighthNode = document.CreateElementNS("http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords", "psk:MediaSizeHeight");
            mediaHeighthNode.SetAttributeNS("http://www.w3.org/2001/XMLSchema-instance", "xsi:type", "xsd:integer");
            mediaHeighthNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:psftype", "ScoredProperty");
            mediaHeighthNode.InnerText = width;

            var portraitNode = document.CreateElementNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemakeywordsv12", "psk12:PortraitImageableSize");
            portraitNode.SetAttributeNS("http://www.w3.org/2001/XMLSchema-instance", "xsi:type", "psf2:ImageableAreaType");
            portraitNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:psftype", "Property");
            portraitNode.InnerText = overall;

            newNode.AppendChild(mediaWidthNode);
            newNode.AppendChild(mediaHeighthNode);
            newNode.AppendChild(portraitNode);

            var parent = defaultPageMediaTypeNode.ParentNode;
            parent.AppendChild(newNode);
        }

        private void AddCustomMediaResolution(ref XmlDocument pdc, string type, string namespaceUri, string mediaType)
        {
            // "psk:DocumentCollate", "http://schemas.contoso.com/keywords", "contoso:ContosoDeCollate"

            // See linked below for the XML.Dom.Xmldocument class documentation.
            // https://docs.microsoft.com/en-us/uwp/api/windows.data.xml.dom.xmldocument
            //
            // NOTE: This example does not show error handling for simplicty. You should ensure that the properties being modified exists in the PDC, before trying to set its attributes.

            string searchString = "//" + type + "//*[@psf2:default='true']"; //*[@psf2:default='true']",

            // Select the 'media-type-supported' XML section.
            var defaultPageMediaTypeNode = pdc.SelectSingleNodeNS(
                searchString,
                "xmlns:psk=\"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords\" xmlns:psf2=\"http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2\"");

            // Get the owner document so that we can add new elements to the currently selected XML section.
            var document = defaultPageMediaTypeNode.OwnerDocument;
            // Create the new XML element for our custom media type.
            var newNode = document.CreateElementNS(namespaceUri, mediaType);
            newNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:psftype", "Option");
            newNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:default", "false");

            var mediaWidthNode = document.CreateElementNS("http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords", "psk:ResolutionX");
            mediaWidthNode.SetAttributeNS("http://www.w3.org/2001/XMLSchema-instance", "xsi:type", "xsd:integer");
            mediaWidthNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:psftype", "ScoredProperty");
            mediaWidthNode.InnerText = "900";

            var mediaHeighthNode = document.CreateElementNS("http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords", "psk:ResolutionY");
            mediaHeighthNode.SetAttributeNS("http://www.w3.org/2001/XMLSchema-instance", "xsi:type", "xsd:integer");
            mediaHeighthNode.SetAttributeNS("http://schemas.microsoft.com/windows/2013/12/printing/printschemaframework2", "psf2:psftype", "ScoredProperty");
            mediaHeighthNode.InnerText = "900";


            newNode.AppendChild(mediaWidthNode);
            newNode.AppendChild(mediaHeighthNode);

            var parent = defaultPageMediaTypeNode.ParentNode;
            parent.AppendChild(newNode);
        }

        private bool CustomTypeExistsInPDR(ref XmlDocument pdr, string attribute)
        {
            string attributeSearch = "//data[@name=\"" + attribute + "\"]";
            var isExists = pdr.SelectSingleNode(attributeSearch);
            return isExists != null;
        }

        private void AddCustomPdrString(ref XmlDocument pdr, string attributeName, string localizedName)
        {
            // Spacer & Tab Node are functionally unnecessary, but it allow for the PDR document to remain consistent format when saved as a xml document.
            IXmlNode _rootNode = pdr.SelectSingleNode("root");
            IXmlNode tabNode = pdr.CreateTextNode("\t");
            IXmlNode spacerNode = pdr.CreateTextNode("\n");

            var childNode = _rootNode.OwnerDocument.CreateElement("data");
            childNode.SetAttribute("name", attributeName);
            var granchildNode = _rootNode.OwnerDocument.CreateElement("value");
            granchildNode.InnerText = localizedName;
            childNode.AppendChild(granchildNode);

            _rootNode.AppendChild(tabNode);
            _rootNode.AppendChild(childNode);
            _rootNode.AppendChild(spacerNode);
        }

        // Event handler called when the PSA Associated printer is selected in Print Dialog
        private void OnPrinterSelected(PrintSupportExtensionSession session, PrintSupportPrinterSelectedEventArgs args)
        {
            using (args.GetDeferral())
            {
                // Show adaptive card in the Print Dialog (generated based on Printer and Printing App) 
                args.SetAdaptiveCard(GetCustomAdaptiveCard(session.Printer, args.SourceAppInfo));
                // Request to show Features and Parameters in the Print Dialog if not shown already
                const string xmlNamespace = "\"http://schemas.microsoft.com/windows/2003/08/printing/printschemakeywords\"";
                var additionalFeatures = new List<PrintSupportPrintTicketElement> { new PrintSupportPrintTicketElement { LocalName = "PageMediaType", NamespaceUri = xmlNamespace }, };
                var additionalParameters = new List<PrintSupportPrintTicketElement> { new PrintSupportPrintTicketElement { LocalName = "JobCopiesAllDocuments", NamespaceUri = xmlNamespace } };
                if ((additionalFeatures.Count + additionalParameters.Count) <= args.AllowedAdditionalFeaturesAndParametersCount)
                {
                    args.SetAdditionalFeatures(additionalFeatures);
                    args.SetAdditionalParameters(additionalParameters);
                }
                else
                {
                    // Cannot show that many additional features and parameters , consider reducing the number
                    // of additional features and parameters by selecting only the most important ones
                }
            }
        }

        // Create simple AdaptiveCard to show in MPD
        public Windows.UI.Shell.IAdaptiveCard GetCustomAdaptiveCard(Windows.Devices.Printers.IppPrintDevice ippPrinter, Windows.ApplicationModel.AppInfo appInfo)
        {
            //original line below does not work because appInfo.DisplayInfo.DisplayName is not implemented by windows yet
            //return Windows.UI.Shell.AdaptiveCardBuilder.CreateAdaptiveCardFromJson($@"{{""body"": [ {{""type"": ""TextBlock"", ""text"": ""Hello {appInfo.DisplayInfo.DisplayName} from {ippPrinter.PrinterName}!"" }}],""$schema"": ""http://adaptivecards.io/schemas/adaptive-card.json"", ""type"": ""AdaptiveCard"", ""version"": ""1.0""}}");
            return Windows.UI.Shell.AdaptiveCardBuilder.CreateAdaptiveCardFromJson($@"{{""body"": [ {{""type"": ""TextBlock"", ""text"": ""Hello PSA Support App from {ippPrinter.PrinterName}!"" }}],""$schema"": ""http://adaptivecards.io/schemas/adaptive-card.json"", ""type"": ""AdaptiveCard"", ""version"": ""1.0""}}");

            //Alternative way:
            //string DisplayName = "PSA Support App";
            //string PrinterName = ippPrinter.PrinterName;
            //string strJson = "{\"body\": [ { \"type\": \"TextBlock\", \"text\": \"Hello " + DisplayName + " from " + PrinterName + " !\" } ], \"$schema\": \"http://adaptivecards.io/schemas/adaptive-card.json\", \"type\": \"AdaptiveCard\", \"version\": \"1.0\"}";
            //return Windows.UI.Shell.AdaptiveCardBuilder.CreateAdaptiveCardFromJson(strJson);
        }
    }
}
