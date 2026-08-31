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
include(${PROJECT_SOURCE_DIR}/cmake/EmbedBinary.cmake)

CPMAddPackage("gh:Neargye/magic_enum@0.9.7")

#----------------------------------------------------------------------------
# happly: header-only .ply parser, vendored under external/happly
add_library(happly INTERFACE)
add_library(happly::happly ALIAS happly)
target_include_directories(happly INTERFACE ${PROJECT_SOURCE_DIR}/external/happly)
target_compile_features(happly INTERFACE cxx_std_11)

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

target_link_libraries(MeshReader PRIVATE happly::happly ${Geant4_LIBRARIES})
set_target_properties(MeshReader PROPERTIES CXX_STANDARD 20)

#----------------------------------------------------------------------------
# Embed the .ply mesh files as ordinary compiler-emitted const arrays.
#
# This used to be done via incbin.h's raw `.incbin` inline-asm trick, which
# is fine on macOS/Clang but on GCC/aarch64/ELF could leave a dynamically
# initialized global's storage inside the read-only .text segment (see the
# SIGSEGV this replaced). bin2cpp generates a normal .cc/.hh pair per file
# instead, so the compiler places the data like any other global.

add_executable(bin2cpp tools/bin2cpp.cc)
set_target_properties(bin2cpp PROPERTIES CXX_STANDARD 17)

set(CLOVER_MESH_ROOT "${PROJECT_SOURCE_DIR}/Mesh-Models/DETECTORS/CLOVER")
set(STRUCTURES_MESH_ROOT "${PROJECT_SOURCE_DIR}/Mesh-Models/STRUCTURES")

afrodite_embed_binaries(CLOVER_MESH_SOURCES CLOVER_MESH_INCLUDE_DIR
        NAME vaccum       FILE "${CLOVER_MESH_ROOT}/CLOVER-InternalVacuum/CloverInternalVacuum_approx.ply"
        NAME encasement   FILE "${CLOVER_MESH_ROOT}/CloverEncasement/CloverEncasement_new_approx.ply"
        NAME HPGeCrystalA FILE "${CLOVER_MESH_ROOT}/HPGeCrystals/HPGe-RoundedCrystal1_10umTolerance.ply"
        NAME HPGeCrystalB FILE "${CLOVER_MESH_ROOT}/HPGeCrystals/HPGe-RoundedCrystal2_10umTolerance.ply"
        NAME HPGeCrystalC FILE "${CLOVER_MESH_ROOT}/HPGeCrystals/HPGe-RoundedCrystal3_10umTolerance.ply"
        NAME HPGeCrystalD FILE "${CLOVER_MESH_ROOT}/HPGeCrystals/HPGe-RoundedCrystal4_10umTolerance.ply"
        NAME HPGeContactA FILE "${CLOVER_MESH_ROOT}/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact1_10um.ply"
        NAME HPGeContactB FILE "${CLOVER_MESH_ROOT}/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact2_10um.ply"
        NAME HPGeContactC FILE "${CLOVER_MESH_ROOT}/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact3_10um.ply"
        NAME HPGeContactD FILE "${CLOVER_MESH_ROOT}/HPGeCrystals/HPGe_pureCylindricalBorehole_LithiumContact4_10um.ply"
        NAME body         FILE "${CLOVER_MESH_ROOT}/Shield/Body/Body_Modified2_tol_10um.ply"
        NAME heavimet     FILE "${CLOVER_MESH_ROOT}/Shield/Heavimet/HEAVIMET_30mm.ply"
        NAME PMTConArray  FILE "${CLOVER_MESH_ROOT}/Shield/PMT-Connectors/PMT-ConnecterArray.ply"
        NAME BGOCrystal0  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_1.ply"
        NAME BGOCrystal1  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_2.ply"
        NAME BGOCrystal2  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_3.ply"
        NAME BGOCrystal3  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_4.ply"
        NAME BGOCrystal4  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_5.ply"
        NAME BGOCrystal5  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_6.ply"
        NAME BGOCrystal6  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_7.ply"
        NAME BGOCrystal7  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_8.ply"
        NAME BGOCrystal8  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_9.ply"
        NAME BGOCrystal9  FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_10.ply"
        NAME BGOCrystal10 FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_11.ply"
        NAME BGOCrystal11 FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_12.ply"
        NAME BGOCrystal12 FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_13.ply"
        NAME BGOCrystal13 FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_14.ply"
        NAME BGOCrystal14 FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_15.ply"
        NAME BGOCrystal15 FILE "${CLOVER_MESH_ROOT}/Shield/BGO-Crystals/BGO-Crystal_Modified_16.ply"
        NAME BGOPMT0      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT1.ply"
        NAME BGOPMT1      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT2.ply"
        NAME BGOPMT2      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT3.ply"
        NAME BGOPMT3      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT4.ply"
        NAME BGOPMT4      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT5.ply"
        NAME BGOPMT5      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT6.ply"
        NAME BGOPMT6      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT7.ply"
        NAME BGOPMT7      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT8.ply"
        NAME BGOPMT8      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT9.ply"
        NAME BGOPMT9      FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT10.ply"
        NAME BGOPMT10     FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT11.ply"
        NAME BGOPMT11     FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT12.ply"
        NAME BGOPMT12     FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT13.ply"
        NAME BGOPMT13     FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT14.ply"
        NAME BGOPMT14     FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT15.ply"
        NAME BGOPMT15     FILE "${CLOVER_MESH_ROOT}/Shield/PMTs/PMT16.ply"
)

