#
# Copyright (c) 2009-2010, Oracle and/or its affiliates. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of Oracle nor the names of its contributors
#   may be used to endorse or promote products derived from this software without
#   specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
#  There exist several targets which are by default empty and which can be 
#  used for execution of your targets. These targets are usually executed 
#  before and after some main targets. They are: 
#
#     .build-pre:              called before 'build' target
#     .build-post:             called after 'build' target
#     .clean-pre:              called before 'clean' target
#     .clean-post:             called after 'clean' target
#     .clobber-pre:            called before 'clobber' target
#     .clobber-post:           called after 'clobber' target
#     .all-pre:                called before 'all' target
#     .all-post:               called after 'all' target
#     .help-pre:               called before 'help' target
#     .help-post:              called after 'help' target
#
#  Targets beginning with '.' are not intended to be called on their own.
#
#  Main targets can be executed directly, and they are:
#  
#     build                    build a specific configuration
#     clean                    remove built files from a configuration
#     clobber                  remove all built files
#     all                      build all configurations
#     help                     print help mesage
#  
#  Targets .build-impl, .clean-impl, .clobber-impl, .all-impl, and
#  .help-impl are implemented in nbproject/makefile-impl.mk.
#
#  Available make variables:
#
#     CND_BASEDIR                base directory for relative paths
#     CND_DISTDIR                default top distribution directory (build artifacts)
#     CND_BUILDDIR               default top build directory (object files, ...)
#     CONF                       name of current configuration
#     CND_PLATFORM_${CONF}       platform name (current configuration)
#     CND_ARTIFACT_DIR_${CONF}   directory of build artifact (current configuration)
#     CND_ARTIFACT_NAME_${CONF}  name of build artifact (current configuration)
#     CND_ARTIFACT_PATH_${CONF}  path to build artifact (current configuration)
#     CND_PACKAGE_DIR_${CONF}    directory of package (current configuration)
#     CND_PACKAGE_NAME_${CONF}   name of package (current configuration)
#     CND_PACKAGE_PATH_${CONF}   path to package (current configuration)
#
# NOCDDL


# Environment 
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib

# My variables
MYCFLAGS=-pthread -stdlib=libc++ -std=c++11 -m64 -I/Applications/root_v6.06.02/include
MYLDFLAGS=-m64
MYGLIBS=-L/Applications/root_v6.06.02/lib -lGui -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lpthread -stdlib=libc++ -lm -ldl
MYLIBS=-L/Applications/root_v6.06.02/lib -lGui -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lpthread -stdlib=libc++ -lm -ldl -lRooFit -lRooFitCore -lHtml -lMinuit -lFumili
MYROOTSYS=/Applications/root_v6.06.02
MYHEADERS=model/Constants.h \
          model/Model.h \
          roofit/AbstractModelProvider.h \
          roofit/ChannelConvolutionPdf.h \
          roofit/CompositeModelProvider.h \
          roofit/DampLorentzPdf.h \
	  roofit/GaussianPdf.h \
          roofit/IndirectParamPdf.h \
          roofit/LorentzianPdf.h \
          roofit/OrthogonalPdf.h \
          roofit/ParabolaGaussModelProvider.h \
          roofit/ParabolaLorentzianModelProvider.h \
          roofit/ParabolaPdf.h \
          util/FileUtils.h \
          util/GraphicsHelper.h \
          util/HistProcessor.h \
          util/RootHelper.h \
          util/StringUtils.h \
          util/Variable.h \
          widgets/importSpectrumWidget/IImportSpectrumView.h \
          widgets/importSpectrumWidget/ImportSpectrumPresenter.h \
          widgets/importSpectrumWidget/ImportSpectrumView.h \
          widgets/swCalculatorWidget/SWCalculatorFrame.h \
          main.h
MYSOURCES=model/Constants.cpp \
          model/Model.cpp \
          roofit/AbstractModelProvider.cpp \
          roofit/ChannelConvolutionPdf.cpp \
          roofit/CompositeModelProvider.cpp \
          roofit/DampLorentzPdf.cpp \
	  roofit/GaussianPdf.cpp \
          roofit/IndirectParamPdf.cpp \
          roofit/LorentzianPdf.cpp \
          roofit/OrthogonalPdf.cpp \
          roofit/ParabolaGaussModelProvider.cpp \
          roofit/ParabolaLorentzianModelProvider.cpp \
          roofit/ParabolaPdf.cpp \
          util/FileUtils.cpp \
          util/GraphicsHelper.cpp \
          util/HistProcessor.cpp \
          util/RootHelper.cpp \
          util/StringUtils.cpp \
          util/Variable.cpp \
          widgets/importSpectrumWidget/ImportSpectrumPresenter.cpp \
          widgets/importSpectrumWidget/ImportSpectrumView.cpp \
          widgets/swCalculatorWidget/SWCalculatorFrame.cpp \
          main.cc

# build
build: .build-post

.build-pre:
# Add your pre 'build' code here...
	@echo Generating dictionary: SWCalculatorFrameDict.cpp
	cd src; \
	rootcling -f SWCalculatorFrameDict.cpp -c $(MYCFLAGS) -p $(MYHEADERS) SWCalculatorFrameLinkDef.h
#https://root.cern.ch/interacting-shared-libraries-rootcint
	@echo Generating shared library: sw-calculator.so
	cd src; \
	g++ -shared -o ../lib/sw-calculator.so $(MYCFLAGS) $(MYLIBS) SWCalculatorFrameDict.cpp $(MYSOURCES)

.build-post: .build-impl
# Add your post 'build' code here...
	@echo Copying shared library 'sw-calculator.so' to 
	$(CP) ./lib/sw-calculator.so ./dist/Debug/GNU-MacOSX/sw-calculator.so

#sw-calculator.so:
#	@echo Generating shared library: sw-calculator.so
#	cd src; \
#	g++ -shared -o ../lib/sw-calculator.so $(MYCFLAGS) $(MYLIBS) SWCalculatorFrameDict.cpp $(MYSOURCES)
	
# clean
clean: .clean-post

.clean-pre:
# Add your pre 'clean' code here...

.clean-post: .clean-impl
# Add your post 'clean' code here...


# clobber
clobber: .clobber-post

.clobber-pre:
# Add your pre 'clobber' code here...

.clobber-post: .clobber-impl
# Add your post 'clobber' code here...


# all
all: .all-post

.all-pre:
# Add your pre 'all' code here...

.all-post: .all-impl
# Add your post 'all' code here...


# help
help: .help-post

.help-pre:
# Add your pre 'help' code here...

.help-post: .help-impl
# Add your post 'help' code here...



# include project implementation makefile
include nbproject/Makefile-impl.mk

# include project make variables
include nbproject/Makefile-variables.mk
