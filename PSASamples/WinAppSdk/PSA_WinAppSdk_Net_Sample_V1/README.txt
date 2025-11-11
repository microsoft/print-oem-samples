README for C#/.Net Print Support Application Sample for WinAppSdk

Version 1:
  - Basic C#/.Net WinAppSdk PSA application that showcases setting up registration for PrintSupportExtension and PrintSupportWorkflow operations.
  - Minimal UX
  - Can be installed and used for printing (callbacks are hit).
  - Preferences can be launched however no information is displayed. 

Requirements:
  - Must be using .Net6
    - Moving to .Net8 is being investigated.
  - Needs to use CsWinRT to support hosting the projections for callback from the OS for PSA operations.
    - Currently using CsWinRT version 2.0.8. Next version will use 2.2.0 (or later).
  - Should be using the latest WinAppSdk and build tools

Adjustments to CSProj files:
  - Tasks project:
    - Needs to have CsWinRT Nuget package.
    - Needs to have the following PropertyGroup:
        <PropertyGroup>
          <CsWinRTComponent>true</CsWinRTComponent>
          <CsWinRTWindowsMetadata>10.0.26100.0</CsWinRTWindowsMetadata>
        </PropertyGroup>
      - This is required to bring in WinRT.Host.dll and supporting files. WinRT.Host.dll hosts the projection. 
  - PrintSupportApplication project:
    - Needs to have a dependency/ProjectReference on the Tasks project.
    - Needs to have the following property set:
      <WindowsAppSDKSelfContained>true</WindowsAppSDKSelfContained>
    - Needs to have the following Nuget packages:
      - CsWinRT
      - SDK build tools
      - WindowsAppSDK
  - Appx Manifest:
    - Needs to have the following extension information set for the Application section:
      <Extensions>
        <psa:Extension Category="windows.printSupportExtension" EntryPoint="Tasks.PrintSupportExtensionBackgroundTask"/>
        <psa:Extension Category="windows.printSupportWorkflow" EntryPoint="Tasks.PrintSupportWorkflowBackgroundTask"/>
        <psa:Extension Category="windows.printSupportSettingsUI" EntryPoint="PrintSupportApplication.App"/>
        <psa:Extension Category="windows.printSupportJobUI" EntryPoint="PrintSupportApplication.App"/>
      </Extensions>
      - Note: Names can be adjusted to fit the app.
    - Needs to have the following extension information (outside of application section) set for task activation:
        <Extensions>
          <Extension Category="windows.activatableClass.inProcessServer">
            <InProcessServer>
              <Path>WinRT.Host.dll</Path>
              <ActivatableClass ActivatableClassId="Tasks.PrintSupportExtensionBackgroundTask" ThreadingModel="both" />
              <ActivatableClass ActivatableClassId="Tasks.PrintSupportWorkflowBackgroundTask" ThreadingModel="both" />
            </InProcessServer>
          </Extension>
        </Extensions>
        - Note: Names can be adjusted to fit the app.
    - Needs other standard adjustments to support a PSA (i.e. PrintSupport namespaces, etc).
  - RSP files:
    - Support CsWinRT and have paths specific to the machine. will likely need to be adjusted/recreated for the machine creating the application. 

Known Issues:
  - .Net8 cannot be used at this time.
  - Unable to retrieved the instance arguments as PrintWorkflowJobActivatedEventArgs when handling an activation event for PrintSupportJobUI

Additional Resources:
  - Windows App SDK: https://learn.microsoft.com/en-us/windows/apps/windows-app-sdk/
  - CsWinRT documentation: https://github.com/microsoft/CsWinRT/tree/master/docs
