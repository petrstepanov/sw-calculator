# Environment
CXX=clang++
SRC_DIR=src
OBJ_DIR=build
BIN_DIR=dist

APP_NAME=sw-calculator
DICT_FILENAME=SWDictionary.cpp
DICT_PCM_FILENAME=SWDictionary_rdict.pcm

# Variables
CXXFLAGS=`root-config --cflags` # -pthread -stdlib=libc++ -std=c++11 -m64 -I/Applications/root_v6.06.02/include
GLIBS=`root-config --glibs` -lRooFit -lRooFitCore -lHtml -lMinuit -lFumili
HEADERS=src/event/Event.h \
        src/event/EventBus.h \
        src/event/EventHandler.h \
        src/event/HandlerRegistration.h \
        src/event/Object.h \
        src/event/events/HistogramImportedEvent.h \
        src/event/events/IsTwoDetectorEvent.h \
        src/event/events/SourceHistogramImportedEvent.h \
        src/model/Constants.h \
        src/model/Model.h \
        src/roofit/AbstractModelProvider.h \
        src/roofit/BackgroundPdf.h \
        src/roofit/ChannelConvolutionPdf.h \
        src/roofit/CompositeModelProvider.h \
        src/roofit/DampLorentzPdf.h \
        src/roofit/GaussianPdf.h \
        src/roofit/IndirectParamPdf.h \
        src/roofit/LorentzianPdf.h \
        src/roofit/OrthogonalPdf.h \
        src/roofit/ParabolaGaussModelProvider.h \
        src/roofit/ParabolaLorentzianModelProvider.h \
        src/roofit/ParabolaPdf.h \
        src/util/FileUtils.h \
        src/util/GraphicsHelper.h \
        src/util/HistProcessor.h \
        src/util/RootHelper.h \
        src/util/StringUtils.h \
        src/util/UiHelper.h \
        src/util/Variable.h \
        src/widgets/importSpectrumWidget/AbstractImportSpectrumPresenter.h \
        src/widgets/importSpectrumWidget/AbstractImportSpectrumView.h \
        src/widgets/importSpectrumWidget/ImportSourceSpectrumPresenter.h \
        src/widgets/importSpectrumWidget/ImportSourceSpectrumView.h \
        src/widgets/importSpectrumWidget/ImportSpectrumPresenter.h \
        src/widgets/importSpectrumWidget/ImportSpectrumView.h \
        src/widgets/rooRealVarWidget/RooRealVarPresenter.h \
        src/widgets/rooRealVarWidget/RooRealVarView.h \
        src/widgets/swCalculatorWidget/SWCalculatorPresenter.h \
        src/widgets/swCalculatorWidget/SWCalculatorView.h \
        src/widgets/AbstractPresenter.h \
        src/widgets/AbstractView.h \
        src/widgets/MainView.h \
        src/main.h
FILES=event/EventBus.cpp \
      event/events/HistogramImportedEvent.cpp \
      event/events/IsTwoDetectorEvent.cpp \
      event/events/SourceHistogramImportedEvent.cpp \
      model/Constants.cpp \
      model/Model.cpp \
      roofit/AbstractModelProvider.cpp \
      roofit/BackgroundPdf.cpp \
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
      util/UiHelper.cpp \
      util/Variable.cpp \
      widgets/importSpectrumWidget/AbstractImportSpectrumPresenter.cpp \
      widgets/importSpectrumWidget/AbstractImportSpectrumView.cpp \
      widgets/importSpectrumWidget/ImportSourceSpectrumPresenter.cpp \
      widgets/importSpectrumWidget/ImportSourceSpectrumView.cpp \
      widgets/importSpectrumWidget/ImportSpectrumPresenter.cpp \
      widgets/importSpectrumWidget/ImportSpectrumView.cpp \
      widgets/rooRealVarWidget/RooRealVarPresenter.cpp \
      widgets/rooRealVarWidget/RooRealVarView.cpp \
      widgets/swCalculatorWidget/SWCalculatorPresenter.cpp \
      widgets/swCalculatorWidget/SWCalculatorView.cpp \
      widgets/MainView.cpp \
      main.cpp

SOURCES=$(addprefix $(SRC_DIR)/,$(FILES))
OBJECTS_TEMP=$(SOURCES:.cpp=.o)
OBJECTS=$(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJECTS_TEMP))
EXECUTABLE=$(BIN_DIR)/$(APP_NAME)
DICTIONARY=$(DICT_FILENAME)
SHARED_LIBRARY=$(APP_NAME).so
# List of special targets that do not generate files
.PHONY: directories

# Empty target ensures that list of all 'end products' are called
all: directories $(DICTIONARY) $(SHARED_LIBRARY) $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(SHARED_LIBRARY)
	@echo "Linking "$@
	$(CXX) -o $@ $(OBJECTS) $(SHARED_LIBRARY) $(GLIBS)
	# move .so library to bin folder
	mv $(SHARED_LIBRARY) $(BIN_DIR)/$(SHARED_LIBRARY)
	# change search location of the .so library to the executable directory of the app
	install_name_tool -change $(APP_NAME).so @executable_path/$(APP_NAME).so $(EXECUTABLE)
	# move dictionary to the bin folder - they say you have to
	mv $(DICT_PCM_FILENAME) $(BIN_DIR)/$(DICT_PCM_FILENAME)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling "$@
# compile with dependency files - why?
#	$(CXX) $(CXXFLAGS) -c -g -MMD -MP -MF "$@.d" -o $@ $<
# compile with debug symbols
#	$(CXX) $(CXXFLAGS) -c -g $< -o $@
# just compile
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SHARED_LIBRARY): $(DICTIONARY)
	$(CXX) -shared -o $@ $(CXXFLAGS) $(GLIBS) $< $(SOURCES)

$(DICTIONARY):
	rootcling -f $@ -c $(CXXFLAGS) -p $(HEADERS) $(SRC_DIR)/SWCalculatorLinkDef.h

clean:
	rm -r $(OBJ_DIR)
	rm -r $(BIN_DIR)
	rm $(DICTIONARY)
	rm *.pcm

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
