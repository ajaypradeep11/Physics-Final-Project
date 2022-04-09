!include "LogicLib.nsh"

OutFile "ILoveOpenGL.exe"

Function InstallationCheck
   ${If} ${FileExists} "$DESKTOP\Physics Final\ILoveOpenGL.exe"
	    ${OrIf} ${FileExists} "$DESKTOP\Physics Final\ILoveOpenGL.pdb"
        	MessageBox MB_YESNO|MB_ICONSTOP "Physics Game is already installed Do you want to overwrite?" IDYES yes IDNO cancel
        	yes:
            		DetailPrint "Installing (Overwriting)"
            		Goto next
        	cancel:
            		DetailPrint "Cancelling Installation"
            		Quit
        	next:
    ${EndIf}

FunctionEnd

InstallDir "$DESKTOP\Physics Final"

Section

Call InstallationCheck

CreateDirectory "$DESKTOP\Physics Final"

SetOutPath $INSTDIR

File "ILoveOpenGL.exe"

WriteUninstaller "$INSTDIR\Game_uninstaller.exe"

SectionEnd

Section "Uninstall"

Delete "$INSTDIR\Game_uninstaller.exe"

Delete "$INSTDIR\ILoveOpenGL.exe"

Delete "$INSTDIR\ILoveOpenGL.pdb"

RMDir $INSTDIR

SectionEnd