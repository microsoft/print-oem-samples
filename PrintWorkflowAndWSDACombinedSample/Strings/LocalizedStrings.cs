// Copyright(c) Microsoft Corporation.All rights reserved.
//
//   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//   PARTICULAR PURPOSE.
//
//   Sample code to show how localized strings would be handled in this sample
//
namespace WorkflowAndWSDACombinedSample
{
    [Windows.UI.Xaml.Data.Bindable]
    public class LocalizedStrings
    {
        #region Public properties
        public static string OKButtonLabel
        {
            get
            {
                return okButtonLabel;
            }
        }

        public static string InkLevelLabel
        {
            get
            {
                return inkLevelLabel;
            }
        }

        public static string MainTitleLabel
        {
            get
            {
                return mainTitleLabel;
            }
        }
        #endregion

        #region Internal member functions
        /// <summary>
        /// Return the named resource string
        /// </summary>
        /// <param name="resourceName"></param>
        /// <returns>A string containing the requested resource string value</returns>
        internal static string GetResourceString(string resourceName)
        {
            string theResourceString = "##Failed to get resource string##";
            var resourceLoader = Windows.ApplicationModel.Resources.ResourceLoader.GetForCurrentView();
            theResourceString = resourceLoader.GetString(resourceName);
            return theResourceString;
        }
        #endregion 

        #region Private member variables
        /// <summary>
        /// Localizable resource string properties
        /// </summary>

        private static string okButtonLabel = GetResourceString("OKButtonLabel");
        private static string mainTitleLabel = GetResourceString("MainTitleLabel");
        private static string inkLevelLabel = GetResourceString("InkLevelLabel");
        #endregion
    }
}
