# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Goggles - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Goggles - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Goggles - Win32 Release" && "$(CFG)" !=\
 "Goggles - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Goggles.mak" CFG="Goggles - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Goggles - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Goggles - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Goggles - Win32 Debug"
CPP=cl.exe
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "Goggles - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Goggles.exe"

CLEAN : 
	-@erase ".\Release\Goggles.exe"
	-@erase ".\Release\config.obj"
	-@erase ".\Release\about.obj"
	-@erase ".\Release\sunrise.obj"
	-@erase ".\Release\latlong.obj"
	-@erase ".\Release\notify.obj"
	-@erase ".\Release\utilsw.obj"
	-@erase ".\Release\interpol.obj"
	-@erase ".\Release\sunrisew.obj"
	-@erase ".\Release\state.obj"
	-@erase ".\Release\cities.obj"
	-@erase ".\Release\main.obj"
	-@erase ".\Release\gamma.obj"
	-@erase ".\Release\registry.obj"
	-@erase ".\Release\export.obj"
	-@erase ".\Release\time.obj"
	-@erase ".\Release\goggles.res"
	-@erase ".\Release\ini.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Goggles.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/goggles.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Goggles.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Goggles.pdb" /machine:I386 /out:"$(OUTDIR)/Goggles.exe" 
LINK32_OBJS= \
	"$(INTDIR)/config.obj" \
	"$(INTDIR)/about.obj" \
	"$(INTDIR)/sunrise.obj" \
	"$(INTDIR)/latlong.obj" \
	"$(INTDIR)/notify.obj" \
	"$(INTDIR)/utilsw.obj" \
	"$(INTDIR)/interpol.obj" \
	"$(INTDIR)/sunrisew.obj" \
	"$(INTDIR)/state.obj" \
	"$(INTDIR)/cities.obj" \
	"$(INTDIR)/main.obj" \
	"$(INTDIR)/gamma.obj" \
	"$(INTDIR)/registry.obj" \
	"$(INTDIR)/export.obj" \
	"$(INTDIR)/time.obj" \
	"$(INTDIR)/ini.obj" \
	"$(INTDIR)/goggles.res"

"$(OUTDIR)\Goggles.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Goggles.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\Goggles.exe"
	-@erase ".\Debug\registry.obj"
	-@erase ".\Debug\main.obj"
	-@erase ".\Debug\about.obj"
	-@erase ".\Debug\sunrise.obj"
	-@erase ".\Debug\latlong.obj"
	-@erase ".\Debug\utilsw.obj"
	-@erase ".\Debug\gamma.obj"
	-@erase ".\Debug\cities.obj"
	-@erase ".\Debug\interpol.obj"
	-@erase ".\Debug\notify.obj"
	-@erase ".\Debug\state.obj"
	-@erase ".\Debug\export.obj"
	-@erase ".\Debug\time.obj"
	-@erase ".\Debug\sunrisew.obj"
	-@erase ".\Debug\config.obj"
	-@erase ".\Debug\goggles.res"
	-@erase ".\Debug\ini.obj"
	-@erase ".\Debug\Goggles.ilk"
	-@erase ".\Debug\Goggles.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Goggles.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/goggles.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Goggles.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Goggles.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Goggles.exe" 
LINK32_OBJS= \
	"$(INTDIR)/registry.obj" \
	"$(INTDIR)/main.obj" \
	"$(INTDIR)/about.obj" \
	"$(INTDIR)/sunrise.obj" \
	"$(INTDIR)/latlong.obj" \
	"$(INTDIR)/utilsw.obj" \
	"$(INTDIR)/gamma.obj" \
	"$(INTDIR)/cities.obj" \
	"$(INTDIR)/interpol.obj" \
	"$(INTDIR)/notify.obj" \
	"$(INTDIR)/state.obj" \
	"$(INTDIR)/export.obj" \
	"$(INTDIR)/time.obj" \
	"$(INTDIR)/sunrisew.obj" \
	"$(INTDIR)/config.obj" \
	"$(INTDIR)/ini.obj" \
	"$(INTDIR)/goggles.res"

"$(OUTDIR)\Goggles.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Goggles - Win32 Release"
# Name "Goggles - Win32 Debug"

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\main.c

!IF  "$(CFG)" == "Goggles - Win32 Release"

DEP_CPP_MAIN_=\
	".\config.h"\
	".\gamma.h"\
	".\gogglesw.h"\
	".\sunrise.h"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

DEP_CPP_MAIN_=\
	".\config.h"\
	".\gamma.h"\
	".\gogglesw.h"\
	".\sunrise.h"\
	
NODEP_CPP_MAIN_=\
	".\TimeTick"\
	

