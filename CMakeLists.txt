cmake_minimum_required(VERSION 3.25)
project(phase_align)

# Set the C standard to C11.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(phase_align
    inc/phase_align.h
    src/phase_align.c
)

# Set the include directories for the phase_align library.
target_include_directories(phase_align
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:include>
)

# Add a CTest executable for running all tests.
# This will be used to run the tests defined in the test sources.
# The test sources will be compiled into a test executable.
include(CTest)
create_test_sourcelist(test_sources
    test_runner.c
    test/phase_align_no_shift.c
    test/phase_align_right_shift.c
    test/phase_align_left_shift.c
)

# Add a test executable that links against the phase_align library.
# The test executable will be built from the test sources.
# It will include the phase_align library and any necessary test files.
add_executable(test_runner
    ${test_sources}
)
target_link_libraries(test_runner phase_align)

add_test(NAME no_shift COMMAND test_runner test/phase_align_no_shift)
add_test(NAME right_shift COMMAND test_runner test/phase_align_right_shift)
add_test(NAME left_shift COMMAND test_runner test/phase_align_left_shift)

# https://cmake.org/cmake/help/latest/module/FindDoxygen.html
# https://www.mcternan.me.uk/mscgen/
find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)
if(DOXYGEN_FOUND)
    set(OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    doxygen_add_docs(doxy ${PROJECT_SOURCE_DIR}/inc ${PROJECT_SOURCE_DIR}/src)
endif()

# Before running the full analysis, `cppcheck` needs to parse the source files
# correctly. If headers are missing or configuration macros are wrong, the
# analysis results will prove to be incomplete or misleading.
set(CMAKE_C_CPPCHECK "cppcheck;--enable=all;--inconclusive;--quiet")
