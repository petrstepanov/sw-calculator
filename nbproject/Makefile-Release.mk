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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/model/Constants.o \
	${OBJECTDIR}/src/model/Model.o \
	${OBJECTDIR}/src/roofit/AbstractModelProvider.o \
	${OBJECTDIR}/src/roofit/ChannelConvolutionPdf.o \
	${OBJECTDIR}/src/roofit/CompositeModelProvider.o \
	${OBJECTDIR}/src/roofit/DampLorentzPdf.o \
	${OBJECTDIR}/src/roofit/GaussianPdf.o \
	${OBJECTDIR}/src/roofit/IndirectParamPdf.o \
	${OBJECTDIR}/src/roofit/LorentzianPdf.o \
	${OBJECTDIR}/src/roofit/OrthogonalPdf.o \
	${OBJECTDIR}/src/roofit/ParabolaGaussModelProvider.o \
	${OBJECTDIR}/src/roofit/ParabolaLorentzianModelProvider.o \
	${OBJECTDIR}/src/roofit/ParabolaPdf.o \
	${OBJECTDIR}/src/util/FileUtils.o \
	${OBJECTDIR}/src/util/GraphicsHelper.o \
	${OBJECTDIR}/src/util/HistProcessor.o \
	${OBJECTDIR}/src/util/RootHelper.o \
	${OBJECTDIR}/src/util/StringUtils.o \
	${OBJECTDIR}/src/util/Variable.o \
	${OBJECTDIR}/src/widgets/importSpectrumWidget/ImportSpectrumPresenter.o \
	${OBJECTDIR}/src/widgets/importSpectrumWidget/ImportSpectrumView.o \
	${OBJECTDIR}/src/widgets/swCalculatorWidget/SWCalculatorFrame.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sw-calculator

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sw-calculator: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sw-calculator ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/main.o: src/main.cc
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cc

${OBJECTDIR}/src/model/Constants.o: src/model/Constants.cpp
	${MKDIR} -p ${OBJECTDIR}/src/model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/model/Constants.o src/model/Constants.cpp

${OBJECTDIR}/src/model/Model.o: src/model/Model.cpp
	${MKDIR} -p ${OBJECTDIR}/src/model
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/model/Model.o src/model/Model.cpp

${OBJECTDIR}/src/roofit/AbstractModelProvider.o: src/roofit/AbstractModelProvider.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/AbstractModelProvider.o src/roofit/AbstractModelProvider.cpp

${OBJECTDIR}/src/roofit/ChannelConvolutionPdf.o: src/roofit/ChannelConvolutionPdf.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/ChannelConvolutionPdf.o src/roofit/ChannelConvolutionPdf.cpp

${OBJECTDIR}/src/roofit/CompositeModelProvider.o: src/roofit/CompositeModelProvider.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/CompositeModelProvider.o src/roofit/CompositeModelProvider.cpp

${OBJECTDIR}/src/roofit/DampLorentzPdf.o: src/roofit/DampLorentzPdf.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/DampLorentzPdf.o src/roofit/DampLorentzPdf.cpp

${OBJECTDIR}/src/roofit/GaussianPdf.o: src/roofit/GaussianPdf.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/GaussianPdf.o src/roofit/GaussianPdf.cpp

${OBJECTDIR}/src/roofit/IndirectParamPdf.o: src/roofit/IndirectParamPdf.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/IndirectParamPdf.o src/roofit/IndirectParamPdf.cpp

${OBJECTDIR}/src/roofit/LorentzianPdf.o: src/roofit/LorentzianPdf.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/LorentzianPdf.o src/roofit/LorentzianPdf.cpp

${OBJECTDIR}/src/roofit/OrthogonalPdf.o: src/roofit/OrthogonalPdf.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/OrthogonalPdf.o src/roofit/OrthogonalPdf.cpp

${OBJECTDIR}/src/roofit/ParabolaGaussModelProvider.o: src/roofit/ParabolaGaussModelProvider.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/ParabolaGaussModelProvider.o src/roofit/ParabolaGaussModelProvider.cpp

${OBJECTDIR}/src/roofit/ParabolaLorentzianModelProvider.o: src/roofit/ParabolaLorentzianModelProvider.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/ParabolaLorentzianModelProvider.o src/roofit/ParabolaLorentzianModelProvider.cpp

${OBJECTDIR}/src/roofit/ParabolaPdf.o: src/roofit/ParabolaPdf.cpp
	${MKDIR} -p ${OBJECTDIR}/src/roofit
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/roofit/ParabolaPdf.o src/roofit/ParabolaPdf.cpp

${OBJECTDIR}/src/util/FileUtils.o: src/util/FileUtils.cpp
	${MKDIR} -p ${OBJECTDIR}/src/util
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/util/FileUtils.o src/util/FileUtils.cpp

${OBJECTDIR}/src/util/GraphicsHelper.o: src/util/GraphicsHelper.cpp
	${MKDIR} -p ${OBJECTDIR}/src/util
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/util/GraphicsHelper.o src/util/GraphicsHelper.cpp

${OBJECTDIR}/src/util/HistProcessor.o: src/util/HistProcessor.cpp
	${MKDIR} -p ${OBJECTDIR}/src/util
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/util/HistProcessor.o src/util/HistProcessor.cpp

${OBJECTDIR}/src/util/RootHelper.o: src/util/RootHelper.cpp
	${MKDIR} -p ${OBJECTDIR}/src/util
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/util/RootHelper.o src/util/RootHelper.cpp

${OBJECTDIR}/src/util/StringUtils.o: src/util/StringUtils.cpp
	${MKDIR} -p ${OBJECTDIR}/src/util
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/util/StringUtils.o src/util/StringUtils.cpp

${OBJECTDIR}/src/util/Variable.o: src/util/Variable.cpp
	${MKDIR} -p ${OBJECTDIR}/src/util
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/util/Variable.o src/util/Variable.cpp

${OBJECTDIR}/src/widgets/importSpectrumWidget/ImportSpectrumPresenter.o: src/widgets/importSpectrumWidget/ImportSpectrumPresenter.cpp
	${MKDIR} -p ${OBJECTDIR}/src/widgets/importSpectrumWidget
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/widgets/importSpectrumWidget/ImportSpectrumPresenter.o src/widgets/importSpectrumWidget/ImportSpectrumPresenter.cpp

${OBJECTDIR}/src/widgets/importSpectrumWidget/ImportSpectrumView.o: src/widgets/importSpectrumWidget/ImportSpectrumView.cpp
	${MKDIR} -p ${OBJECTDIR}/src/widgets/importSpectrumWidget
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/widgets/importSpectrumWidget/ImportSpectrumView.o src/widgets/importSpectrumWidget/ImportSpectrumView.cpp

${OBJECTDIR}/src/widgets/swCalculatorWidget/SWCalculatorFrame.o: src/widgets/swCalculatorWidget/SWCalculatorFrame.cpp
	${MKDIR} -p ${OBJECTDIR}/src/widgets/swCalculatorWidget
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/widgets/swCalculatorWidget/SWCalculatorFrame.o src/widgets/swCalculatorWidget/SWCalculatorFrame.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
