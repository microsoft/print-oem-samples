# Print Support Application - Test Plan

## Pre-Build Requirements

### Build Environment
- Visual Studio 2022 or MSBuild
- .NET 10 SDK (10.0.102 or later)
- Windows SDK 10.0.26100.0 or later
- Windows 11 (Build 26100 or later for full PSA support)

### Build Instructions
```powershell
# Option 1: Visual Studio
1. Open PrintSupportApplication.sln in Visual Studio 2022
2. Set platform to x64
3. Build Solution (Ctrl+Shift+B)

# Option 2: MSBuild Command Line
cd C:\work\print-oem-samples-master\PSASamples\WinAppSdk\CS
msbuild /p:Platform=x64 /p:Configuration=Debug

# Option 3: Build only C# projects (without XpsUtil watermarking)
cd C:\work\print-oem-samples-master\PSASamples\WinAppSdk\CS\PrintSupportApplication
dotnet build
```

### Installation
```powershell
# Register the app package
Add-AppxPackage -Register Package.appxmanifest
```

## Test Scenarios

### 1. Application Launch Tests

#### 1.1 Normal Launch
**Objective**: Verify app launches normally and displays UserLaunchMainPage

**Steps**:
1. Launch PrintSupportApplication from Start Menu
2. Verify UserLaunchMainPage is displayed
3. Verify NavigationView shows 3 menu items:
   - App Info
   - Get IPP Printer URL
   - Virtual Printers

**Expected Result**:
- App launches without errors
- UserLaunchMainPage displays with navigation menu
- All navigation items are clickable

#### 1.2 Navigate to App Info
**Steps**:
1. Launch app normally
2. Click "App Info" in navigation menu
3. Verify Package Family Name is displayed

**Expected Result**:
- AppInfo page displays
- Package Family Name shows correctly

#### 1.3 Navigate to IPP Printer URL
**Steps**:
1. Launch app normally
2. Click "Get IPP Printer URL" in navigation menu
3. Verify IPP printer query UI is displayed

**Expected Result**:
- GetIppPrinterUrl page displays
- Manufacturer text box and Start Query button are visible

#### 1.4 Navigate to Virtual Printers
**Steps**:
1. Launch app normally
2. Click "Virtual Printers" in navigation menu
3. Verify virtual printers page is displayed

**Expected Result**:
- VirtualPrintersPage displays
- Add Virtual Printer and Refresh buttons are visible
- Virtual printers list displays (may be empty initially)

### 2. Settings Activation Tests

#### 2.1 Launch Printer Preferences
**Objective**: Verify SettingsActivatedMainPage displays when opening printer preferences

**Steps**:
1. Open Control Panel > Devices and Printers
2. Find a printer associated with this PSA
3. Right-click > Printing Preferences
4. Verify SettingsActivatedMainPage is displayed

**Expected Result**:
- App activates and shows SettingsActivatedMainPage
- NavigationView shows "Print Ticket Manipulation Example"
- Print ticket options are displayed

#### 2.2 Print Ticket Manipulation
**Steps**:
1. Launch printer preferences (as above)
2. Verify PrintTicketManipulationExample page displays
3. Verify print ticket features display as ComboBoxes:
   - DocumentBindingFeature
   - DocumentDuplexFeature
   - PageMediaSizeFeature
   - PageOrientationFeature
   - PageOutputColorFeature
   - etc.
4. Change a setting (e.g., page orientation to Landscape)
5. Close preferences and reopen

**Expected Result**:
- All print ticket features display correctly
- Settings can be changed via ComboBoxes
- Changes persist across sessions
- UpdatePrintTicket is called successfully

#### 2.3 Print Copies and Collation
**Steps**:
1. Launch printer preferences
2. Find "Job Copies All Documents" NumberBox
3. Set copies to 1 - verify Collate ComboBox is disabled
4. Set copies to 3 - verify Collate ComboBox is enabled
5. Change collate setting

**Expected Result**:
- Collate ComboBox is disabled when copies = 1
- Collate ComboBox is enabled when copies > 1
- Inter-dependency works correctly

### 3. Print Workflow Tests

#### 3.1 Basic Print Job
**Objective**: Verify PrintSupportWorkflowBackgroundTask executes

**Steps**:
1. Install a test printer with this PSA
2. Open Notepad and create a test document
3. Print > Select the test printer
4. Click Print
5. Check app logs for workflow execution

**Expected Result**:
- PrintSupportWorkflowBackgroundTask.Run() is called
- OnJobStarting event fires
- OnPdlModificationRequested event fires
- Print job completes successfully

