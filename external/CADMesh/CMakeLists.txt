cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(CADMesh VERSION 1.1.0 LANGUAGES CXX)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()


#############################################
# Add external dependencies
#############################################

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)
include(ExternalProject)


option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
find_package(ASSIMP REQUIRED)
if ( NOT ASSIMP_FOUND )
    message(FATAL_ERROR "Could not find assimp")
else()
    message(STATUS "Found assimp: " ${ASSIMP_LIBRARIES})
endif()

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.4.1")
CPMAddPackage(
    NAME tetgen
    GITHUB_REPOSITORY christopherpoole/tetgen
    VERSION 1.5
    DOWNLOAD_ONLY True
)

if ( tetgen_ADDED )
    add_library(tet ${tetgen_SOURCE_DIR}/tetgen.cxx ${tetgen_SOURCE_DIR}/predicates.cxx)
    target_compile_definitions(tet PRIVATE TETLIBRARY)
    target_include_directories(tet PUBLIC
        $<BUILD_INTERFACE:${tetgen_SOURCE_DIR}>)
    add_library(tet::tet ALIAS tet)
endif()

#ExternalProject_Add(
#        assimp_external
#        SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/assimp
#        GIT_REPOSITORY https://github.com/assimp/assimp.git
#        GIT_TAG v3.1.1
#        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}
#)


#############################################
# Declare the library
#############################################

add_library(CADMesh 
    ${CMAKE_CURRENT_SOURCE_DIR}/source/CADMesh.cc)
add_library(CADMesh::CADMesh ALIAS CADMesh)

target_include_directories(CADMesh
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
    PRIVATE
        ${BUILD_INTERFACE}/include/CADMesh/include/CADMesh)
target_link_libraries(CADMesh PUBLIC tet::tet ASSIMP::ASSIMP ${Geant4_LIBRARIES})

