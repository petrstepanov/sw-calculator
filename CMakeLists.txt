# ROOT CMake file written by Petr Stepanov (stepanovps@gmail.com)

# Instructions:
# 1. Put this CMakeLists.txt in the program directory containing project files: source files, header files and LinkDef.h
# 2. C++ file containing main() function must be named main.c (or .cc, .cpp etc...)
# 3. Specify the name of your executable below (stored it CMake PROJECT_NAME variable):

# Assert CMake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# This is your project statement. You should always list languages
project(swcalculator LANGUAGES CXX)

# ROOT 6.10+ supports config file discovery
# REQUIRED ensures ROOT_FOUND is set
find_package(ROOT REQUIRED COMPONENTS RooFit RooFitCore Gui Html Minuit Fumili)

# For setting project in IDE via CMake generators
include_directories(${ROOT_INCLUDE_DIRS})

# Build list of header files and exclude LinkDef from headers
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hxx 
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp 
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh 
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
list(FILTER HEADERS EXCLUDE REGEX "[lL]ink[dD]ef")
message(STATUS "Found list of headers:")
message(STATUS "${HEADERS}")

# Find LinkDef.h file
file(GLOB_RECURSE LINKDEFH CONFIGURE_DEPENDS 
                           ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hxx 
                           ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp 
                           ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh 
                           ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
list(FILTER LINKDEFH INCLUDE REGEX "[lL]ink[dD]ef")
message(STATUS "Found LinkDef file: ${LINKDEFH}")

# Generate dictionary
ROOT_GENERATE_DICTIONARY(D_${PROJECT_NAME} ${HEADERS} LINKDEF ${LINKDEFH})

# Build list of source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx 
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc 
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
message(STATUS "Found list of sources:")
message(STATUS "${SOURCES}")

# Create target that generates shared library from provided source files and dictionary
add_library(SO_${PROJECT_NAME} SHARED ${SOURCES} D_${PROJECT_NAME}.cxx)

# Link against shared library and list of ROOT libraries
# Manually add specific libraries with ${ROOT_Hist_LIBRARY} etc.. here adding all
target_link_libraries(SO_${PROJECT_NAME} ${ROOT_LIBRARIES})

# Mark .rootmap (contains shares library dependencies) and .pcm (data to serialize classes)
# shared library target RESOURCES for easier installation
set_target_properties(SO_${PROJECT_NAME} PROPERTIES
                      RESOURCE "${CMAKE_BINARY_DIR}/libD_${PROJECT_NAME}.rootmap;${CMAKE_BINARY_DIR}/libD_${PROJECT_NAME}_rdict.pcm")

# Find location of the enrty point file (main.c*)
file(GLOB_RECURSE MAIN ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx 
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc 
                       ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
list(FILTER MAIN INCLUDE REGEX "main\\.c")
message(STATUS "Found entry point file: ${MAIN}")

# Create the main program using the library.
add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} SO_${PROJECT_NAME})

# Print all variables
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

# Compose the install target
install(TARGETS ${PROJECT_NAME} SO_${PROJECT_NAME}
        RUNTIME DESTINATION ${ROOT_BINARY_DIR}
        LIBRARY DESTINATION ${ROOT_LIBRARY_DIR}
        RESOURCE DESTINATION ${ROOT_LIBRARY_DIR})

#----------------------------------------------------------------------------
# Compose the install launcher phony target
# https://stackoverflow.com/questions/38079251/get-cmake-to-declare-a-target-phony
add_custom_target(install-launcher
  # Install icon (SVG support dropped due to slow rendering speeds of some icons)
  COMMAND bash -c "xdg-icon-resource install --novendor --context apps --size 256 ${CMAKE_CURRENT_SOURCE_DIR}/resources/${CMAKE_PROJECT_NAME}.png ${CMAKE_PROJECT_NAME}"
  COMMAND bash -c "gtk-update-icon-cache"
  # Install launcher ("forceupdate" command performed automatically unless "--noupdate" is specified)
  COMMAND bash -c "xdg-desktop-menu install ${CMAKE_CURRENT_SOURCE_DIR}/resources/${CMAKE_PROJECT_NAME}.desktop"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Installing application icon and launcher (for linux)"
)
