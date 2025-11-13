# C# Print Support App

- [C# Print Support App](#c-print-support-app)
  - [Release History](#release-history)
  - [Known Issues](#known-issues)
    - [P1](#p1)
    - [P2](#p2)
  - [Remarks](#remarks)
  - [Resources](#resources)

## Release History

<details>
<summary>2021-02-22 (v7)</summary>

Many updates, including:

 - Demonstration of IPrintWorkflowXpsReceiver to load and render pages as they become available, rather than waiting for the entire XpsDocument to load before showing the initial preview.
 - Restructuring of overall architecture to better seperate responsiblities, especially in watermark preview UI elements and in the watermarking process.
 - Watermarking process standardized between preview and print background tasks through XpsWatermarker.
 - Rendering preview is done in the background to avoid blocking the UI thread.
 - Preview is rendered immediately as watermark text is typed, and in response to other UI events, without need for an explicit "preview" button.
 - Improved UI for choosing relative watermark position on the page.
 - Add a cancel button to abort the workflow from the print dialog.
 - Demonstrate getting and setting custom IPP attributes.

</details>

<details>
<summary>2020-12-17 (v6)</summary>

- Updated PrintSupportWorkflowBackgroundTask.cs - Update PSA sample to new API CreateJobOnPrinter() and remove GetJobAttributes().

</details>

<details>
<summary>2020-11-06 (v5)</summary>

- Updated PrintTicketManipulationExample.xaml.cs - Print ticket options are selected from the Print Ticket object, instead of the Print Capabilities object.
- Added UserLaunchMainPage.xaml.cs - Added example to obtain the IP address of IPP Printers connected to the device.

</details>

<details>
<summary>2020-08-13 (v4)</summary>

- Added OnSessionJobNotification example. See `JobNotificationExample.xaml.cs`
  - Small refactoring was done on `WatermarkManipulationExample.xaml.cs` and `JobActivatedMainPage.xaml.cs`
- Added AddCustomMediaType example and removed SetAsDefaultPageMediaSize example. See `PrintSupportExtensionBackGroundTask.cs`
  - Currently the default values on the print ticket cannot be changed. The sample now adds a new selectable option for `PageMediaType`.
- Added GetJobAttributes example. See `PrintSupportWorkflowBackgroundTask.cs`

</details>

<details>
<summary>2020-07-14 (v3)</summary>

- Package.appxmanifest
  - Added `privateNetworkClientServer` to Capabilities section. This is to allow IPP requests to be made by the app.
  - Added `uap10:SupportsMultipleInstances="true"` to Application section in order to support multi-instance app.
- XpsDocument
  - Replaced `args.SourceContent.GetInputStream()` with `args.SourceContent.GetContentFileAsync` in OnPdlModificationRequested. `GetContentFileAsync` is the proper way to get the document data.
  - `CLSID_XPSRAS_FACTORY` is replaced with `CLSID_XPSRASTERIZER_FACTORY` in XpsDocument::ConvertXPStoBMP.
- Watermark
  - UI closes after clicking `Print`.

</details>

<details>
<summary>2020-06-30 (v2)</summary>

- Updated watermark example.
  - Added document preview.
  - Added ability to add a custom watermark image to the XPS document.
- Updated `PrintSupportWorkflowBackgroundTask.OnPdlModificationRequested` to convert the XPS document to a printer supported PDL, instead of only XPS->PDF.
- Added `PrintSupportWorkflowBackgroundTask.GetDocumentFormatDefault` to get the `document-format-default` IPP attribute from the printer.
- Added `PrintSupportExtensionBackGroundTask.OnSessionPrintDeviceCapabilitiesChanged` to update the default page media size.
  - **Issue**: We are investigating an issue where the default print ticket does not reflect the udpated PDC values.
- New contract IDs are available. Please update `Package.appxmanifest`. See usage in files listed below.
  - `Package.appxmanifest`
    1. Add the new namespace `xmlns:uap12="http://schemas.microsoft.com/appx/manifest/uap/windows10/12"`.
    2. Add `uap12` to the space separated list of `IgnorableNamespaces`.
    3. Update `TargetDeviceFamily` as shown below:
        > TargetDeviceFamily Name="Windows.Universal" MinVersion="10.0.0.0" MaxVersionTested="10.0.0.0"
    4. Update the contract IDs.
    - Remove
      - `windows.printTaskSettings`
      - `windows.printWorkflowForegroundTask`
      - `windows.printWorkflowBackgroundTask`
    - Add
      - `windows.printSupportExtension`
      - `windows.printSupportWorkflow`
      - `windows.printSupportSettingsUI`
      - `windows.printSupportJobUI`

  - `windows.printSupportExtension`
    - `PrintSupportExtensionBackGroundTask.cs`.
  - `windows.printSupportWorkflow`
    - `PrintSupportWorkflowBackgroundTask.cs`.
  - `windows.printSupportSettingsUI`
    - `JobActivatedMainPage.xaml.cs`
    - `WatermarkManipulationExample.xaml.cs`
  - `windows.printSupportJobUI`
    - `SettingsActivatedMainPage.xaml.cs`
    - `PrintTicketManipulationExample.xaml.cs`

</details>

<details>
<summary>2020-06-02 (v1)</summary>

- Initial C# Sample PSA prepared for print partners.

</details>

## Known Issues

### P1

### P2

- ~~`LaunchAndCompleteUIAsync` launches the PSA behind other windows.~~ [Fixed in 2020-07-14 build]
  - This is fixed with the exception of when printing from the Maps app.

<details>
<summary>Fixed</summary>

- ~~Printdialog crashes when PSA is launched and closed without modifying the print ticket settings.~~ [Fixed in 2020-06-30 build]
- ~~PSA background tasks are all in-proc. This means that PSA can not be run as a multi-instance app.~~ [Fixed in 2020-07-14 build]
- ~~Currently, the PSA background tasks all have a 25 second time out period. If the background tasks runs for more than 25s, it will be terminated by the system. The timeout will be removed in a future release.~~ [Fixed]
  - ~~Additionally, because PSA is running as a single instance app, any UI will also be terminated, when the background task is terminated.~~ [Fixed]
- ~~The default print ticket received in the printing app does not have the updated PDC, after the PDC was updated by PSA.~~ [Fixed in 2020-07-14 build]
  - ~~Currently PDC edits does not support changing of default media size.~~ [By Design]
  - ~~Update PDC example is not updated yet.~~ [Fixed in V4]
- ~~Upon calling `PrintWorkflowPdlDataAvailableEventArgs.PrintWorkflowConfiguration.AbortPrintFlow`, the print queue will show the status of the print job as "Deleting", and the job will stay in the queue in a broken state.~~ [Fixed]
- ~~PSA is NOT closed after setting the watermark and selecting `Print`.~~ [Fixed in 2020-07-14 build]
  - ~~Once the background tasks can run out-of-proc & PSA can run as a multi-instance app, this issue will be fixed.~~

</details>

## Remarks

<details>

- ~~The current manifest contract (windows.printWorkflowBackgroundTask) used in the sample app is not the final/correct contract for a Print Support App. We are temporarly using this contract until the new PSA contracts are added to the SDK.~~ [Fixed in 2020-06-30 build]

- For builds 2020-06-30 and above, please make sure the following contract IDs are removed from the `Package.appxmanifest` file.
  - `windows.printTaskSettings`
  - `windows.printWorkflowBackgroundTask`
  - `windows.printWorkflowForegroundTask`

</details>

## Resources

- [Create a multi-instance Universal Windows App](https://docs.microsoft.com/en-us/windows/uwp/launch-resume/multi-instance-uwp)
- [Using an Extension INF File](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/using-an-extension-inf-file)
- [Windows.Data.Xml.Dom](https://docs.microsoft.com/en-us/uwp/api/windows.data.xml.dom)
  - [XPath Syntax](https://www.w3schools.com/xml/xpath_syntax.asp)
- [Windows.Graphics.Printing.PrintTicket](https://docs.microsoft.com/en-us/uwp/api/windows.graphics.printing.printticket)