#### 3.2 Job UI Activation
**Objective**: Verify JobActivatedMainPage displays during print job

**Steps**:
1. Uncomment UI launcher code in PrintSupportWorkflowBackgroundTask (line 85)
2. Rebuild and reinstall app
3. Print a test document
4. Verify JobActivatedMainPage is displayed

**Expected Result**:
- JobActivatedMainPage displays during print job
- JobNotification event handler is called
- PdlDataAvailable event handler is called
- WatermarkManipulationExample page displays

**Known Issue**:
PrintWorkflowJobActivatedEventArgs retrieval may fail - see README.txt

#### 3.3 Watermark Application
**Objective**: Verify watermarks are applied to XPS documents

**Pre-requisites**: XpsUtil must be built with MSBuild

**Steps**:
1. Launch printer preferences
2. Navigate to watermark settings (if accessible via JobUI)
3. Configure watermark:
   - Text: "CONFIDENTIAL"
   - Font size: 48
   - Position: Center
4. Print a test document
5. Open output PDF and verify watermark

**Expected Result**:
- Watermark settings are saved to LocalStorage
- XpsPageWatermarker is configured correctly
- Watermark appears on printed document
- XPS to PDF conversion succeeds

### 4. Virtual Printer Tests

#### 4.1 Create Virtual Printer (PDF)
**Objective**: Verify virtual PDF printer can be created

**Steps**:
1. Launch app normally
2. Navigate to Virtual Printers page
3. Click "Add Virtual Printer"
4. Configure:
   - Printer Name: "Test PDF Printer"
   - Printer URI: "test-psa:print-to-pdf"
   - Preferred Format: OpenXps
   - Enable File Printer: Yes
   - Output Extensions: "pdf"
5. Click Add
6. Check Windows Settings > Printers to verify printer appears

**Expected Result**:
- Virtual printer is created successfully
- Printer appears in Windows printers list
- Printer shows "Test PDF Printer" as display name

#### 4.2 Print to Virtual Printer
**Objective**: Verify VirtualPrinterBackgroundTask executes

**Steps**:
1. Create virtual PDF printer (as above)
2. Open Notepad with test content
3. Print > Select "Test PDF Printer"
4. Choose save location for PDF
5. Verify PDF is created

**Expected Result**:
- VirtualPrinterBackgroundTask.Run() is called
- Session_VirtualPrinterDataAvailable event fires
- XPS to PDF conversion succeeds
- PDF file is created at chosen location

#### 4.3 Virtual Printer with Watermark
**Objective**: Verify watermarks work with virtual printers

**Pre-requisites**: XpsUtil must be built with MSBuild

**Steps**:
1. Print to virtual PDF printer
2. In the UI that appears, configure watermark
3. Click submit/continue
4. Verify output PDF has watermark

**Expected Result**:
- UI launches for watermark configuration
- Watermark settings are applied
- Output PDF contains watermark

#### 4.4 Remove Virtual Printer
**Steps**:
1. Launch app normally
2. Navigate to Virtual Printers page
3. Click "Refresh" to load printers
4. Find test printer and click "Remove"
5. Confirm removal
6. Verify printer is removed from Windows

**Expected Result**:
- Remove button works
- Confirmation dialog appears
- Printer is removed from Windows
- Printer disappears from app list after refresh

### 5. IPP Printer Discovery Tests

#### 5.1 Query Local IPP Printers
**Steps**:
1. Launch app normally
2. Navigate to Get IPP Printer URL
3. Click "Start Query"
4. Wait for query to complete

**Expected Result**:
- QueryLocalIppPrinters executes
- DeviceInformation.FindAllAsync completes
- IPP printers are listed with:
  - Printer name
  - Printer URL
  - IsPaired status
  - PSAMatched indicator

#### 5.2 Query by Manufacturer
**Steps**:
1. Navigate to Get IPP Printer URL
2. Enter manufacturer name (e.g., "HP", "Canon")
3. Click "Start Query"
4. Verify filtered results

**Expected Result**:
- QueryIppPrintersAssociationEndpointInfo executes
- Results are filtered by manufacturer
- Only matching printers display

### 6. Extension Background Task Tests

#### 6.1 Print Device Capabilities Changed
**Objective**: Verify PrintSupportExtensionBackgroundTask handles PDC changes

**Steps**:
1. Install app with a test printer
2. Modify printer capabilities (if possible)
3. Check logs for ExtensionBackgroundTask execution

**Expected Result**:
- PrintDeviceCapabilitiesChanged event fires
- Custom media types are added to PDC
- PDC manipulation succeeds

