# Microsoft Developer Studio Project File - Name="RMarker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RMarker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RMarker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RMarker.mak" CFG="RMarker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RMarker - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RMarker - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RMarker - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "..\Release\rmarker"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RMARKER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\h" /I "..\hh" /I "..\hhh" /I "..\usage" /I "..\hrk" /I "..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RMARKER_EXPORTS" /D "__RMARKER__" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 cline.lib ccom.lib /nologo /dll /machine:I386 /libpath:"..\bin"

!ELSEIF  "$(CFG)" == "RMarker - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bind"
# PROP Intermediate_Dir "..\Debug\rmarker"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RMARKER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\h" /I "..\hh" /I "..\hhh" /I "..\usage" /I "..\hrk" /I "..\hdebug" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__NT__" /D "__RMARKER__" /FR /FD /GZ /c
# SUBTRACT CPP /X /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cline.lib ccom.lib /nologo /dll /incremental:no /debug /machine:I386 /libpath:"..\bind"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RMarker - Win32 Release"
# Name "RMarker - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=..\USAGE\AMU_Geom.CPP
# End Source File
# Begin Source File

SOURCE=.\dll.cpp
# End Source File
# Begin Source File

SOURCE=..\USAGE\Line_AM_Util.CPP
# End Source File
# Begin Source File

SOURCE=..\USAGE\Line_VP_2_AM.CPP
# End Source File
# Begin Source File

SOURCE=..\USAGE\Line_VP_Util.CPP
# End Source File
# Begin Source File

SOURCE=..\USAGE\MarkDataOper.CPP
# End Source File
# Begin Source File

SOURCE=..\USAGE\Puma_Err.CPP
# End Source File
# Begin Source File

SOURCE=.\RMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\RmarkerMem.cpp
# End Source File
# Begin Source File

SOURCE=.\rmtimer.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortVerticalLinesFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\USAGE\UN_Buff.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MarkPage.h
# End Source File
# Begin Source File

SOURCE=.\mymem.h
# End Source File
# Begin Source File

SOURCE=..\HHH\RMarker.h
# End Source File
# Begin Source File

SOURCE=.\RmarkerMem.h
# End Source File
# Begin Source File

SOURCE=.\rmfunc.h
# End Source File
# Begin Source File

SOURCE=.\ShortVerticalLinesFilter.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\rmarker.rc
# End Source File
# End Group
# End Target
# End Project
