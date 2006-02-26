# Microsoft Developer Studio Project File - Name="forum" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=forum - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "forum.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "forum.mak" CFG="forum - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "forum - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "forum - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "forum - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "H:\PWS\MySQL\include" /I "misc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 libmysql.lib zlib.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /profile /machine:I386 /out:"/is/apache2/cgi-bin/forum.cgi"

!ELSEIF  "$(CFG)" == "forum - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "H:\PWS\MySQL\include" /I "misc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libmysql.lib zlib.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "forum - Win32 Release"
# Name "forum - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\misc\cgi.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\cookie.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\sql\database.cpp
# End Source File
# Begin Source File

SOURCE=.\form_base.cpp
# End Source File
# Begin Source File

SOURCE=.\form_config.cpp
# End Source File
# Begin Source File

SOURCE=.\form_ipa_search.cpp
# End Source File
# Begin Source File

SOURCE=.\form_languages.cpp
# End Source File
# Begin Source File

SOURCE=.\form_layouts.cpp
# End Source File
# Begin Source File

SOURCE=.\form_login.cpp
# End Source File
# Begin Source File

SOURCE=.\form_message.cpp
# End Source File
# Begin Source File

SOURCE=.\form_password.cpp
# End Source File
# Begin Source File

SOURCE=.\form_preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\form_profile.cpp
# End Source File
# Begin Source File

SOURCE=.\form_register.cpp
# End Source File
# Begin Source File

SOURCE=.\form_search.cpp
# End Source File
# Begin Source File

SOURCE=.\form_smilies.cpp
# End Source File
# Begin Source File

SOURCE=.\form_styles.cpp
# End Source File
# Begin Source File

SOURCE=.\forum.cpp
# End Source File
# Begin Source File

SOURCE=.\forum_database.cpp
# End Source File
# Begin Source File

SOURCE=.\forum_global.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\html.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\html_template.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\multi_line.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\sql\sql_query.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\sql\sql_result.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\misc\string_conversion.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\virtual_binary.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\web_tools.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\xcc_error.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\xcc_z.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\xf2_mm.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\form_base.h
# End Source File
# Begin Source File

SOURCE=.\form_config.h
# End Source File
# Begin Source File

SOURCE=.\form_ipa_search.h
# End Source File
# Begin Source File

SOURCE=.\form_languages.h
# End Source File
# Begin Source File

SOURCE=.\form_layouts.h
# End Source File
# Begin Source File

SOURCE=.\form_login.h
# End Source File
# Begin Source File

SOURCE=.\form_message.h
# End Source File
# Begin Source File

SOURCE=.\form_password.h
# End Source File
# Begin Source File

SOURCE=.\form_preferences.h
# End Source File
# Begin Source File

SOURCE=.\form_profile.h
# End Source File
# Begin Source File

SOURCE=.\form_register.h
# End Source File
# Begin Source File

SOURCE=.\form_search.h
# End Source File
# Begin Source File

SOURCE=.\form_smilies.h
# End Source File
# Begin Source File

SOURCE=.\form_styles.h
# End Source File
# Begin Source File

SOURCE=.\forum_database.h
# End Source File
# Begin Source File

SOURCE=.\forum_global.h
# End Source File
# Begin Source File

SOURCE=.\result_cache.h
# End Source File
# Begin Source File

SOURCE=.\misc\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
