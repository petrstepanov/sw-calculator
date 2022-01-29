# ROOT CMake file written by Petr Stepanov (stepanovps@gmail.com)

# Instructions:
# 1. Put this CMakeLists.txt in the program directory containing project files: source files, header files and LinkDef.h
# 2. C++ file containing main() function must be named main.c (or .cc, .cpp etc...)
# 3. Specify the name of your executable below (stored it CMake PROJECT_NAME variable):
  project(sw-calculator)

# Set the minimum required version of cmake for a project
  cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Locate the ROOT package and define a number of useful targets and variables (need RIO Net?)
  find_package(ROOT REQUIRED COMPONENTS)
# message(STATUS "Found ROOT libraries:")
# message(STATUS "${ROOT_LIBRARIES}")

# Compose list of ROOT libraries with "ROOT::" prefix
  set(LIB_NAMES "")
  FOREACH(X IN LISTS ROOT_LIBRARIES)
    get_filename_component(FILENAME ${X} NAME)
    string(REGEX REPLACE "lib([a-zA-Z0-9]+).so" "ROOT::\\1" FILENAME2 ${FILENAME})
    list(APPEND LIB_NAMES "${FILENAME2}")
  ENDFOREACH()

# Manually append extra ROOT libraries (why missing?)
  list(APPEND LIB_NAMES "ROOT::Gui")
  list(APPEND LIB_NAMES "ROOT::RooFit")
  list(APPEND LIB_NAMES "ROOT::RooFitCore")
  list(APPEND LIB_NAMES "ROOT::Html")
  list(APPEND LIB_NAMES "ROOT::Minuit")
  list(APPEND LIB_NAMES "ROOT::Fumili")

# message(STATUS "Modified ROOT libraries:")
# message(STATUS "${LIB_NAMES}")

#----------------------------------------------------------------------------
# Attach ROOT and Geant Sources
# https://stackoverflow.com/questions/9581358/using-cmake-to-index-source-files-of-an-external-library-with-eclipse

