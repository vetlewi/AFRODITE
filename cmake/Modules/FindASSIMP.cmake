find_package(PkgConfig QUIET)
pkg_check_modules(PC_ASSIMP QUIET assimp)

# Find the headers
find_path(ASSIMP_INCLUDE_DIR NAMES postprocess.h HINTS
		${PC_ASSIMP_INCLUDE_DIRS}
		/Users/vetlewi/prog/include
		/Users/vetlewi/prog/include/assimp
		/usr/include
		/usr/include/assimp
)

# Find the library
find_library(ASSIMP_LIBRARY NAMES assimp NAMES_PER_DIR HINTS
		${PC_ASSIMP_LIBRARY_DIRS}
		/Users/vetlewi/prog/lib
		/usr/lib
		/usr/lib64
)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(ASSIMP
		REQUIRED_VARS ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)

if ( ASSIMP_FOUND )
	set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
	set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR}/..)

	if (NOT TARGET ASSIMP::ASSIMP)
		add_library(ASSIMP::ASSIMP UNKNOWN IMPORTED)
		set_target_properties(ASSIMP::ASSIMP PROPERTIES
				IMPORTED_LOCATION "${ASSIMP_LIBRARIES}"
				INTERFACE_INCLUDE_DIRECTORIES "${ASSIMP_INCLUDE_DIRS}"
				)#INTERFACE_COMPILE_DEFINITIONS "BOOST_ALL_DYN_LINK BOOST_ALL_NO_LIB")
	endif()
endif()

mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)