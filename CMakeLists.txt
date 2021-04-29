cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(AFRODITE VERSION 1.0.0 LANGUAGES C CXX)

# Make sure CMake finds the CMake scripts
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.2/lib/cmake")
list(APPEND CMAKE_PREFIX_PATH "/Users/vetlewi/prog/lib/Geant4-10.7.1")

#list(APPEND CMAKE_CXX_FLAGS "-flto")
#list(APPEND CMAKE_EXE_LINK_FLAGS "-flto")
#list(APPEND CMAKE_SHARED_LINK_FLAGS "-flto")

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
        GITHUB_REPOSITORY jarro2783/cxxopts
        VERSION 2.2.1
        OPTIONS "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_BUILD_EXAMPLES NO"
)

CPMAddPackage(gh:ikalnytskyi/termcolor@2.0.0)

CPMAddPackage(
	NAME CADMesh
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/CADMesh)


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

#----------------------------------------------------------------------------
# Detector factory library

add_library(detector
        src/Detectors/CloverFactory.cc
        src/Detectors/OCLLaBr3.cc
        src/Detectors/FTALaBr3.cc
        src/Detectors/S2Factory.cc
)

target_include_directories(detector
    PUBLIC
        $<)


add_executable(AFRODITE
        app/AFRODITE.cc
        src/ActionInitialization.cc

        src/Detectors/CloverFactory.cc
        src/Detectors/OCLLaBr3.cc
        src/Detectors/FTALaBr3.cc
        src/Detectors/S2Factory.cc

        src/DetectorConstruction.cc
        src/DetectorSetupMessenger.cc
        src/EventAction.cc
        src/PrimaryGeneratorAction.cc
        src/RunAction.cc
        src/SteppingAction.cc)

target_include_directories(AFRODITE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(AFRODITE ${Geant4_LIBRARIES} CADMesh::CADMesh cxxopts termcolor::termcolor)
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
