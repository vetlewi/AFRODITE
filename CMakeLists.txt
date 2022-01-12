cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(AFRODITE VERSION 1.0.0 LANGUAGES C CXX)

# Make sure CMake finds the CMake scripts
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake/Modules)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)

#----------------------------------------------------------------------------
# Download and locate external dependencies
include(${PROJECT_SOURCE_DIR}/cmake/tools.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Modules/FindAssimp.cmake)

CPMAddPackage("gh:Neargye/magic_enum@0.7.2")

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
#include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Detector factory library

add_library(MeshReader
        source/meshreader/MeshReader.cc)

add_library(AFRODITE::MeshReader ALIAS MeshReader)

target_include_directories(MeshReader
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include/meshreader
        )

target_link_libraries(MeshReader PRIVATE assimp::assimp ${Geant4_LIBRARIES})
set_target_properties(MeshReader PROPERTIES CXX_STANDARD 17)

add_library(detector
        source/Detectors/CloverFactory.cc
        source/Detectors/OCLLaBr3.cc
        source/Detectors/FTALaBr3.cc
        source/Detectors/S2Factory.cc
        source/Detectors/MeshModels.cc
)

add_library(AFRODITE::detector ALIAS detector)

target_include_directories(detector
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/detector
)

target_compile_definitions(detector PRIVATE SRC_PATH="${PROJECT_SOURCE_DIR}/")
set_target_properties(detector PROPERTIES CXX_STANDARD 17)
target_link_libraries(detector PUBLIC ${Geant4_LIBRARIES} assimp::assimp
        PRIVATE AFRODITE::MeshReader)

add_library(AFRODITE
        source/G4User/ActionInitialization.cc
        source/G4User/Co60SourceGenerator.cc
        source/G4User/DetectorConstruction.cc
        source/DetectorSetupMessenger.cc
        source/G4User/EventAction.cc
        source/G4User/PrimaryGeneratorAction.cc
        source/G4User/RunAction.cc
        source/G4User/SteppingAction.cc)

add_library(AFRODITE::AFRODITE ALIAS AFRODITE)
set_target_properties(AFRODITE PROPERTIES CXX_STANDARD 17)
target_include_directories(AFRODITE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(AFRODITE PUBLIC detector ${Geant4_LIBRARIES} magic_enum::magic_enum)
target_compile_definitions(AFRODITE PRIVATE SRC_PATH="${PROJECT_SOURCE_DIR}/")