#### 6.2 Print Ticket Validation
**Steps**:
1. Configure printer with this PSA
2. Create print ticket with invalid settings
3. Verify validation occurs

**Expected Result**:
- Invalid settings are corrected
- Print ticket validation succeeds
- Job continues without errors

### 7. Localization Tests

#### 7.1 String Resources
**Steps**:
1. Check that virtual printers use localized display names
2. Verify "PdfPrintDisplayName" resolves to "VirtualPdfPrinter"
3. Verify "PsPrintDisplayName" resolves to "VirtualPostScriptPrinter"

**Expected Result**:
- ms-resource references resolve correctly
- Display names appear in English (en-US)

#### 7.2 Media Type Resources
**Steps**:
1. Open printer preferences
2. Check media type names (paper sizes)
3. Verify human-readable names display

**Expected Result**:
- ContosoMediaTypes.resw provides custom names
- IppMediaTypes.resw provides IPP names
- Names display correctly in UI

### 8. Configuration Files Tests

#### 8.1 PrinterPdc.xml Loading
**Steps**:
1. Create virtual printer with PdcFile="Config\PrinterPdc.xml"
2. Verify printer capabilities include custom features
3. Check for custom job feature (JobCustomFeature)

**Expected Result**:
- PrinterPdc.xml is loaded successfully
- Custom features appear in printer capabilities
- Paper sizes match PDC configuration

#### 8.2 PrinterPdr.xml Loading
**Steps**:
1. Open printer preferences
2. Check display names for IPP features
3. Verify names come from PDR file

**Expected Result**:
- PrinterPdr.xml provides localized names
- IPP features show user-friendly names
- Main tray shows as "Main tray" not raw IPP value

## Performance Tests

### P1. Settings Launch Time
**Steps**:
1. Measure time from right-click > Printing Preferences to UI display
2. Should be < 2 seconds

### P2. Print Job Processing Time
**Steps**:
1. Print a 10-page document
2. Measure time from Print button to job completion
3. With watermark vs without watermark

### P3. Virtual Printer Creation Time
**Steps**:
1. Measure time to create virtual printer
2. Should be < 5 seconds

## Error Handling Tests

### E1. Missing XpsUtil.dll
**Steps**:
1. Delete XpsUtil.dll from app package
2. Attempt to print with watermarking
3. Verify graceful error handling

**Expected Result**:
- Error is logged
- User is notified
- App doesn't crash

### E2. Invalid Print Ticket
**Steps**:
1. Create print ticket with incompatible settings
2. Attempt to print
3. Verify validation corrects issues

### E3. Cancel Print Job
**Steps**:
1. Start print job
2. Cancel from print dialog
3. Verify cleanup occurs

**Expected Result**:
- OnPdlModificationRequested handles cancellation
- Resources are released
- No memory leaks

## Regression Tests

After any code changes, re-run:
- Application Launch Tests (1.1-1.4)
- Settings Activation Tests (2.1-2.2)
- Print Workflow Tests (3.1)
- Virtual Printer Tests (4.1-4.2)

## Known Issues to Verify

### Issue 1: PrintWorkflowJobActivatedEventArgs Retrieval
**Description**: Unable to cast activation args to PrintWorkflowJobActivatedEventArgs

**Test**:
- Run test 3.2 (Job UI Activation)
- Check if .NET 10 and CsWinRT 2.2+ resolved the issue

**Status**: To be tested

### Issue 2: XpsUtil Build Requirement
**Description**: Cannot build with dotnet CLI

**Workaround**:
- Build with MSBuild/Visual Studio
- Pre-built binaries work for testing

## Test Environment Checklist

- [ ] Windows 11 Build 26100 or later
- [ ] .NET 10 SDK installed
- [ ] Visual Studio 2022 (for XpsUtil build)
- [ ] Test printer available
- [ ] IPP-compatible printer available (for IPP tests)
- [ ] Administrator privileges (for virtual printer creation)

## Success Criteria

The migration is successful if:
1. All 8 test scenarios pass
2. No crashes or unhandled exceptions
3. UI displays correctly in all activation modes
4. Print workflow executes end-to-end
5. Watermarking works (with XpsUtil)
6. Virtual printers can be created and used
7. All background tasks execute without errors
8. Settings persist across sessions

## Bug Tracking

Use this format for any bugs found:

**Bug ID**: [Number]
**Severity**: Critical / High / Medium / Low
**Title**: [Short description]
**Steps to Reproduce**: [Detailed steps]
**Expected**: [What should happen]
**Actual**: [What actually happens]
**Workaround**: [If any]
**Status**: Open / Fixed / Won't Fix
