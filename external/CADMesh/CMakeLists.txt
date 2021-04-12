cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(CADMesh VERSION 1.1.0 LANGUAGES CXX)

#list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

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
    message(STATUS "Found assimp: " ${ASSIMP_LIBRARIES}, ${ASSIMP_INCLUDE_DIRS})
endif()

CPMAddPackage(
    NAME tetgen
    GITHUB_REPOSITORY christopherpoole/tetgen
    VERSION 1.5
    DOWNLOAD_ONLY True
)

if ( tetgen_ADDED )
    add_library(tet STATIC ${tetgen_SOURCE_DIR}/tetgen.cxx ${tetgen_SOURCE_DIR}/predicates.cxx)
    target_compile_definitions(tet PRIVATE TETLIBRARY)
    target_include_directories(tet PUBLIC
        $<BUILD_INTERFACE:${tetgen_SOURCE_DIR}>)
    add_library(tet::tet ALIAS tet)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
    if ( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" )
        message(STATUS "MacOS debug")
        add_library(myassimp INTERFACE)
        target_include_directories(myassimp INTERFACE ${ASSIMP_INCLUDE_DIRS})
        target_link_libraries(myassimp INTERFACE ${ASSIMP_ROOT_DIR}/lib/${ASSIMP_LIBRARIES})
    else()
        add_library(myassimp ALIAS assimp::assimp)
    endif()
else()
    add_library(myassimp ALIAS assimp::assimp)
endif()


#############################################
# Declare the library
#############################################

add_library(CADMesh STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/CADMesh.cc)
add_library(CADMesh::CADMesh ALIAS CADMesh)

target_include_directories(CADMesh
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
    PRIVATE
        ${BUILD_INTERFACE}/include/CADMesh/include/CADMesh
        )
target_link_libraries(CADMesh PUBLIC tet::tet myassimp ${Geant4_LIBRARIES})

