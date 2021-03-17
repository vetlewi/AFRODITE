
set(assimp_FOUND 0)
set(CMAKE_FIND_FRAMEWORK_SAVE ${CMAKE_FIND_FRAMEWORK})
set(CMAKE_FIND_FRAMEWORK NEVER)
set(CMAKE_REQUIRED_QUIET_SAVE ${CMAKE_REQUIRED_QUIET})
set(CMAKE_REQUIRED_QUIET ${assimp_FIND_QUIETLY})

find_path(
	assimp_INCLUDE_PATH
	NAMES postprocess.h scene.h version.h config.h cimport.h
	PATHS /Users/vetlewi/prog/include /Users/vetlewi/prog/include/assimp)

find_library(
	assimp_LIB
	NAMES assimp
	PATHS /Users/vetlewi/prog/lib /Users/vetlewi/prog/lib/assimp)

if (assimp_INCLUDE_PATH AND assimp_LIB)
    set(assimp_FOUND 1)
endif ()

if (NOT TARGET assimp::assimp)
	add_library(assimp::assimp UNKNOWN IMPORTED)
	set_target_properties(assimp::assimp PROPERTIES
		IMPORTED_LOCATION ${assimp_LIB}
		INTERFACE_INCLUDE_DIRECTORIES ${assimp_INCLUDE_PATH}/..)
endif()

mark_as_advanced(
	assimp_INCLUDE_PATH
	assimp_LIB)

set(CMAKE_FIND_FRAMEWORK ${CMAKE_FIND_FRAMEWORK_SAVE})
set(CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET_SAVE})