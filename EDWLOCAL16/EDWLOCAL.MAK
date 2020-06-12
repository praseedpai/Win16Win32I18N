# Microsoft Visual C++ generated build script - Do not modify

PROJ = EDWLOCAL
DEBUG = 0
PROGTYPE = 1
CALLER = C:\edwinlc\edwin.exe
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG 
R_RCDEFINES = /dNDEBUG 
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\PAI\TEMP\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP = SORT.CPP    
RC = rc
CFLAGS_D_WDLL = /nologo /Gs /G2 /Zp1 /Gy /W3 /Z7 /AL /Gx- /YX /Od /D "_DEBUG" /FR /GD /GEf /Fp"CONVERT.PCH"
CFLAGS_R_WDLL = /nologo /f /G2 /Zp1 /W3 /AL /YX /O1 /D "NDEBUG" /FR /GD /GEf /Fp"CONVERT.PCH"
LFLAGS_D_WDLL = /NOLOGO /NOD /NOE /PACKC:61440 /ALIGN:16 /ONERROR:NOEXE /CO /MAP:FULL
LFLAGS_R_WDLL = /NOLOGO /NOD /NOE /PACKC:61440 /ALIGN:16 /ONERROR:NOEXE /MAP:FULL
LIBS_D_WDLL = oldnames libw ldllcew 
LIBS_R_WDLL = oldnames libw ldllcew commdlg.lib 
RCFLAGS = /nologo 
RESFLAGS = /nologo 
RUNFLAGS = 
DEFFILE = EDWLOCAL.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WDLL)
LFLAGS = $(LFLAGS_D_WDLL)
LIBS = $(LIBS_D_WDLL)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WDLL)
LFLAGS = $(LFLAGS_R_WDLL)
LIBS = $(LIBS_R_WDLL)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = SORT.SBR \
		BUFF.SBR \
		MEM.SBR


SORT_DEP = c:\pai\temp\vbsort.h \
	c:\pai\temp\buff.h \
	c:\pai\temp\pm.h


BUFF_DEP = c:\pai\temp\buff.h


MEM_DEP = c:\pai\temp\pm.h


all:	$(PROJ).DLL $(PROJ).BSC

SORT.OBJ:	SORT.CPP $(SORT_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c SORT.CPP

BUFF.OBJ:	BUFF.CPP $(BUFF_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c BUFF.CPP

MEM.OBJ:	MEM.CPP $(MEM_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MEM.CPP


$(PROJ).DLL::	SORT.OBJ BUFF.OBJ MEM.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
SORT.OBJ +
BUFF.OBJ +
MEM.OBJ +
$(OBJS_EXT)
$(PROJ).DLL
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) $@
	implib /nowep $(PROJ).LIB $(PROJ).DLL


run: $(PROJ).DLL
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
