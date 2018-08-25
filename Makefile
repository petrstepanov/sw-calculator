# Environment
OS:=$(shell uname)
ifeq ($(OS),Darwin)
  CXX=clang++
else
  CXX=g++
endif

SRC_DIR=src
OBJ_DIR=build
BIN_DIR=dist

APP_NAME=sw-calculator
DICT_FILENAME=sw-dict.cpp
DICT_PCM_FILENAME=sw-dict_rdict.pcm
DSYM_DIR=sw-calculator.so.dSYM

# Variables
CXXFLAGS=-O3 `root-config --cflags` -fPIC # -pthread -stdlib=libc++ -std=c++11 -m64 -I/Applications/root_v6.06.02/include
LDFLAGS=`root-config --ldflags`
GLIBS=`root-config --glibs` -lRooFit -lRooFitCore -lHtml -lMinuit -lFumili
H_EXT = h
HEADERS = $(shell find $(SRC_DIR) -type f -name *.$(H_EXT))

SRC_EXT = cpp
SOURCES = $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))

OBJECTS_TEMP = $(SOURCES:.cpp=.o)
OBJECTS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJECTS_TEMP))

EXECUTABLE=$(BIN_DIR)/$(APP_NAME)
DICTIONARY=$(DICT_FILENAME)
SHARED_LIBRARY=$(APP_NAME).so

# Empty target ensures that list of all 'end products' are called
all: executable

debug: CXXFLAGS += -g #-ggdb -DDEBUG -g
debug: executable

executable: directories $(DICTIONARY) $(SHARED_LIBRARY) $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(SHARED_LIBRARY)
	@echo "Linking "$@
	$(CXX) -o $@ $(OBJECTS) $(SHARED_LIBRARY) $(GLIBS)
	# move .so library to bin folder
	mv $(SHARED_LIBRARY) $(BIN_DIR)/$(SHARED_LIBRARY)
	# change search location of the .so library to the executable directory of the app (macOS only)
ifeq ($(OS),Darwin)
	install_name_tool -change $(APP_NAME).so @executable_path/$(APP_NAME).so $(EXECUTABLE)
endif
	# move dictionary to the bin folder - they say you have to
	mv $(DICT_PCM_FILENAME) $(BIN_DIR)/$(DICT_PCM_FILENAME)
	rm $(DICT_FILENAME)

$(DICTIONARY): $(HEADERS) $(SRC_DIR)/LinkDef.hpp
	rootcling -f $@ -c $(CXXFLAGS) -p $^

# https://root.cern.ch/interacting-shared-libraries-rootcint (they forgot $(GLIBS) damn)
$(SHARED_LIBRARY): $(DICTIONARY) $(SOURCES)
	$(CXX) -shared -o $@ $(LDFLAGS) $(CXXFLAGS) $(GLIBS) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
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
	rm -f $(DICTIONARY)
	rm -f *.pcm

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)/event
	mkdir -p $(OBJ_DIR)/event/events
	mkdir -p $(OBJ_DIR)/model
	mkdir -p $(OBJ_DIR)/roofit
	mkdir -p $(OBJ_DIR)/util
	mkdir -p $(OBJ_DIR)/widgets
	mkdir -p $(OBJ_DIR)/widgets/importSpectrumWidget
	mkdir -p $(OBJ_DIR)/widgets/rooRealVarWidget
	mkdir -p $(OBJ_DIR)/widgets/swCalculatorWidget

echo:
	$(info SOURCES: $(SOURCES))
	$(info HEADERS: $(HEADERS))

# List of special targets that do not generate files
.PHONY: directories echo
