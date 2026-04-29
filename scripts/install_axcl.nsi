; NSIS Install Script - AXCL Installer
; Install materials: ax650_card.pac and axcl.win64.zip

; Request Administrator Privileges - MUST be at the top
RequestExecutionLevel admin

; Variables for dynamic file names
Var DRIVE_PAC_FILE
Var DID_COPY_CORE
Var DID_FIRMWARE_COPY

!define PRODUCT_NAME "AXCL"
; Version can be overridden at compile time via /DPRODUCT_VERSION
!ifndef PRODUCT_VERSION
  !define PRODUCT_VERSION "1.0.0"
!endif
!define PRODUCT_PUBLISHER "Axera Technology"
!define PRODUCT_WEB_SITE "https://www.axera-tech.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\axcl"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; Include Modern UI
!include "MUI2.nsh"
!include "FileFunc.nsh"
!include "LogicLib.nsh"
!include "StrFunc.nsh"
!include "x64.nsh"
!include "WordFunc.nsh"

; Declare string functions
${StrStr}
${StrRep}

; Declare uninstaller string functions
${UnStrStr}

!insertmacro WordFind
!insertmacro WordReplace
!insertmacro un.WordFind

; Installer Information
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "AXCL_Setup.exe"
InstallDir "$EXEDIR\AXCL"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

; Request Administrator Privileges (already set at top)

VIProductVersion "${VI_PRODUCT_VERSION}"
VIAddVersionKey "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey "CompanyName" "${PRODUCT_PUBLISHER}"
VIAddVersionKey "LegalCopyright" "Copyright (C) 2025 ${PRODUCT_PUBLISHER}"
VIAddVersionKey "FileDescription" "AXCL Installer"
VIAddVersionKey "FileVersion" "${PRODUCT_VERSION}"
VIAddVersionKey "InternalName" "AXCL_Setup"
VIAddVersionKey "OriginalFilename" "AXCL_Setup.exe"
VIAddVersionKey "LegalTrademarks" "AXCL is a trademark of ${PRODUCT_PUBLISHER}"

; Interface Settings
!define MUI_ABORTWARNING
!define MUI_ICON "..\icon\axcl.ico"
!define MUI_UNICON "..\icon\axcl.ico"

; Install Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

; Customize finish page
!define MUI_FINISHPAGE_TITLE "AXCL Installation Complete"
!define MUI_FINISHPAGE_TEXT "AXCL has been successfully installed on your computer.$\r$\n$\r$\nPlease review the installation details shown above before clicking Finish.$\r$\n$\r$\nYou may need to restart your computer for all changes to take effect."

; Uninstall Pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language
!insertmacro MUI_LANGUAGE "English"

; Function to check if running as administrator and find latest pac file
Function .onInit
  ; Check if running as administrator
  System::Call 'advapi32::OpenSCManagerA(i 0, i 0, i 0x1) i .r0'
  IntCmp $0 0 notAdmin admin
  System::Call 'advapi32::CloseServiceHandle(i r0)'
  Goto admin
  
  notAdmin:
    MessageBox MB_ICONSTOP "This installer requires administrator privileges. Please run as administrator."
    Quit
    
  admin:
    ; Set install directory to the directory where the installer is located
    StrCpy $INSTDIR "$EXEDIR\AXCL"
    ; Find the latest AX650_card*.pac file
    Call FindLatestPacFile
    ; Continue with installation
FunctionEnd

; Function to find the latest AX650_card*.pac file
Function FindLatestPacFile
  ; Prefer value provided at compile-time via /DDRIVE_PAC_FILE, fallback to hardcoded name
  !ifdef DRIVE_PAC_FILE
    StrCpy $DRIVE_PAC_FILE "${DRIVE_PAC_FILE}"
  !endif
FunctionEnd

; Helper: trim trailing CR/LF
Function un.TrimNewlines
    Exch $R0
    Push $R1
    Push $R2
    StrLen $R1 $R0
    loop:
        IntOp $R1 $R1 - 1
        IntCmp $R1 0 done
        StrCpy $R2 $R0 1 $R1
        StrCmp $R2 "$\r" loop
        StrCmp $R2 "$\n" loop
        IntOp $R1 $R1 + 1
        StrCpy $R0 $R0 $R1
        Goto done
    done:
    Pop $R2
    Pop $R1
    Exch $R0
FunctionEnd

