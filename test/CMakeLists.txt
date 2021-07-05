cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(AFRODITEtests LANGUAGES CXX)


# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:onqtam/doctest#2.4.5")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.0")
CPMAddPackage(
        NAME magic_enum
        GITHUB_REPOSITORY Neargye/magic_enum
        GIT_TAG v0.7.2 # Where `x.y.z` is the release version you want to use.
        OPTIONS "MAGIC_ENUM_OPT_BUILD_EXAMPLES OFF" "MAGIC_ENUM_OPT_BUILD_TESTS OFF" "MAGIC_ENUM_OPT_INSTALL OFF"
)

CPMAddPackage(NAME AFRODITE SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

add_executable(AFRODITEtests
        ${CMAKE_CURRENT_LIST_DIR}/source/Detector.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/MeshReader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/Slot.cpp)

target_compile_definitions(AFRODITEtests PRIVATE PRIVATE SRC_PATH="${PROJECT_SOURCE_DIR}/../" TEST_INTERFACE)
target_link_libraries(AFRODITEtests doctest::doctest AFRODITE::MeshReader AFRODITE::AFRODITE magic_enum::magic_enum)
set_target_properties(AFRODITEtests PROPERTIES CXX_STANDARD 17)

# ---- Add GreeterTests ----

enable_testing()

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(AFRODITEtests)


# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
    target_compile_options(detector PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(detector PUBLIC -fprofile-arcs -ftest-coverage)
endif()