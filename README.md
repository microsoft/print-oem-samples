# Print Support App (PSA) Samples

This repository contains Print Support App (PSA) samples demonstrating printer customization capabilities for OEM print partners across different development platforms.

## About

These samples show OEM printer partners how to implement Print Support Applications using modern Windows platforms. The samples demonstrate advanced printer customization, print workflow processing, and XPS document manipulation.

## Samples Overview

### UWP Print Support App Sample

A Universal Windows Platform (UWP) C# Print Support Application demonstrating:

- Print workflow customization and background tasks
- Print ticket manipulations
- XPS document watermarking using the XPS Object Model
- Job notification handling
- Virtual printer implementation
- IPP printer URL configuration
- Settings and job activation scenarios

The UWP sample includes three main components:
- **PrintSupportApp**: The main UWP application with UI for printer settings and workflow customization
- **Tasks**: Background tasks for print support extension, workflow processing, and virtual printer handling
- **XpsUtil**: Native Windows Runtime Component for XPS document manipulation

### PSA App SDK Print Support App Samples

Print Support App samples for modern Windows app development in C++ demonstrating:

- Print Support Extension background tasks
- Workflow background task processing
- Virtual printer implementation
- XPS document manipulation
- Job and user activation scenarios
- Modal window handling for printer settings
- Watermark preview and customization

These samples leverage the Windows App SDK for enhanced functionality and deployment flexibility.

## System Requirements

- **Windows 11 Build 26100 or higher** for UWP samples
- **Windows 11 Build 26100 or higher** for PSA App SDK samples
- Visual Studio 2022 (version 17.8 or higher)
- Windows SDK version 26100 or later
- .NET 8.0 or higher for C# projects

## Building the Samples

1. If you download the samples ZIP, unzip the entire archive, not just individual folders.
2. Open the solution file (.sln) in the sample folder with Visual Studio.
3. Press Ctrl+Shift+B or select **Build** > **Build Solution**.

## Running the Samples

1. To install and run the app, press F5 or use **Debug** > **Start Debugging**.
2. To run without debugging, press Ctrl+F5 or use **Debug** > **Start Without Debugging**.

For Print Workflow activation debugging in Visual Studio 2022 or higher:
- From the Debug menu, choose **Other Debug Targets** > **Debug Installed App Package**
- Select **Local Machine** and choose your Print Support App
- Check **Do not launch, but debug my code when it starts** and **Stop at first statement**