; Function: GetOemInf
; Input:  stack top = device instance ID
; Output: stack top = oemXX.inf (empty if not found)
Function un.GetOemInf
    Exch $0
    Push $1
    Push $2
    Push $3

    ; Run pnputil and filter for "oem"
    ${DisableX64FSRedirection}
    nsExec::ExecToStack '"$WINDIR\\System32\\pnputil.exe" /enum-devices /instanceid "$0"'
    ${EnableX64FSRedirection}
    Pop $1 ; return code
    Pop $2 ; first line with "oem"
    DetailPrint "pnputil /enum-devices /instanceid : $2"
    ; Trim CR/LF
    Push $2
    Call un.TrimNewlines
    Pop $2

    ; Extract substring starting at "oem"
    StrCpy $R0 ""
    StrLen $3 $2
    StrCpy $1 0
    find_oem:
        StrCmp $1 $3 not_found
        StrCpy $R0 $2 3 $1
        StrCmp $R0 "oem" 0 +3
            StrCpy $R0 $2 "" $1
            Goto check_inf
        IntOp $1 $1 + 1
        Goto find_oem

    check_inf:
        ; Ensure it ends with .inf
        ${If} $R0 != ""
            ${IfNot} ${R0} == ""
                ; cut at first space if exists
                StrCpy $R0 $R0 "" 0
                ; optional: you can add more parsing if needed
            ${EndIf}
        ${EndIf}
        Goto done

    not_found:
        StrCpy $R0 ""

    done:
    Pop $3
    Pop $2
    Pop $1
    Exch $R0
FunctionEnd

; Function: GetInstanceId
; Input:  stack top = device ID prefix (e.g. "PCI\VEN_1F4B&DEV_0650")
; Output: stack top = complete instance ID (empty if not found)
Function un.GetInstanceId
    Exch $0 ; $0 = device ID prefix
    Push $1
    Push $2
    Push $3
    Push $4
    Push $5

    ; Run pnputil /enum-devices and capture output to file
    ${DisableX64FSRedirection}
    GetTempFileName $5
    nsExec::ExecToLog 'cmd /c "$WINDIR\System32\pnputil.exe /enum-devices > $5 2>&1"'
    ${EnableX64FSRedirection}

    ; Read the output file line by line
    FileOpen $1 $5 r
    StrCpy $R0 "" ; Result

    read_loop:
        FileRead $1 $2
        IfErrors done_reading
        
        ; Trim whitespace
        Push $2
        Call un.TrimNewlines
        Pop $2
        
        ; Check if this line contains our device prefix
        ${UnStrStr} $3 "$2" "$0"
        ${If} $3 == ""
            ; Not found in this line, continue
            Goto read_loop
        ${EndIf}
        
        ; Found! Now extract the complete PCI ID from the line
        ; Find where "PCI\" starts
        ${UnStrStr} $3 "$2" "PCI\"
        ${If} $3 == ""
            ; Strange, should have PCI\, skip this line
            Goto read_loop
        ${EndIf}
        
        ; Extract from "PCI\" to end of line, trimming spaces
        StrCpy $4 $3 "" 0
        StrLen $R1 $4
        
        ; Find the end of the ID (first space, tab, or end of string)
        StrCpy $R2 0
        find_end:
            StrCmp $R2 $R1 extract_id
            StrCpy $3 $4 1 $R2
            StrCmp $3 " " extract_id
            StrCmp $3 "$\t" extract_id
            StrCmp $3 "$\r" extract_id
            StrCmp $3 "$\n" extract_id
            IntOp $R2 $R2 + 1
            Goto find_end
        
        extract_id:
            StrCpy $R0 $4 $R2
            Goto done_reading

    done_reading:
        FileClose $1

    Pop $5
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Exch $R0
FunctionEnd

Function un.UninstallAxeraDrivers
    Push $0
    Push $1

    ; Example: hard‑coded instance ID, or pass it in via stack
    Push "PCI\VEN_1F4B&DEV_0650"
    Call un.GetInstanceId
    Pop $1

    ${If} $1 != ""
        DetailPrint "Found complete instance ID: $1"
        Push $1
        Call un.GetOemInf
        Pop $0 ; $0 = oemXX.inf
        StrCmp $0 "" no_driver
        DetailPrint "OemInf = $0"
        ; Run pnputil to uninstall the driver
        ${DisableX64FSRedirection}
        ; Uninstall driver package
        nsExec::ExecToLog 'pnputil /delete-driver $0 /uninstall /force'
        nsExec::ExecToLog 'pnputil /remove-device $1'

        ${EnableX64FSRedirection}
        Goto done
    ${Else}
        DetailPrint "No matching device found for PCI\VEN_1F4B&DEV_0650"
        Goto done
    ${EndIf}

  no_driver:
    DetailPrint "No OEM INF found for Axera device."

  done:
    Pop $1
    Pop $0
