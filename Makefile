#
# Makefile to create Java JNI library.
# On Windows, uses Cygwin b20.1 tools with Mingw runtime. 
# 
# Things you may need to change, or redefine on the command line:
#   BUDDY_SRC    -- location of BuDDy source code
#   CUDD_SRC     -- location of CUDD source code
#   CAL_SRC      -- location of CAL source code
#   JDK_ROOT     -- location where you installed JDK
#

BUDDY_SRC = buddy22/src
CUDD_SRC = cudd-2.4.0
CAL_SRC = cal-2.1

ifeq (${OS},Windows_NT)
  JDK_ROOT = $(firstword $(wildcard c:/j2sdk*))
  CLASSPATH = .\;jdd.jar
  CC = gcc
  CFLAGS = -Wall -O3 -mno-cygwin
  OBJECT_OUTPUT_OPTION = -o$(space)
  LINK = dllwrap
  LINKFLAGS = -s -mno-cygwin -mwindows --target=i386-mingw32 \
              --add-stdcall-alias --driver-name gcc
  DLL_OUTPUT_OPTION = -o$(space)
  INCLUDES = -I. -I$(JDK_ROOT)/include \
             -I$(BUDDY_SRC) \
             -I$(CUDD_SRC)/cudd -I$(CUDD_SRC)/epd -I$(CUDD_SRC)/mtr \
             -I$(CUDD_SRC)/st -I$(CUDD_SRC)/util \
             -I$(CAL_SRC) \
             -I$(JDK_ROOT)/include/win32
  BUDDY_DLL_NAME = buddy.dll
  CUDD_DLL_NAME = cudd.dll
  CAL_DLL_NAME = cal.dll
  ifeq (${CC},icl)    # Intel Windows compiler
    CFLAGS = -O3 -QaxW
    OBJECT_OUTPUT_OPTION = -Fo
    LINK = xilink
    LINKFLAGS = /dll /libpath:$(JDK_ROOT)/lib user32.lib gdi32.lib
    DLL_OUTPUT_OPTION = /out:
  endif
  ifeq (${CC},cl)     # Microsoft Visual C++ compiler
    CFLAGS = -O2
    OBJECT_OUTPUT_OPTION = -Fo
    LINK = cl
    LINKFLAGS = -MLd -LDd -Zi /link /libpath:$(JDK_ROOT)/lib user32.lib gdi32.lib
    DLL_OUTPUT_OPTION = -Fe
  endif
else
  JDK_ROOT = $(firstword $(wildcard /usr/java/j2sdk*))
  CLASSPATH = .:jdd.jar
  CFLAGS = -D_REENTRANT -D_GNU_SOURCE -O3
  CAL_CFLAGS = -DTEST -O3 -DCLOCK_RESOLUTION=60 -DRLIMIT_DATA_DEFAULT=16777216 -DNDEBUG=1 -DSTDC_HEADERS=1 -DHAVE_SYS_WAIT_H=1 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_ERRNO_H=1 -DHAVE_ASSERT_H=1 -DHAVE_SYS_WAIT_H=1 -DHAVE_PWD_H=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_TIMES_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_STDARG_H=1 -DHAVE_VARARGS_H=1 -DSIZEOF_VOID_P=4 -DSIZEOF_INT=4 -DHAVE_IEEE_754=1 -DPAGE_SIZE=4096 -DLG_PAGE_SIZE=12 -DRETSIGTYPE=void -DHAVE_STRCOLL=1 -DHAVE_SYSCONF=1 -DHAVE_GETHOSTNAME=1 -DHAVE_STRCSPN=1 -DHAVE_STRERROR=1 -DHAVE_STRSPN=1 -DHAVE_STRSTR=1 -DHAVE_GETENV=1 -DHAVE_STRCHR=1 -DHAVE_GETRLIMIT=1 -DHAVE_GETRUSAGE=1 -DHAVE_VALLOC=1
  OBJECT_OUTPUT_OPTION = -o$(space)
  LINK = $(CC)
  LINKFLAGS = -shared
  DLL_OUTPUT_OPTION = -o$(space)
  INCLUDES = -I. -I$(JDK_ROOT)/include \
             -I$(BUDDY_SRC) \
             -I$(CUDD_SRC)/cudd -I$(CUDD_SRC)/epd -I$(CUDD_SRC)/mtr \
             -I$(CUDD_SRC)/st -I$(CUDD_SRC)/util \
             -I$(CAL_SRC) \
             -I$(JDK_ROOT)/include/linux
  BUDDY_DLL_NAME = libbuddy.so
  CUDD_DLL_NAME = libcudd.so
  CAL_DLL_NAME = libcal.so
  ifeq (${CC},icc)    # Intel Linux compiler
    LINKFLAGS = -shared -static-libcxa
  endif
endif

