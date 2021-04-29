cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(AFRODITEStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)


CPMAddPackage(
        GITHUB_REPOSITORY jarro2783/cxxopts
        VERSION 2.2.1
        OPTIONS "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_BUILD_EXAMPLES NO"
)

CPMAddPackage(gh:ikalnytskyi/termcolor@2.0.0)

CPMAddPackage(NAME AFRODITE SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)


# ---- Create standalone executable ----

add_executable(AFRODITEStandalone AFRODITE.cc)
set_target_properties(AFRODITEStandalone PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "AFRODITE")
target_compile_definitions(AFRODITEStandalone PRIVATE G4VIS_USE=1 G4UI_USE=1)
target_link_libraries(AFRODITEStandalone AFRODITE::AFRODITE cxxopts termcolor::termcolor)


#----------------------------------------------------------------------------
# Copy all scripts to the build directory.
# This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#

add_custom_command(TARGET AFRODITEStandalone POST_BUILD
        COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/../macros/* ${CMAKE_CURRENT_BINARY_DIR})