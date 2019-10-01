# Select compiler command depending on environment
OS:=$(shell uname)
CXX=g++
ifeq ($(OS),Darwin)
  CXX=clang++
endif

# Path for linking dynamic libraries
# macOS system integrity protection blocks access to $(DYLD_LIBRARY_PATH)
# https://github.com/nteract/nteract/issues/1523#issuecomment-284027093
DYNAMIC_LIBRARY_PATH = $(ROOTSYS)/lib

# Define variables for directories
SRC_DIR=src
OBJ_DIR=build
BIN_DIR=dist

# Replace with your application name
APP_NAME=sw-calculator

DICT_NAME=$(APP_NAME)-dictionary
DICT_FILENAME=$(DICT_NAME).cxx             # app-dictionary.cxx
DICT_PCM_FILENAME=$(DICT_NAME)_rdict.pcm   # app-dictionary_rdict.pcm

# Compiler flags, library search paths and ROOT shared libraries names
CXXFLAGS=`root-config --cflags` -fPIC
LDFLAGS=`root-config --ldflags`
GLIBS=`root-config --glibs` -lRooFit -lRooFitCore -lHtml -lMinuit -lFumili

# Build lists of header, source and object files
H_EXT = h
HEADERS = $(shell find $(SRC_DIR) -type f -name *.$(H_EXT))
HEADERS := $(filter-out $(SRC_DIR)/LinkDef.h,$(HEADERS))

SRC_EXT = cpp
SOURCES = $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))

OBJECTS_TEMP = $(SOURCES:.cpp=.o)
OBJECTS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJECTS_TEMP))

# Executable and shared library files path and name
EXECUTABLE=$(BIN_DIR)/$(APP_NAME)       # src/app
SHARED_LIBRARY=$(APP_NAME)-library.so           # app-library.so
SHARED_LIBRARY_DS=$(APP_NAME)-library.so.dSYM   # .so debug symbols (generated on macOS)

# convenience variable for making directories
dir_guard=@mkdir -p $(@D)

# for 'install' target PREFIX is environment variable, but if it is not set, then set default value
# https://stackoverflow.com/questions/39892692/how-to-implement-make-install-in-a-makefile
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

# Empty target ensures that list of all 'end products' are called
all: release

# Add -O3 optimization level for the release
release: CXXFLAGS+=-O3
release: executable move_files

# Also might add flags for debug optimizations: -Og -ggdb -DDEBUG
debug: CXXFLAGS+=-g
debug: executable move_files move_debug_symbols

executable: directories $(DICT_FILENAME) $(SHARED_LIBRARY) $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(SHARED_LIBRARY)
	@echo "Linking "$@
	$(CXX) -o $@ $(OBJECTS) $(SHARED_LIBRARY) $(GLIBS)
#ifeq ($(OS),Darwin)
	# for macOS just link against the shared library
	# $(CXX) -o $@ $(OBJECTS) $(SHARED_LIBRARY) $(GLIBS)
	# then change search location of the .so library in the executable - set as same directory (macOS only)
	# install_name_tool -change $(SHARED_LIBRARY) @executable_path/$(SHARED_LIBRARY) $(EXECUTABLE)
#else
	# for Linux add runtime shared library search path ./ relative to the executable (gcc only)
	# https://stackoverflow.com/questions/38058041/correct-usage-of-rpath-relative-vs-absolute
	# $(CXX) -o $@ $(OBJECTS) $(SHARED_LIBRARY) $(GLIBS) -Wl,-rpath,'$$ORIGIN'
#endif

$(DICT_FILENAME): $(HEADERS) $(SRC_DIR)/LinkDef.h
	rootcling -f $@ -c $(CXXFLAGS) -p $^

# https://root.cern.ch/interacting-shared-libraries-rootcling (they forgot $(GLIBS) damn)
$(SHARED_LIBRARY): $(DICT_FILENAME) $(SOURCES)
	$(CXX) -shared -o $@ $(LDFLAGS) $(CXXFLAGS) $(GLIBS) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(dir_guard)
	@echo "Compiling "$@
# compile with dependency files - why?
#	$(CXX) $(CXXFLAGS) -c -g -MMD -MP -MF "$@.d" -o $@ $<
# compile with debug symbols
#	$(CXX) $(CXXFLAGS) -c -g $< -o $@
# just compile
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f -r $(OBJ_DIR)
	rm -f -r $(BIN_DIR)
	rm -f $(DICT_FILENAME)
	rm -f $(DICT_PCM_FILENAME)
	rm -f $(SHARED_LIBRARY)
	rm -f -r $(SHARED_LIBRARY_DS)

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

echo:
	$(info SOURCES: $(SOURCES))
	$(info HEADERS: $(HEADERS))

move_files:
	# move .so library to /dist folder
	mv $(SHARED_LIBRARY) $(BIN_DIR)/$(SHARED_LIBRARY)

	# move dictionary .pcm to /dist folder, remove dictionary .cxx
	mv $(DICT_PCM_FILENAME) $(BIN_DIR)/$(DICT_PCM_FILENAME)
	rm $(DICT_FILENAME)
	# copy icon
	# cp resources/$(APP_NAME).xpm $(BIN_DIR)/$(APP_NAME).xpm
	
move_debug_symbols:
ifeq ($(OS),Darwin)
	# move .so.dSYM debub symbols to /dist folder
	mv $(SHARED_LIBRARY_DS) $(BIN_DIR)/$(SHARED_LIBRARY_DS)
endif

install:
	sudo install -m 755 $(EXECUTABLE) $(DESTDIR)$(PREFIX)/bin/
	sudo install -m 755 $(BIN_DIR)/$(SHARED_LIBRARY) $(DYNAMIC_LIBRARY_PATH)/
	sudo install -m 755 $(BIN_DIR)/$(DICT_PCM_FILENAME) $(DYNAMIC_LIBRARY_PATH)/

# List of special targets that do not generate files
.PHONY: clean directories move_files move_debug_symbols echo