# The java tools:
JAVAC = $(JDK_ROOT)/bin/javac
JAVA = $(JDK_ROOT)/bin/java
JAVAH = $(JDK_ROOT)/bin/javah
JAVADOC = $(JDK_ROOT)/bin/javadoc
JAR = $(JDK_ROOT)/bin/jar

# The java source code
JAVA_SOURCES = org/sf/javabdd/BDD.java \
	org/sf/javabdd/BDDBitVector.java \
	org/sf/javabdd/BDDDomain.java \
	org/sf/javabdd/BDDException.java \
	org/sf/javabdd/BDDFactory.java \
	org/sf/javabdd/BDDPairing.java \
	org/sf/javabdd/BuDDyFactory.java \
	org/sf/javabdd/CALFactory.java \
	org/sf/javabdd/CUDDFactory.java \
	org/sf/javabdd/FindBestOrder.java \
	org/sf/javabdd/JavaFactory.java \
	org/sf/javabdd/JDDFactory.java \
	org/sf/javabdd/TestBDDFactory.java \
	org/sf/javabdd/TypedBDDFactory.java
JAVA_CLASSFILES = org/sf/javabdd/*.class
JAVA_PACKAGES = org.sf.javabdd
BUDDY_CLASSFILE = org/sf/javabdd/BuDDyFactory.class
CUDD_CLASSFILE = org/sf/javabdd/CUDDFactory.class
CAL_CLASSFILE = org/sf/javabdd/CALFactory.class
BUDDY_CLASSNAMES = org.sf.javabdd.BuDDyFactory \
	org.sf.javabdd.BuDDyFactory\$$BuDDyBDD \
	org.sf.javabdd.BuDDyFactory\$$BuDDyBDDDomain \
	org.sf.javabdd.BuDDyFactory\$$BuDDyBDDPairing
CUDD_CLASSNAMES = org.sf.javabdd.CUDDFactory \
	org.sf.javabdd.CUDDFactory\$$CUDDBDD \
	org.sf.javabdd.CUDDFactory\$$CUDDBDDDomain \
	org.sf.javabdd.CUDDFactory\$$CUDDBDDPairing
CAL_CLASSNAMES = org.sf.javabdd.CALFactory \
	org.sf.javabdd.CALFactory\$$CALBDD \
	org.sf.javabdd.CALFactory\$$CALBDDDomain \
	org.sf.javabdd.CALFactory\$$CALBDDPairing
EXAMPLE_SOURCES = NQueens.java
EXAMPLE_CLASSFILES = $(EXAMPLE_SOURCES:%.java=%.class)
JAR_NAME = javabdd.jar

BUDDY_INCLUDE = buddy_jni.h
BUDDY_SRCS = buddy_jni.c \
	$(BUDDY_SRC)/bddio.c $(BUDDY_SRC)/bddop.c $(BUDDY_SRC)/bvec.c \
	$(BUDDY_SRC)/cache.c $(BUDDY_SRC)/fdd.c $(BUDDY_SRC)/imatrix.c \
	$(BUDDY_SRC)/kernel.c $(BUDDY_SRC)/pairs.c $(BUDDY_SRC)/prime.c \
	$(BUDDY_SRC)/reorder.c $(BUDDY_SRC)/tree.c
BUDDY_OBJS = $(BUDDY_SRCS:.c=.o)

CUDD_INCLUDE = cudd_jni.h
CUDD_SRCS = cudd_jni.c \
	  $(CUDD_SRC)/cudd/cuddAPI.c $(CUDD_SRC)/cudd/cuddAddAbs.c $(CUDD_SRC)/cudd/cuddAddApply.c $(CUDD_SRC)/cudd/cuddAddFind.c $(CUDD_SRC)/cudd/cuddAddIte.c \
	  $(CUDD_SRC)/cudd/cuddAddInv.c $(CUDD_SRC)/cudd/cuddAddNeg.c $(CUDD_SRC)/cudd/cuddAddWalsh.c $(CUDD_SRC)/cudd/cuddAndAbs.c \
	  $(CUDD_SRC)/cudd/cuddAnneal.c $(CUDD_SRC)/cudd/cuddApa.c $(CUDD_SRC)/cudd/cuddApprox.c $(CUDD_SRC)/cudd/cuddBddAbs.c $(CUDD_SRC)/cudd/cuddBddCorr.c \
	  $(CUDD_SRC)/cudd/cuddBddIte.c $(CUDD_SRC)/cudd/cuddBridge.c $(CUDD_SRC)/cudd/cuddCache.c $(CUDD_SRC)/cudd/cuddCheck.c $(CUDD_SRC)/cudd/cuddClip.c \
	  $(CUDD_SRC)/cudd/cuddCof.c $(CUDD_SRC)/cudd/cuddCompose.c $(CUDD_SRC)/cudd/cuddDecomp.c $(CUDD_SRC)/cudd/cuddEssent.c \
	  $(CUDD_SRC)/cudd/cuddExact.c $(CUDD_SRC)/cudd/cuddExport.c $(CUDD_SRC)/cudd/cuddGenCof.c $(CUDD_SRC)/cudd/cuddGenetic.c \
	  $(CUDD_SRC)/cudd/cuddGroup.c $(CUDD_SRC)/cudd/cuddHarwell.c $(CUDD_SRC)/cudd/cuddInit.c $(CUDD_SRC)/cudd/cuddInteract.c \
	  $(CUDD_SRC)/cudd/cuddLCache.c $(CUDD_SRC)/cudd/cuddLevelQ.c \
	  $(CUDD_SRC)/cudd/cuddLinear.c $(CUDD_SRC)/cudd/cuddLiteral.c $(CUDD_SRC)/cudd/cuddMatMult.c $(CUDD_SRC)/cudd/cuddPriority.c \
	  $(CUDD_SRC)/cudd/cuddRead.c $(CUDD_SRC)/cudd/cuddRef.c $(CUDD_SRC)/cudd/cuddReorder.c $(CUDD_SRC)/cudd/cuddSat.c $(CUDD_SRC)/cudd/cuddSign.c \
	  $(CUDD_SRC)/cudd/cuddSolve.c $(CUDD_SRC)/cudd/cuddSplit.c $(CUDD_SRC)/cudd/cuddSubsetHB.c $(CUDD_SRC)/cudd/cuddSubsetSP.c $(CUDD_SRC)/cudd/cuddSymmetry.c \
	  $(CUDD_SRC)/cudd/cuddTable.c $(CUDD_SRC)/cudd/cuddUtil.c $(CUDD_SRC)/cudd/cuddWindow.c $(CUDD_SRC)/cudd/cuddZddCount.c $(CUDD_SRC)/cudd/cuddZddFuncs.c \
	  $(CUDD_SRC)/cudd/cuddZddGroup.c $(CUDD_SRC)/cudd/cuddZddIsop.c $(CUDD_SRC)/cudd/cuddZddLin.c $(CUDD_SRC)/cudd/cuddZddMisc.c \
	  $(CUDD_SRC)/cudd/cuddZddPort.c $(CUDD_SRC)/cudd/cuddZddReord.c $(CUDD_SRC)/cudd/cuddZddSetop.c $(CUDD_SRC)/cudd/cuddZddSymm.c \
	  $(CUDD_SRC)/cudd/cuddZddUtil.c \
	  $(CUDD_SRC)/epd/epd.c \
	  $(CUDD_SRC)/mtr/mtrBasic.c $(CUDD_SRC)/mtr/mtrGroup.c \
	  $(CUDD_SRC)/st/st.c \
	  $(CUDD_SRC)/util/cpu_time.c $(CUDD_SRC)/util/datalimit.c $(CUDD_SRC)/util/safe_mem.c
CUDD_OBJS = $(CUDD_SRCS:.c=.o)

CAL_INCLUDE = cal_jni.h
CAL_SRCS = cal_jni.c \
	  $(CAL_SRC)/cal.c $(CAL_SRC)/calApplyReduce.c $(CAL_SRC)/calAssociation.c $(CAL_SRC)/calBddCompose.c $(CAL_SRC)/calBddITE.c \
	  $(CAL_SRC)/calBddManager.c $(CAL_SRC)/calBddOp.c $(CAL_SRC)/calBddSatisfy.c $(CAL_SRC)/calBddSize.c \
	  $(CAL_SRC)/calBddSubstitute.c $(CAL_SRC)/calBddSupport.c $(CAL_SRC)/calBddSwapVars.c $(CAL_SRC)/calBddVarSubstitute.c \
	  $(CAL_SRC)/calBlk.c $(CAL_SRC)/calCacheTableTwo.c $(CAL_SRC)/calDump.c $(CAL_SRC)/calGC.c $(CAL_SRC)/calHashTable.c \
	  $(CAL_SRC)/calHashTableOne.c $(CAL_SRC)/calHashTableThree.c $(CAL_SRC)/calInteract.c $(CAL_SRC)/calMem.c \
	  $(CAL_SRC)/calMemoryManagement.c $(CAL_SRC)/calPipeline.c $(CAL_SRC)/calPrint.c \
	  $(CAL_SRC)/calPrintProfile.c $(CAL_SRC)/calQuant.c $(CAL_SRC)/calReduce.c $(CAL_SRC)/calReorderBF.c \
	  $(CAL_SRC)/calReorderDF.c $(CAL_SRC)/calReorderUtil.c \
	  $(CAL_SRC)/calTerminal.c $(CAL_SRC)/calUtil.c
CAL_OBJS = $(CAL_SRCS:.c=.o)

default: jar $(BUDDY_DLL_NAME)

all: $(BUDDY_DLL_NAME) $(CUDD_DLL_NAME) $(CAL_DLL_NAME)

dll: $(DLL_NAME)

$(BUDDY_DLL_NAME): $(BUDDY_OBJS)
	$(LINK) $(DLL_OUTPUT_OPTION)$@ $(BUDDY_OBJS) $(LINKFLAGS)
#	$(LINK) $(INCLUDES) $(CFLAGS) $(DLL_OUTPUT_OPTION)$@ $(DLL_SRCS) -MLd -LDd -Zi /link /libpath:$(JDK_ROOT)/lib 

$(CUDD_DLL_NAME): $(CUDD_OBJS)
	$(LINK) $(DLL_OUTPUT_OPTION)$@ $(CUDD_OBJS) $(LINKFLAGS)

$(CAL_DLL_NAME): $(CAL_OBJS)
	$(LINK) $(DLL_OUTPUT_OPTION)$@ $(CAL_OBJS) $(LINKFLAGS)

buddy_jni.o: buddy_jni.c $(BUDDY_INCLUDE)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(OBJECT_OUTPUT_OPTION)$@ $<

cudd_jni.o: cudd_jni.c $(CUDD_INCLUDE)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(OBJECT_OUTPUT_OPTION)$@ $<

cal_jni.o: cal_jni.c $(CAL_INCLUDE)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(OBJECT_OUTPUT_OPTION)$@ $<

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $(OBJECT_OUTPUT_OPTION)$@ $<

$(BUDDY_INCLUDE): $(BUDDY_CLASSFILE)
	$(JAVAH) -jni -o $(BUDDY_INCLUDE) $(BUDDY_CLASSNAMES)

$(CUDD_INCLUDE): $(CUDD_CLASSFILE)
	$(JAVAH) -jni -o $(CUDD_INCLUDE) $(CUDD_CLASSNAMES)

$(CAL_INCLUDE): $(CAL_CLASSFILE)
	$(JAVAH) -jni -o $(CAL_INCLUDE) $(CAL_CLASSNAMES)

$(BUDDY_CLASSFILE): $(JAVA_SOURCES)
	$(JAVAC) -classpath $(CLASSPATH) $(JAVA_SOURCES)

$(CUDD_CLASSFILE): $(JAVA_SOURCES)
	$(JAVAC) -classpath $(CLASSPATH) $(JAVA_SOURCES)

$(CAL_CLASSFILE): $(JAVA_SOURCES)
	$(JAVAC) -classpath $(CLASSPATH) $(JAVA_SOURCES)

$(EXAMPLE_CLASSFILES): $(EXAMPLE_SOURCES)
	$(JAVAC) -classpath $(CLASSPATH) $(EXAMPLE_SOURCES)

examples: $(EXAMPLE_CLASSFILES)

javadoc: $(JAVA_SOURCES)
	$(JAVADOC) -d javadoc -breakiterator $(JAVA_PACKAGES)

jar: $(JAR_NAME)

$(JAR_NAME): $(BUDDY_CLASSFILE) $(CUDD_CLASSFILE) $(CAL_CLASSFILE) $(EXAMPLE_CLASSFILES)
	$(JAR) cfm $(JAR_NAME) javabddManifest $(JAVA_CLASSFILES) $(EXAMPLE_CLASSFILES)

pdo:
	icl -Qprof_gen $(INCLUDES) $(CFLAGS) $(DLL_OUTPUT_OPTION)buddy.dll $(DLL_SRCS) -LD /link /libpath:$(JDK_ROOT)/lib 
	$(JAVA) NQueens 12
	icl -Qprof_use $(INCLUDES) $(CFLAGS) $(DLL_OUTPUT_OPTION)buddy.dll $(DLL_SRCS) -LD /link /libpath:$(JDK_ROOT)/lib 
	$(JAVA) NQueens 12

opt_report:
	icl -Qopt_report -Qopt_report_phase all $(INCLUDES) $(CFLAGS) $(DLL_OUTPUT_OPTION)buddy.dll $(DLL_SRCS) -LD /link /libpath:$(JDK_ROOT)/lib 

test:	$(DLL_NAME) $(EXAMPLE_CLASSFILES)
	$(JAVA) NQueens 8

clean:
	$(RM) -f $(JAVA_CLASSFILES) $(BUDDY_INCLUDE) $(CUDD_INCLUDE) $(CAL_INCLUDE) $(BUDDY_OBJS) $(CAL_OBJS) $(CUDD_OBJS) $(DLL_NAME) $(EXAMPLE_CLASSFILES) $(JAR_NAME)
	$(RM) -rf javadoc

empty := 
space := $(empty) $(empty)