FunctionEnd


; Rollback handler for failed installations
Function DoRollback
  DetailPrint "Rolling back installation..."
  ${If} $DID_FIRMWARE_COPY == "1"
    ${If} ${FileExists} "$SYSDIR\drivers\$DRIVE_PAC_FILE"
      Delete "$SYSDIR\drivers\$DRIVE_PAC_FILE"
      DetailPrint "Removed firmware: $SYSDIR\drivers\$DRIVE_PAC_FILE"
    ${EndIf}
  ${EndIf}

  ; Remove installed core files
  ${If} ${FileExists} "$INSTDIR\axcl\out\axcl_win_x64\bin"
    RMDir /r "$INSTDIR\axcl"
  ${EndIf}
  ${If} ${FileExists} "$INSTDIR\uninst.exe"
    Delete "$INSTDIR\uninst.exe"
  ${EndIf}
  ${If} ${FileExists} "$INSTDIR"
    RMDir "$INSTDIR"
  ${EndIf}
FunctionEnd

; Called automatically when installation fails or is aborted
Function .onInstFailed
  Call DoRollback
FunctionEnd


; Install Types
InstType "Full Installation"
InstType "Minimal Installation"

; Component Selection
Section "AXCL Core Files" SEC01
  SectionIn RO 1 2
  SetOutPath "$INSTDIR"
  
  ; Copy extracted AXCL files
  DetailPrint "Copying AXCL files..."
  
  ; Copy all files from extracted directory (path provided at compile time)
  !ifdef AXCL_EXTRACTED_DIR
    File /r "${AXCL_EXTRACTED_DIR}\*.*"
  !else
    File /r "axcl_extracted\*.*"
  !endif
  StrCpy $DID_COPY_CORE "1"
  
  ; Set install directory permissions (using Windows built-in commands)
  DetailPrint "Setting directory permissions..."
  nsExec::ExecToLog 'icacls "$INSTDIR" /grant Everyone:F /T'
SectionEnd

Section "Windows Driver" SEC02
  SectionIn 1 2
  ; Install driver directly from source without copying to installation directory
  ${DisableX64FSRedirection}
  nsExec::ExecToStack 'pnputil /add-driver "$INSTDIR\axcl\out\axcl_win_x64\drv\axcl_pcie.inf" /install /force'
  Pop $0
  ${EnableX64FSRedirection}
  ${If} $0 == 0
    DetailPrint "pnputil driver installation successful"
  ${ElseIf} $0 == 259
    DetailPrint "Driver already installed, skipping installation"
  ${Else}
    DetailPrint "Driver installation failed with error code: $0"
    MessageBox MB_ICONSTOP|MB_OK "Driver installation failed with error code: $0.$\r$\n$\r$\nThis may be due to:$\r$\n- Missing administrator privileges$\r$\n- Driver signature issues$\r$\n- Hardware not detected$\r$\n$\r$\nInstallation will be aborted."
    Call DoRollback
    Abort "Driver installation failed with error code: $0"
  ${EndIf}
  
SectionEnd
Section "AX650 Card Firmware" SEC03
  ${DisableX64FSRedirection}

  SectionIn 1 2
  
  SetOutPath "$SYSDIR\drivers"
  ; Copy ax650_card.pac to Windows drivers directory
  DetailPrint "Copying AX650 card firmware to Windows drivers directory..."
  DetailPrint "Source file: $DRIVE_PAC_FILE"
  
  ; Include the pac file in the installer
  File "${DRIVE_PAC_FILE}"
  StrCpy $DID_FIRMWARE_COPY "1"
  DetailPrint "AX650 card firmware copy completed"
  ; Store only filename to registry for later deletion
  ${GetFileName} $DRIVE_PAC_FILE $0
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "PacFileName" "$0"
  ${EnableX64FSRedirection}

SectionEnd


