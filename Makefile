# Manulally written GNU Makefile for CERN ROOT program compilation

# Select compiler command depending on environment
OS:=$(shell uname)
CXX=g++
ifeq ($(OS),Darwin)
	CXX=clang++
endif

# Variables for compiling and linking
# Remove the "" in ROOT's `root-config -cflags`
# TODO: hack - report to ROOT devs to exclude "" from -I"<inc-path>"
CXXFLAGS_ROOT=$(shell root-config --cflags) -fPIC
$(info CXXFLAGS_ROOT=$(CXXFLAGS_ROOT))
FIND:="
REPLACE:= 
CXXFLAGS=$(subst $(FIND),$(REPLACE),$(CXXFLAGS_ROOT))
$(info CXXFLAGS=$(CXXFLAGS))

LDFLAGS=$(shell root-config --ldflags) --verbose
#LDFLAGS=$(shell root-config --ldflags) --verbose
$(info LDFLAGS=$(LDFLAGS))

# Pass SONAMES of the libraries:
# Maybe: readelf -d <lib_file_path> | grep SONAME
LIBS=$(shell root-config --glibs) -lRooFit -lRooFitCore -lHtml -lMinuit -lFumili
$(info LIBS=$(LIBS))

INCDIR=$(shell root-config --incdir)
$(info INCDIR=$(INCDIR))

# Define variables for directories
SRC_DIR=src
OBJ_DIR=build
BIN_DIR=dist

# Set the name of your executable
APP_NAME=sw-calculator

# Set variables with dictionary .cxx and .pcm filenames
DICT_CXX_FILENAME=$(APP_NAME)-dictionary.cxx        # app-dictionary.cxx
DICT_PCM_FILENAME=$(APP_NAME)-dictionary_rdict.pcm  # app-dictionary_rdict.pcm

# Shared library and its debug symbols filenames
SHARED_LIBRARY=$(APP_NAME)-library.so               # app-library.so
SHARED_LIBRARY_DS=$(APP_NAME)-library.so.dSYM       # app-library.so.dSYM

# Binary executable file names with path
EXECUTABLE=$(BIN_DIR)/$(APP_NAME)                   # dist/app
EXECUTABLE_LOCAL=$(BIN_DIR)/$(APP_NAME)-local       # dist/app-local

# Define list of header files (.h), source files (.cpp) and object files (.o)
H_EXT = h
HEADERS = $(shell find $(SRC_DIR) -type f -name *.$(H_EXT))
HEADERS := $(filter-out $(SRC_DIR)/LinkDef.h,$(HEADERS))

SRC_EXT = cpp
SOURCES = $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))

OBJECTS_TEMP = $(SOURCES:.cpp=.o)
OBJECTS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJECTS_TEMP))

# Convenience variable for creating new directories
dir_guard=@mkdir -p $(@D)

# Path for installing the shared library and dictionary .pcm
# Tip: macOS system integrity protection blocks access to $(DYLD_LIBRARY_PATH)
#      so instead we use $(ROOTSYS)/lib
# https://github.com/nteract/nteract/issues/1523#issuecomment-284027093
DYNAMIC_LIBRARY_PATH = $(ROOTSYS)/lib

# Path for installing the executable binary 
# https://stackoverflow.com/questions/39892692/how-to-implement-make-install-in-a-makefile
ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

# Default target builds production executable
all: production

# Add -O3 optimization level for the production release
production: CXXFLAGS+=-O3
production: directories $(DICT_CXX_FILENAME) $(SHARED_LIBRARY) $(OBJECTS) $(EXECUTABLE) move_files

# Add -O3 optimization level for the test release
release: CXXFLAGS+=-O3
release: directories $(DICT_CXX_FILENAME) $(SHARED_LIBRARY) $(OBJECTS) $(EXECUTABLE_LOCAL) move_files

# Also might add flags for debug optimizations: -Og -ggdb -DDEBUG
debug: CXXFLAGS+=-g
debug: directories $(DICT_CXX_FILENAME) $(SHARED_LIBRARY) $(OBJECTS) $(EXECUTABLE_LOCAL) move_files move_debug_symbols

# Target for dictionary generation (creates .cxx and .pcm)
$(DICT_CXX_FILENAME): $(HEADERS) $(SRC_DIR)/LinkDef.h
	rootcling -f $@ $^

