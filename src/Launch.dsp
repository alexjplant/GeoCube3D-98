# Microsoft Developer Studio Project File - Name="Launch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Launch - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Launch.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Launch.mak" CFG="Launch - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Launch - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Launch - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Launch - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 dinput.lib ddraw.lib dplayx.lib d3drm.lib dsound.lib winmm.lib dxguid.lib vfw32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib"

!ELSEIF  "$(CFG)" == "Launch - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dinput.lib ddraw.lib dplayx.lib d3drm.lib dsound.lib winmm.lib dxguid.lib vfw32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "Launch - Win32 Release"
# Name "Launch - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bullet.cpp
# End Source File
# Begin Source File

SOURCE=.\dxa3drm.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\dxadraw.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=.\dxainput.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=.\dxamusic.cpp
# End Source File
# Begin Source File

SOURCE=.\dxaobj.cpp

!IF  "$(CFG)" == "Launch - Win32 Release"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "Launch - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dxaplay.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\dxasound.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=.\dxautil.cpp
# End Source File
# Begin Source File

SOURCE=.\DxaWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\dxerror.cpp
# End Source File
# Begin Source File

SOURCE=.\gameplay.cpp
# End Source File
# Begin Source File

SOURCE=.\Launch.cpp
# End Source File
# Begin Source File

SOURCE=.\Launch.rc
# End Source File
# Begin Source File

SOURCE=.\LaunchDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LaunchView.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LobbyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlinePages.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\player.cpp
# End Source File
# Begin Source File

SOURCE=.\playersh.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocksh.cpp
# End Source File
# Begin Source File

SOURCE=.\sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\bullet.h
# End Source File
# Begin Source File

SOURCE=.\dxa3drm.h
# End Source File
# Begin Source File

SOURCE=.\dxadraw.h
# End Source File
# Begin Source File

SOURCE=.\dxainput.h
# End Source File
# Begin Source File

SOURCE=.\dxamusic.h
# End Source File
# Begin Source File

SOURCE=.\dxaobj.h
# End Source File
# Begin Source File

SOURCE=.\dxaplay.h
# End Source File
# Begin Source File

SOURCE=.\dxasound.h
# End Source File
# Begin Source File

SOURCE=.\dxautil.h
# End Source File
# Begin Source File

SOURCE=.\DxaWnd.h
# End Source File
# Begin Source File

SOURCE=.\dxerror.h
# End Source File
# Begin Source File

SOURCE=.\gameplay.h
# End Source File
# Begin Source File

SOURCE=.\Launch.h
# End Source File
# Begin Source File

SOURCE=.\LaunchDoc.h
# End Source File
# Begin Source File

SOURCE=.\LaunchView.h
# End Source File
# Begin Source File

SOURCE=.\LoadDlg.h
# End Source File
# Begin Source File

SOURCE=.\LobbyDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\NewDlg.h
# End Source File
# Begin Source File

SOURCE=.\OnlineDlg.h
# End Source File
# Begin Source File

SOURCE=.\OnlinePages.h
# End Source File
# Begin Source File

SOURCE=.\OnlineSheet.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\player.h
# End Source File
# Begin Source File

SOURCE=.\playersh.h
# End Source File
# Begin Source File

SOURCE=.\PreviewWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\rocksh.h
# End Source File
# Begin Source File

SOURCE=.\sprite.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cube1.bin
# End Source File
# Begin Source File

SOURCE=.\head.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Launch.ico
# End Source File
# Begin Source File

SOURCE=.\res\Launch.rc2
# End Source File
# Begin Source File

SOURCE=.\res\LaunchDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\xof1.bin
# End Source File
# End Group
# Begin Source File

SOURCE=.\Launch.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