afrodite_embed_binaries(STRUCTURES_MESH_SOURCES STRUCTURES_MESH_INCLUDE_DIR
        NAME TargetChamber FILE "${STRUCTURES_MESH_ROOT}/MathisTC/target_chamber_new_sealed_fused_10umTolerance.ply"
        NAME DetectorFrame FILE "${STRUCTURES_MESH_ROOT}/Frame/OuterFrame_100umTolerance.ply"
)

add_library(mesh_data ${CLOVER_MESH_SOURCES} ${STRUCTURES_MESH_SOURCES})
add_library(AFRODITE::mesh_data ALIAS mesh_data)
target_include_directories(mesh_data PUBLIC ${CLOVER_MESH_INCLUDE_DIR})
set_target_properties(mesh_data PROPERTIES CXX_STANDARD 17)

add_library(detector
        source/Detectors/CloverFactory.cc
        source/Detectors/OCLLaBr3.cc
        source/Detectors/FTALaBr3.cc
        source/Detectors/S2Factory.cc
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
set_target_properties(detector PROPERTIES CXX_STANDARD 20)
target_link_libraries(detector PUBLIC ${Geant4_LIBRARIES}
        PRIVATE AFRODITE::MeshReader AFRODITE::mesh_data)

add_library(AFRODITE
        source/G4User/ActionInitialization.cc
        source/G4User/Co60SourceGenerator.cc
        source/G4User/LorentzBoostedGammaSource.cc
        source/G4User/PrimaryGeneratorActionMessenger.cc
        source/G4User/DetectorConstruction.cc
        source/DetectorSetupMessenger.cc
        source/Frame/DetectorFrame.cc
        source/G4User/EventAction.cc
        source/G4User/PrimaryGeneratorAction.cc
        source/G4User/RunAction.cc
        source/G4User/SteppingAction.cc)

add_library(AFRODITE::AFRODITE ALIAS AFRODITE)
set_target_properties(AFRODITE PROPERTIES CXX_STANDARD 20)
target_include_directories(AFRODITE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(AFRODITE PUBLIC detector ${Geant4_LIBRARIES} magic_enum::magic_enum
        PRIVATE AFRODITE::mesh_data)
target_compile_definitions(AFRODITE PRIVATE SRC_PATH="${PROJECT_SOURCE_DIR}/")