; NSIS installer script for Tonestack
;--------------------------------

!include "MUI2.nsh"

; The name of the installer
Name "Tonestack"

; The file to write
OutFile "tonestack.exe"

; Request application privileges for Windows Vista and higher
RequestExecutionLevel admin

; Build Unicode installer
Unicode True

; The default installation directory
InstallDir $PROGRAMFILES64\Tonestack

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Tonestack" "Install_Dir"

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING

;--------------------------------
;Pages

!insertmacro MUI_PAGE_LICENSE "LICENSE"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
!insertmacro MUI_LANGUAGE "English"

;--------------------------------

; The stuff to install
Section "Tonestack (required)"

	SectionIn RO

	SetOutPath "$INSTDIR\bin"
	File /r "bin\*"

	SetOutPath "$INSTDIR\circuits"
	File /r "circuits\*"

	SetOutPath "$INSTDIR"
	File "COPYING"
	File "LICENSE"

	; Write the installation path into the registry
	WriteRegStr HKLM SOFTWARE\Tonestack "Install_Dir" "$INSTDIR"

	; Write the uninstall keys for Windows
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tonestack" "DisplayName" "Tonestack"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tonestack" "UninstallString" '"$INSTDIR\uninstall.exe"'
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tonestack" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tonestack" "NoRepair" 1
	WriteUninstaller "$INSTDIR\uninstall.exe"
  
	;Create shortcuts
	SetOutPath "$INSTDIR\bin"
	CreateDirectory "$SMPROGRAMS\Tonestack"
	CreateShortcut "$SMPROGRAMS\Tonestack\Tonestack.lnk" "$INSTDIR\bin\Tonestack.exe"
	CreateShortcut "$SMPROGRAMS\Tonestack\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
	; Remove registry keys
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tonestack"
	DeleteRegKey /ifempty HKLM SOFTWARE\Tonestack

	; Remove files and uninstaller
	Delete $INSTDIR\uninstall.exe

	; Remove shortcuts, if any
	Delete "$SMPROGRAMS\Tonestack\*.lnk"

	; Remove directories
	RMDir "$SMPROGRAMS\Tonestack"
	RMDir /r /REBOOTOK "$INSTDIR"

SectionEnd
