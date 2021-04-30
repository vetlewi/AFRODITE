find_package(PkgConfig QUIET)

pkg_check_modules(PC_ASSIMP QUIET assimp)

# Find the headers
find_path(ASSIMP_INCLUDE_DIR
    NAMES
        postprocess.h
    HINTS
        ${PC_ASSIMP_INCLUDE_DIRS}
        /usr/include
        /usr/local/include
        /usr/include/assimp
        /usr/local/include/assimp
        )

# Find the library
find_library(ASSIMP_LIBRARY
    NAMES
        assimp NAMES_PER_DIR HINTS
        ${PC_ASSIMP_LIBRARY_DIRS}
        /usr/lib
        /usr/local/lib
        /usr/lib64
        /usr/local/lib64
        )

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(ASSIMP DEFAULT_MSG ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)

if ( ASSIMP_FOUND )
    set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
    set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR}/..)
    if (NOT TARGET assimp::assimp)
        add_library(assimp::assimp UNKNOWN IMPORTED)
        set_target_properties(assimp::assimp PROPERTIES
                IMPORTED_LOCATION "${ASSIMP_LIBRARIES}"
                INTERFACE_INCLUDE_DIRECTORIES "${ASSIMP_INCLUDE_DIRS}"
                )
    endif()
endif()

mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)