# Microsoft Developer Studio Project File - Name="SGUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SGUI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SGUI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SGUI.mak" CFG="SGUI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SGUI - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SGUI - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SGUI - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\Codes\App" /I "..\..\Codes\GUI\Config" /I "..\..\Codes\GUI\Core\inc" /I "..\..\Codes\GUI\Fonts" /I "..\..\Codes\GUI\Widget" /I "..\..\Codes\sdl\include\win32" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /Zm200 /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# Begin Special Build Tool
OutDir=.\Release
SOURCE="$(InputPath)"
PreLink_Cmds=copy ..\..\Codes\sdl\win32\SDL.dll $(OutDir)
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SGUI - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /GX /ZI /Od /I "..\..\Codes\App" /I "..\..\Codes\GUI\Config" /I "..\..\Codes\GUI\Core\inc" /I "..\..\Codes\GUI\Fonts" /I "..\..\Codes\GUI\Widget" /I "..\..\Codes\sdl\include\win32" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBC" /FR /YX /FD /GZ /Zm200 /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0x10000 /subsystem:console /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
OutDir=.\Debug
SOURCE="$(InputPath)"
PreLink_Cmds=copy ..\..\Codes\sdl\win32\SDL.dll $(OutDir)
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SGUI - Win32 Release"
# Name "SGUI - Win32 Debug"
# Begin Group "App"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "app_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Codes\App\Bitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\App\game.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\App\GUI_Test.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\App\sdlscreen.h
# End Source File
# End Group
# Begin Group "app_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Codes\App\game.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\App\GUI_Test.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\App\main.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\App\pic_rootwin.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\App\sdlscreen.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Group "Config"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Codes\GUI\Config\GUI_Botton.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Config\GUI_Botton.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Config\GUI_Config.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Config\GUI_Typedef.h
# End Source File
# End Group
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Group "core_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Bitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_ClipArea.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Color.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Core.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Device.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_DispChar.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_GL.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_GL_AA.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_KeyBoard.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Malloc.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Math.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_MemDev.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Queue.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Rect.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_String.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Surface.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_Timer.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_TouchPad.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\GUI_WM.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\linked_list.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\inc\RootWindow.h
# End Source File
# End Group
# Begin Group "core_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Bitmap.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_ClipArea.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Color.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Core.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Device.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_DispChar.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_GL.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_GL_AA.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_KeyBoard.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Malloc.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Math.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_MemDev.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Queue.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Rect.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_String.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Surface.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_Timer.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_TouchPad.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\GUI_WM.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\linked_list.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Core\src\RootWindow.c
# End Source File
# End Group
# End Group
# Begin Group "Fonts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Codes\GUI\Fonts\ASCII_8x16.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Fonts\GUI_Font.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Fonts\UI17_4pp.c
# End Source File
# End Group
# Begin Group "Widget"

# PROP Default_Filter ""
# Begin Group "widget_inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Codes\GUI\Widget\BUTTON.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Widget\CHECKBOX.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Widget\WIDGET.c
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Widget\WINDOW.c
# End Source File
# End Group
# Begin Group "widget_src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Codes\GUI\Widget\BUTTON.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Widget\CHECKBOX.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Widget\WIDGET.h
# End Source File
# Begin Source File

SOURCE=..\..\Codes\GUI\Widget\WINDOW.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "SDL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Codes\sdl\include\win32\SDL\SDL.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Codes\sdl\win32\SDL.lib
# End Source File
# End Target
# End Project
