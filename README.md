# Writing print workflow apps and migrating WSDAs to UWP

This sample shows OEM print partners how to use the Print Workflow feature and migrate their existing Windows Store Device Apps (WSDAs) code to the Universal Windows Platform. 

## About
This is a sample primarily for OEM printer partners to show the combined implementation of both a Print Workflow app and a WSDA. Both are invoked by the `OnActivated()` method when a user opts to print something to a printer that has been associated with the app.

The Workflow background task uses the *XPS Object Model Windows Runtime* component to create a **PrintWorkflowXpsReceiver** implementation, called on a different thread to the app. The generator receives the document object model parts, and the app uses the *XPS Object Model* to add watermark text and a logo image before sending it to the printer.

The UI foreground task cannot change the XPS data; this must be done by the background task. Therefore, any parameters gathered by the UI are saved in local storage for use by the background task, which then makes the changes. The UI does not exit the `OnXpsDataAvailable()` function until the required data has been collected.

The main processing is done in the associated background task, and this sample demonstrates how to ensure that the application does not suspend after the UI is dismissed by the *Print Workflow Broker user service* if there are still background tasks to process. The main application code in the *App.xaml.cs* file will call a public static method in the background task class to ensure that all background tasks have exited during the Suspending handler, which will allot up to 30 minutes to complete processing.

The *PrinterContextNativeRuntimeComponent* Windows Runtime Component, initialized through the `PrintHelperClass` constructor, is of particular importance in the migration of existing Windows 8.1 WSDAs. It gives the Windows Runtime code access to the *Printer Extension COM model* used in 8.1. It can be initialized with either the traditional WSDA Printer Extension Context or with the new **IPrinterExtensionContextNative** interface provided in Print Workflow activation contexts.

The **IPrinterExtensionContextNative** interface has the same functionality as the 8.1 **IPrinterExtensionContext** interface except for the **IPrintSchemaTicket** print ticket, because that is available as a first-class WinRT object in the Print Workflow activation scenario.

## Test this sample

The following steps will explain how to test this sample app in its intended use cases.

### Create a local printing port for testing purposes

You may wish to create a "local port" printer before running the sample. Use a none-production system for this, because you will need to turn on test signing: 

`Bcdedit.exe -set TESTSIGNING ON`

See [The TESTSIGNING Boot Configuration Option](https://msdn.microsoft.com/library/windows/hardware/ff553484(v=vs.85).aspx) for more information.

To create a local port, do the following.
 
1. Press Win + R to bring up the Run dialog, and type `Control Printers`, or `%systemroot%\System32\control.exe /name Microsoft.Printers`
2. Select *Add a printer*
3. Select *The printer is not listed*
4. Select the last item: *Add a local printer…*
5. Create a new port. For example: "WorkflowPrinterPort". Select *Local* and *Name your Port*
1. Select *Microsoft Enterprise Cloud Print Class Driver*
2. Name the printer. For example: "WorkflowPrinter"
3. Configure the Workflow policy settings: open a PowerShell window and run the command for the new printer: `Set-Printer [-WorkflowPolicy {Uninitialized | Disabled | Enabled}]`. For example: `Set-Printer WorkflowPrinter -WorkflowPolicy Enabled`

You can optionally select the *Keep Printed Documents* option in the Control Panel Printer Properties. 

If you created a local printer port as detailed in the section above, then one way to use this is to open the Device Manager and look at the *Details* tab in the Properties pane for your printer and set the *Property* dropdown to *Hardware Ids*.

At this point you have a printer that writes XPS data to your Documents folder. The name of the output file will match the port name.

### Associate the sample app with your printer

To associate the application with the printer, modify the included sample Device Metadata file, *Example-WorkflowPrinterPort.devicemetadata-ms*, using the Device Metadata Wizard which you can obtain from the [Devices and Printers Metadata Authoring Wizard](https://msdn.microsoft.com/library/windows/hardware/dn629490(v=vs.85).aspx#user_s_guide_for_the_devices_and_printers_metadata_authoring_wizard) guide. Modify the file to match your printer and import the *Package.appxmanifest* file to get the app information.

Finally, build and run the sample application, which will install it on your system. You will then be able to test it with real-world scenarios - for example: running Notepad.exe, printing and selecting your printer, then interacting with this application when it is launched.

## Related samples

[Print settings and print notifications](https://code.msdn.microsoft.com/windowsapps/Device-app-for-printers-91f363a9)  

[Printing](https://microsoft.visualstudio.com/DefaultCollection/OS.Samples/_git/collections.universal?_a=contents&path=%2FSamples%2FPrinting)  

## System requirements
-------------------

**Client:** Windows 10 build 16288 or higher

Build the sample
----------------

1. It is recommended that, in PrintWorkflowXpsReceiver.cpp at the line below, you replace Symbols.ttf with a Unicode font such as ARIALUNI.TTF and also add that font to the Assets folder
      LPCWSTR fontFileName = L"Assets\\Symbols.ttf";
2. If you download the samples ZIP, be sure to unzip the entire archive, not just the folder with the sample you want to build. 
3. Start Microsoft Visual Studio 2017 and select **File** \> **Open** \> **Project/Solution**.
4. Starting in the folder where you unzipped the samples, go to the Samples subfolder, then the subfolder for this specific sample, then double-click the Visual Studio Solution (.sln) file.
5. Press Ctrl+Shift+B, or select **Build** \> **Build Solution**.

Please note that you need to install the Windows SDK version 16288 "Fall Creators Update" or later to build the sample.

Run the sample
--------------

To install and run the app, press F5 or use **Debug** \> **Start Debugging**. To run the app without debugging, press Ctrl+F5 or use **Debug** \> **Start Without Debugging**.

To debug the application when it is launched through Modern Print Dialog "More settings.." activation, or in the Print Workflow activation, from the Debug menu on Visual Studio 2017 or higher, choose **Other Debug Targets** > **Debug Installed App Package**, select **Local Machine** in the dropdown at the top, and choose the **WorkflowAndWSDACombinedSample**. Check the **Do not launch, but debug my code when it starts** and **Stop at first statement** checkboxes. 


