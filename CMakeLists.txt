cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(AFRODITE VERSION 1.0.0 LANGUAGES C CXX)

# Make sure CMake finds the CMake scripts
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.2/lib/cmake")
list(APPEND CMAKE_PREFIX_PATH "/Users/vetlewi/prog/lib/Geant4-10.7.1")

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Download and locate external dependencies

include(cmake/CPM.cmake)

CPMAddPackage(
	NAME CADMesh
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/CADMesh)


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(AFRODITE
        app/AFRODITE.cc
        src/ActionInitialization.cc
        src/DetectorConstruction.cc
        src/EventAction.cc
        src/MagneticFieldMapping.cc
        src/OCLLaBr3.cc
        src/PrimaryGeneratorAction.cc
        src/RunAction.cc
        src/SteppingAction.cc)

target_include_directories(AFRODITE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(AFRODITE ${Geant4_LIBRARIES} CADMesh::CADMesh)

target_compile_definitions(AFRODITE PRIVATE G4VIS_USE=1 G4UI_USE=1 SRC_PATH="${PROJECT_SOURCE_DIR}/")

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B4a. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#

add_custom_command(TARGET AFRODITE POST_BUILD
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/macros/* ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS AFRODITE DESTINATION bin)
