

find_path(
	assimp_INCLUDE_PATH
	NAMES postprocess.h version.h config.h
	PATHS /Users/vetlewi/prog/include /Users/vetlewi/prog/include/assimp /usr/include /usr/include/assimp)

find_library(
	assimp_LIBRARIES
	NAMES assimp
	PATHS /Users/vetlewi/prog/lib /Users/vetlewi/prog/lib/assimp /usr/lib /usr/lib64)

if (NOT TARGET assimp::assimp)
	add_library(assimp::assimp UNKNOWN IMPORTED)
	set_target_properties(assimp::assimp PROPERTIES
		IMPORTED_LOCATION ${assimp_LIBRARIES}
		INTERFACE_INCLUDE_DIRECTORIES ${assimp_INCLUDE_PATH})
endif()