# Function searches for the C++ source files in the given folder
# Next creates a subfolder in the project source folder and symlinks there all found sources
function(attachExternalSources librarySourceLocation folderName)
	# Create folder for Geant4 sources
	file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${folderName})

	message(STATUS "Searching for C++ sources in\n   \"${librarySourceLocation}\"")
	FILE(GLOB_RECURSE libSources
		${librarySourceLocation}/*.c
		${librarySourceLocation}/*.cpp
		${librarySourceLocation}/*.cxx
		${librarySourceLocation}/*.cc
	)

	message(STATUS "Symlinking sources into\n   \"${CMAKE_SOURCE_DIR}/${folderName}\"\n   Please wait...")
	foreach(source ${libSources})
		# Obtain source filename
		get_filename_component(source_filename ${source} NAME)

		# Create symlink unless it already exists
		set(symlink "${CMAKE_SOURCE_DIR}/${folderName}/${source_filename}")
		if(NOT EXISTS ${symlink})
			execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${symlink})
		endif()
	endforeach()

	# Scan all the symlinks created under the project folder and disable their compilation
	FILE(GLOB sources_symlinks ${CMAKE_SOURCE_DIR}/${folderName}/*)
	SET_SOURCE_FILES_PROPERTIES(${sources_symlinks} PROPERTIES HEADER_FILE_ONLY TRUE)
endfunction()

if(DEFINED ROOT_SOURCE_LOCATION)
	# Expand ROOT source path variables (wildcard)
	FILE(GLOB ROOT_SOURCE_LOCATIONS_EXPAND "${ROOT_SOURCE_LOCATION}")
	# Get last GLOB element
	list(GET ROOT_SOURCE_LOCATIONS_EXPAND -1 ROOT_SOURCE_LOCATION_EXPAND)
	message(STATUS "Found CERN ROOT source code folder:\n   \"${ROOT_SOURCE_LOCATION_EXPAND}\"")
	attachExternalSources(${ROOT_SOURCE_LOCATION_EXPAND} "root-sources")
else()
	message(STATUS "CERN ROOT source folder location not provided")
endif()

#--------------------------------
# Save project file
if(EXISTS ${CMAKE_ROOT}./project)
  message(STATUS "Found Eclipse project configuration: ${CMAKE_ROOT}./project")
  FILE(READ "${CMAKE_ROOT}./project" OLD_PROJECT_SETTINGS)
endif()


#----------------------------------------------------------------------------
# Build list of header files and exclude LinkDef from headers
  file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hxx ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
  list(FILTER HEADERS EXCLUDE REGEX "[lL]ink[dD]ef")
# message(STATUS "Found list of headers:")
# message(STATUS "${HEADERS}")

#----------------------------------------------------------------------------
# Find LinkDef.h file
  file(GLOB_RECURSE LINKDEFH ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hxx ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
  list(FILTER LINKDEFH INCLUDE REGEX "[lL]ink[dD]ef")
# message(STATUS "Found LinkDef file: ${LINKDEFH}")

#----------------------------------------------------------------------------
# Generate dictionary
  ROOT_GENERATE_DICTIONARY(D_${PROJECT_NAME} ${HEADERS} LINKDEF ${LINKDEFH})

#----------------------------------------------------------------------------
# Build list of source files
  file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
# message(STATUS "Found list of sources:")
# message(STATUS "${SOURCES}")

#----------------------------------------------------------------------------
# Create shared library with a generated dictionary.
  add_library(SO_${PROJECT_NAME} ${SOURCES} D_${PROJECT_NAME}.cxx)

#----------------------------------------------------------------------------
# Link against shared library and list of ROOT libraries
  target_link_libraries(SO_${PROJECT_NAME} PUBLIC ${LIB_NAMES})

#----------------------------------------------------------------------------
# Find location of the enrty point file (main.c*)
  file(GLOB_RECURSE MAIN ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
  list(FILTER MAIN INCLUDE REGEX "main\\.c")
# message(STATUS "Found entry point file: ${MAIN}")

#----------------------------------------------------------------------------
# Create the main program using the library.
  add_executable(${PROJECT_NAME} ${MAIN})
  target_link_libraries(${PROJECT_NAME} SO_${PROJECT_NAME})

#----------------------------------------------------------------------------
# Compose the install target
install(TARGETS ${PROJECT_NAME} SO_${PROJECT_NAME} 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION $ENV{ROOTSYS}/lib
        ARCHIVE DESTINATION $ENV{ROOTSYS}/lib)
install(FILES ${PROJECT_BINARY_DIR}/libD_${PROJECT_NAME}.rootmap
        DESTINATION $ENV{ROOTSYS}/lib)
install(FILES ${PROJECT_BINARY_DIR}/libD_${PROJECT_NAME}_rdict.pcm
        DESTINATION /usr/local/bin)

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



#----------------------------------------------------------------------------
# Try custom post install targets

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Running no-byproduct command after 'sw-calculator'"
  )

#add_custom_command(
#  TARGET foo
#  POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -E echo "Running no-byproduct command after 'rebuild-cache'"
#  )

#function(test)
#  message(STATUS "Test")
#endfunction()

#function(patch_eclipse_makefile)
#  message(STATUS "Locating Makefile: ${PROJECT_BINARY_DIR}/Makefile")
#  if(EXISTS ${PROJECT_BINARY_DIR}/Makefile)
#    message(STATUS "Found Makefile. Pathcing...")
#    file(READ "${PROJECT_BINARY_DIR}/Makefile" MF)
#    string(REPLACE "rebuild_cache:" "rebuild_cache\n\tcp $(CMAKE_BINARY_DIR)/.project $(CMAKE_BINARY_DIR)/.project_backup" MF2 ${MF})
#    string(REPLACE ".PHONY : rebuild_cache" "cp $(CMAKE_BINARY_DIR)/.project_backup $(CMAKE_BINARY_DIR)/.project\n.PHONY : rebuild_cache" MF3 ${MF2})
#    file(WRITE ${PROJECT_BINARY_DIR}/Makefile ${MF3})
#  else()
#    message(STATUS "Makefile not found")
#  endif()
#endfunction()

#add_custom_command(
#    POST_BUILD
#    COMMAND test()
#    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#    OUTPUT  Makefile
#    COMMENT "Patching Makefile..."
#)

#add_custom_target(
#    mmm ALL
#    DEPENDS Makefile
#    Makefile
#) 


#-------------------------------------------------------------------------------------------
#add_executable(lol main.cpp)

# Tweak Eclipse Generator "rebuild_cache" target. Protect from overriding project preferences
#add_custom_command(TARGET lol PRE_BUILD
#  COMMAND file(COPY_FILE ${PROJECT_BINARY_DIR}/.cproject ${PROJECT_BINARY_DIR}/.cproject_bak)
#  COMMENT "Storing '.cproject' before 'rebuild_cache'"
#)

#add_custom_command(TARGET lol POST_BUILD
#  COMMAND file(COPY_FILE ${PROJECT_BINARY_DIR}/.cproject_bak ${PROJECT_BINARY_DIR}/.cproject)
#  COMMENT "Restoring '.cproject' after 'rebuild_cache'"
#)


#add_custom_command(TARGET rebuild_cache PRE_BUILD
#  COMMAND FILE(READ "${CMAKE_ROOT}./project" OLD_PROJECT_SETTINGS)
#  COMMENT "Saving Eclipse project"
#)

#add_custom_command(TARGET all POST_BUILD
#  COMMAND file(WRITE ${CMAKE_ROOT}./project ${OLD_PROJECT_SETTINGS})
#  COMMENT "Restoring Eclipse project"
#  WORKING_DIRECTORY WORKING_DIRECTORY ${CMAKE_ROOT}
#)

#if(DEFINED PERSIST_ECLIPSE_PROJECT)
#  if(EXISTS ${CMAKE_ROOT}./project)
#    message(STATUS "Found Eclipse project configuration: ${CMAKE_ROOT}./project")
#    FILE(READ "${CMAKE_ROOT}./project" OLD_PROJECT_SETTINGS)
#    message(STATUS "Saving Eclipse project")		
#    endif()

#    add_custom_command(OUTPUT ${CMAKE_ROOT}./project
#                       COMMAND file(WRITE ${CMAKE_ROOT}./project ${OLD_PROJECT_SETTINGS})
#                       COMMENT "Restoring Eclipse project"
#                       DEPENDS all)
#endif()
