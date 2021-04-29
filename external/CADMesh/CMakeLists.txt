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

set(ON 1)
find_package(ASSIMP REQUIRED)
unset(ON)
if ( NOT ASSIMP_FOUND )
    message(FATAL_ERROR "Could not find assimp")
else()#if( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" )
    get_target_property(assimp_debug assimp::assimp IMPORTED_SONAME_${CMAKE_BUILD_TYPE})
    if ( ${assimp_debug} STREQUAL "assimp_debug-NOTFOUND" )
        message(WARNING "assimp only has no IMPORTED_SONAME_${CMAKE_BUILD_TYPE} property. Adding manually IMPORTED_SONAME property")
        set_target_properties(assimp::assimp PROPERTIES
                IMPORTED_SONAME "${ASSIMP_LIBRARIES}"
                IMPORTED_LOCATION "${ASSIMP_ROOT_DIR}/lib/${sharedLibraryName}")
    endif()
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

target_link_libraries(CADMesh PUBLIC tet::tet assimp::assimp ${Geant4_LIBRARIES})