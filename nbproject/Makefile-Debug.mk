#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AbstractModelProvider.o \
	${OBJECTDIR}/FileUtils.o \
	${OBJECTDIR}/GraphicsHelper.o \
	${OBJECTDIR}/HistProcessor.o \
	${OBJECTDIR}/LorentzianPdf.o \
	${OBJECTDIR}/ParabolaGaussModelProvider.o \
	${OBJECTDIR}/ParabolaLorentzianModelProvider.o \
	${OBJECTDIR}/ParabolaPdf.o \
	${OBJECTDIR}/SWCalculatorFrame.o \
	${OBJECTDIR}/StringUtils.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pthread -stdlib=libc++ -std=c++11 -m64
CXXFLAGS=-pthread -stdlib=libc++ -std=c++11 -m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/Applications/root_v6.06.02/lib -L. sw-calculator.so

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sw-calculator

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sw-calculator: sw-calculator.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sw-calculator: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sw-calculator ${OBJECTFILES} ${LDLIBSOPTIONS} -lGui -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lpthread -stdlib=libc++ -lm -ldl -lRooFit -lRooFitCore -lHtml -lMinuit -lFumili

${OBJECTDIR}/AbstractModelProvider.o: AbstractModelProvider.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AbstractModelProvider.o AbstractModelProvider.cpp

${OBJECTDIR}/FileUtils.o: FileUtils.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileUtils.o FileUtils.cpp

${OBJECTDIR}/GraphicsHelper.o: GraphicsHelper.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GraphicsHelper.o GraphicsHelper.cpp

${OBJECTDIR}/HistProcessor.o: HistProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/HistProcessor.o HistProcessor.cpp

${OBJECTDIR}/LorentzianPdf.o: LorentzianPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/LorentzianPdf.o LorentzianPdf.cpp

${OBJECTDIR}/ParabolaGaussModelProvider.o: ParabolaGaussModelProvider.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ParabolaGaussModelProvider.o ParabolaGaussModelProvider.cpp

${OBJECTDIR}/ParabolaLorentzianModelProvider.o: ParabolaLorentzianModelProvider.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ParabolaLorentzianModelProvider.o ParabolaLorentzianModelProvider.cpp

${OBJECTDIR}/ParabolaPdf.o: ParabolaPdf.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ParabolaPdf.o ParabolaPdf.cpp

${OBJECTDIR}/SWCalculatorFrame.o: SWCalculatorFrame.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/SWCalculatorFrame.o SWCalculatorFrame.cpp

${OBJECTDIR}/SWCalculatorFrame.h.gch: SWCalculatorFrame.h
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o "$@" SWCalculatorFrame.h

${OBJECTDIR}/SWCalculatorFrameLinkDef.h.gch: SWCalculatorFrameLinkDef.h
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o "$@" SWCalculatorFrameLinkDef.h

${OBJECTDIR}/StringUtils.o: StringUtils.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/StringUtils.o StringUtils.cpp

${OBJECTDIR}/main.o: main.cc
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/Applications/root_v6.06.02/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sw-calculator.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sw-calculator

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