# Target for compiling the shared library
# Official ROOT docs are missing $(LIBS) parameter https://root.cern.ch/interacting-shared-libraries-rootcling
$(SHARED_LIBRARY): $(DICT_CXX_FILENAME) $(SOURCES)
	$(CXX) -shared -o $@ $(LDFLAGS) $(CXXFLAGS) $(LIBS) $^
	# $(CXX) -shared -o $@ $(LDFLAGS) $(CXXFLAGS) -I$(INCDIR) $^

# Target for compiling the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(dir_guard)
	@echo "Compiling "$@
# compile with dependency files - why?
#	$(CXX) $(CXXFLAGS) -c -g -MMD -MP -MF "$@.d" -o $@ $<
# compile with debug symbols
#	$(CXX) $(CXXFLAGS) -c -g $< -o $@
# just compile
	$(CXX) -c $(CXXFLAGS) $< -o $@

# Target for linking the production executable
$(EXECUTABLE): $(OBJECTS) $(SHARED_LIBRARY)
	@echo "Linking "$@
	$(CXX) -o $@ $(OBJECTS) $(SHARED_LIBRARY) $(LIBS)

# Target for linking the test release and development executable 
# We assign relative .so search path (./) to the executable 
$(EXECUTABLE_LOCAL):
ifeq ($(OS),Darwin)
	# for macOS just link against the shared library
	$(CXX) -o $@ $(OBJECTS) $(SHARED_LIBRARY) $(LIBS)
	# then change search location of the .so library in the executable - set as same directory
	install_name_tool -change $(SHARED_LIBRARY) @executable_path/$(SHARED_LIBRARY) $(EXECUTABLE_LOCAL)
else
	# for Linux add runtime shared library search path ./ relative to the executable
	# https://stackoverflow.com/questions/38058041/correct-usage-of-rpath-relative-vs-absolute
	$(CXX) -o $@ $(OBJECTS) $(SHARED_LIBRARY) $(LIBS) -Wl,-rpath,'$$ORIGIN'
endif

# Target moves .so and .pcm files to ./dist
move_files:
	# move .so library to ./dist folder
	mv $(SHARED_LIBRARY) $(BIN_DIR)/$(SHARED_LIBRARY)
	# move dictionary .pcm to ./dist folder, remove dictionary .cxx
	mv $(DICT_PCM_FILENAME) $(BIN_DIR)/$(DICT_PCM_FILENAME)
	# remove dictionary .cxx code (don't need it anymore)
	rm $(DICT_CXX_FILENAME)
	
# Target moves .so.dSYM debug symbols to /dist folder
move_debug_symbols:
ifeq ($(OS),Darwin)
	mv $(SHARED_LIBRARY_DS) $(BIN_DIR)/$(SHARED_LIBRARY_DS)
endif

# Target installs the executable and moves shared library to $ROOTSYS/lib
install:
	# run 'make install' or 'sudo -E make install' to preserve user environment 
	sudo install -m 755 $(EXECUTABLE) $(DESTDIR)$(PREFIX)/bin/
	sudo install -m 755 $(BIN_DIR)/$(SHARED_LIBRARY) $(DYNAMIC_LIBRARY_PATH)/
	sudo install -m 755 $(BIN_DIR)/$(DICT_PCM_FILENAME) $(DYNAMIC_LIBRARY_PATH)/

# Target to copy the application icon and create the launcher on Linux
install-linux-launcher:
	xdg-icon-resource install --context apps --size 128 ./resources/$(APP_NAME).png $(APP_NAME)
	xdg-desktop-menu install ./resources/$(APP_NAME).desktop

# Target that cleand the buld
clean:
	rm -f -r $(OBJ_DIR)
	rm -f -r $(BIN_DIR)
	rm -f $(DICT_CXX_FILENAME)
	rm -f $(DICT_PCM_FILENAME)
	rm -f $(SHARED_LIBRARY)
	rm -f -r $(SHARED_LIBRARY_DS)

# Target for creating the ./build and ./dist directories 
directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

# Target for test output
echo:
	$(info SOURCES: $(SOURCES))
	$(info HEADERS: $(HEADERS))

# List of special targets that do not generate files
.PHONY: clean directories move_files move_debug_symbols echo