"$(INTDIR)\main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\sunrise.c
DEP_CPP_SUNRI=\
	".\sunrise.h"\
	

"$(INTDIR)\sunrise.obj" : $(SOURCE) $(DEP_CPP_SUNRI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\sunrise.h

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\goggles.rc
DEP_RSC_GOGGL=\
	".\Goggles.exe.manifest"\
	".\icon1.ico"\
	".\always.ico"\
	".\never.ico"\
	".\night.ico"\
	".\day.ico"\
	

"$(INTDIR)\goggles.res" : $(SOURCE) $(DEP_RSC_GOGGL) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\sunrisew.c
DEP_CPP_SUNRIS=\
	".\gogglesw.h"\
	".\sunrise.h"\
	

"$(INTDIR)\sunrisew.obj" : $(SOURCE) $(DEP_CPP_SUNRIS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\time.c

!IF  "$(CFG)" == "Goggles - Win32 Release"

NODEP_CPP_TIME_=\
	".\{"\
	

"$(INTDIR)\time.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"


"$(INTDIR)\time.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\about.c

"$(INTDIR)\about.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\gogglesw.h

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\config.c
DEP_CPP_CONFI=\
	".\config.h"\
	".\gamma.h"\
	".\gogglesw.h"\
	

"$(INTDIR)\config.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\utilsw.c
DEP_CPP_UTILS=\
	".\gogglesw.h"\
	

"$(INTDIR)\utilsw.obj" : $(SOURCE) $(DEP_CPP_UTILS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\notify.c

!IF  "$(CFG)" == "Goggles - Win32 Release"

DEP_CPP_NOTIF=\
	".\gamma.h"\
	".\gogglesw.h"\
	

"$(INTDIR)\notify.obj" : $(SOURCE) $(DEP_CPP_NOTIF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

DEP_CPP_NOTIF=\
	".\gamma.h"\
	".\gogglesw.h"\
	
NODEP_CPP_NOTIF=\
	".\NOTIFYICONDATA"\
	

"$(INTDIR)\notify.obj" : $(SOURCE) $(DEP_CPP_NOTIF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\gamma.c
DEP_CPP_GAMMA=\
	".\gamma.h"\
	".\gogglesw.h"\
	".\sunrise.h"\
	

"$(INTDIR)\gamma.obj" : $(SOURCE) $(DEP_CPP_GAMMA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\gamma.h

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\temp.h

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\config.h

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\latlong.c

!IF  "$(CFG)" == "Goggles - Win32 Release"

NODEP_CPP_LATLO=\
	".\;"\
	

"$(INTDIR)\latlong.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"


"$(INTDIR)\latlong.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\interpol.c

!IF  "$(CFG)" == "Goggles - Win32 Release"

NODEP_CPP_INTER=\
	".\1"\
	

"$(INTDIR)\interpol.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"


"$(INTDIR)\interpol.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\state.c

!IF  "$(CFG)" == "Goggles - Win32 Release"

DEP_CPP_STATE=\
	".\gamma.h"\
	".\gogglesw.h"\
	".\sunrise.h"\
	".\temp.h"\
	

"$(INTDIR)\state.obj" : $(SOURCE) $(DEP_CPP_STATE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

DEP_CPP_STATE=\
	".\gamma.h"\
	".\gogglesw.h"\
	".\sunrise.h"\
	".\temp.h"\
	
NODEP_CPP_STATE=\
	".\{"\
	

"$(INTDIR)\state.obj" : $(SOURCE) $(DEP_CPP_STATE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\export.c
DEP_CPP_EXPOR=\
	".\gogglesw.h"\
	

"$(INTDIR)\export.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\cities.txt

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cities.c

!IF  "$(CFG)" == "Goggles - Win32 Release"

DEP_CPP_CITIE=\
	".\config.h"\
	".\gamma.h"\
	".\gogglesw.h"\
	

"$(INTDIR)\cities.obj" : $(SOURCE) $(DEP_CPP_CITIE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

DEP_CPP_CITIE=\
	".\config.h"\
	".\gamma.h"\
	".\gogglesw.h"\
	
NODEP_CPP_CITIE=\
	".\packed"\
	

"$(INTDIR)\cities.obj" : $(SOURCE) $(DEP_CPP_CITIE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TODO.txt

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Goggles.exe.manifest

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\registry.c
DEP_CPP_REGIS=\
	".\gogglesw.h"\
	

"$(INTDIR)\registry.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ini.c
DEP_CPP_INI_C=\
	".\config.h"\
	".\gogglesw.h"\
	

"$(INTDIR)\ini.obj" : $(SOURCE) $(DEP_CPP_INI_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\README.md

!IF  "$(CFG)" == "Goggles - Win32 Release"

!ELSEIF  "$(CFG)" == "Goggles - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
