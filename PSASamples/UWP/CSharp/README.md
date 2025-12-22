# UWP C# Print Support App

- [UWP C# Print Support App](#c-print-support-app)
  - [Release History](#release-history)
  - [Remarks](#remarks)
  - [Resources](#resources)

## Release History

December 2025 - Latest updates for modern Windows development:

- Updated for Windows 11 Build 26100 and Windows App SDK compatibility
- Migrated to .NET 8.0 for C# projects
- Enhanced watermarking and XPS document manipulation capabilities
- Improved print ticket handling with latest Print Support App APIs
- Updated code examples for current platform best practices
- Modernized UI framework for Windows 11 design language

## Remarks

**Building the Solution:**
1. Open `PrintSupportApp.sln` (`"print-oem-samples\PSASamples\UWP\CSharp\PrintSupportApp\PrintSupportApp.sln`) in Visual Studio 2022 or later
2. Restore NuGet packages
    - In the Solution Explorer, right click on the solution header at the top
    - Select "Restore NuGet packages"
3. Clean, then build the solution
    - In the Solution Explorer, right click on the solution header at the top
    - Select "Clean Solution"
    - Wait for the Output panel to show the cleanup was completed successfully
    - Right click the solution header again and select "Build Solution"
    - Wait for the Output panel to show the build is completed
4. Also make sure that PrintSupportApp is selected as the startup project
    - In the Solution Explorer, right click on the PrintSupportApp project header
    - Select "Set as Starup Project"
5. Deploy and run the application
    - Make sure the settings are configured as "Debug" and "x64" at the top of the Visual Studio menu
    - Click the green triangle button to run the application on your local machine
6. PSA Popup Appears
    - A popup titled "Print Support App" should appear, displaying the package family name of the printer
7. Close the popup window and stop the application by clicking the red square in Visual Studio

## Resources

- [Create a multi-instance Universal Windows App](https://docs.microsoft.com/en-us/windows/uwp/launch-resume/multi-instance-uwp)
- [Using an Extension INF File](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/using-an-extension-inf-file)
- [Windows.Data.Xml.Dom](https://docs.microsoft.com/en-us/uwp/api/windows.data.xml.dom)
  - [XPath Syntax](https://www.w3schools.com/xml/xpath_syntax.asp)
- [Windows.Graphics.Printing.PrintTicket](https://docs.microsoft.com/en-us/uwp/api/windows.graphics.printing.printticket)