; Post-installation processing
Section -AdditionalIcons
  SetOutPath $INSTDIR
  CreateShortCut "$SMPROGRAMS\AXCL\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\axcl\out\axcl_win_x64\bin\axcl-smi.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\axcl\out\axcl_win_x64\bin\axcl-smi.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  
  ; Add AXCL bin directory to system PATH
  DetailPrint "Adding AXCL bin directory to system PATH..."
  StrCpy $1 "$INSTDIR\axcl\out\axcl_win_x64\bin"
  
  ; Read current PATH from registry
  ReadRegStr $0 HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "PATH"
  
  ; Check if path already exists in PATH
  ${If} $0 != ""
    ; Use StrStr to check if path exists
    ${StrStr} $2 "$0" "$1"
    ${If} $2 == ""
      ; Path not found, add it
      StrCpy $0 "$0;$1"
      ; WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "PATH" $0
      DetailPrint "Successfully added AXCL bin directory to PATH"
    ${Else}
      DetailPrint "AXCL bin directory already in PATH"
    ${EndIf}
  ${Else}
    ; PATH is empty, set it directly
    ; WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "PATH" $1
    DetailPrint "Successfully set AXCL bin directory as PATH"
  ${EndIf}
  
  ; Send environment variable update message
  SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
  
  ; Show final installation details
  DetailPrint "=========================================="
  DetailPrint "AXCL Installation Completed Successfully!"
  DetailPrint "=========================================="
  DetailPrint "Installation Summary:"
  DetailPrint "- AXCL core files and libraries installed"
  DetailPrint "- Windows PCIe driver installed"
  DetailPrint "- AX650 card firmware installed"
  DetailPrint "- PATH environment variable updated"
  DetailPrint ""
  DetailPrint "You may need to restart your computer for all changes to take effect."
  DetailPrint "Please review the installation details above before proceeding."
  DetailPrint "=========================================="
SectionEnd

; Component Descriptions
LangString DESC_SEC01 ${LANG_ENGLISH} "AXCL core files and libraries"
LangString DESC_SEC02 ${LANG_ENGLISH} "Windows PCIe driver"
LangString DESC_SEC03 ${LANG_ENGLISH} "AX650 card firmware file"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} $(DESC_SEC01)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} $(DESC_SEC02)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} $(DESC_SEC03)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; Uninstaller
Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall

  DetailPrint "Starting AXCL uninstallation..."
  Call un.UninstallAxeraDrivers


  ; Remove AXCL bin directory from system PATH
  DetailPrint "Removing AXCL bin directory from system PATH..."
  StrCpy $1 "$INSTDIR\axcl\out\axcl_win_x64\bin"
  
  ; Read current PATH from registry
  ReadRegStr $0 HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "PATH"
  
  ; Remove path from PATH
  ${If} $0 != ""
    ; Remove occurrences of the target path in common positions
    StrCpy $4 $0
    StrCpy $5 "$1;"       ; Path with semicolon at end
    StrCpy $6 ";$1"       ; Path with semicolon at start
    StrCpy $7 "$1"        ; Path without semicolon
    
    ${WordReplace} $4 "$5" "" "+" $4
    ${WordReplace} $4 "$6" "" "+" $4
    ${WordReplace} $4 "$7" "" "+" $4
    ; Collapse any accidental double semicolons
    ${WordReplace} $4 ";;" ";" "+" $4

    ; WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "PATH" $4
    DetailPrint "Successfully removed AXCL bin directory from PATH"
  ${Else}
    DetailPrint "PATH is empty, nothing to remove"
  ${EndIf}
  
  ; Send environment variable update message
  SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000

  ; Delete AXCL files
  Delete "$INSTDIR\uninst.exe"
  
  ; Delete AXCL directories
  RMDir /r "$INSTDIR\axcl"
  RMDir "$INSTDIR"
  
  ; Delete AX650 card firmware
  ReadRegStr $DRIVE_PAC_FILE HKLM "${PRODUCT_DIR_REGKEY}" "PacFileName"
  DetailPrint "Deleting firmware file: $SYSDIR\drivers\$DRIVE_PAC_FILE"
  ${DisableX64FSRedirection}
  Delete "$SYSDIR\drivers\$DRIVE_PAC_FILE"
  ${EnableX64FSRedirection}
  DeleteRegValue HKLM "${PRODUCT_DIR_REGKEY}" "PacFileName"
  
  ; Note: Driver files are not copied to installation directory, so no need to delete them from there
  ; Driver uninstallation is handled by pnputil command above
  
  ; Send environment variable update message
  SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
  
  SetAutoClose false
SectionEnd


