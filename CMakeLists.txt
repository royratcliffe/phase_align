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
    test/le32.c
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
add_test(NAME le32 COMMAND test_runner test/le32)

# https://cmake.org/cmake/help/latest/module/FindDoxygen.html
# https://www.mcternan.me.uk/mscgen/
find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)
if(DOXYGEN_FOUND)
    set(OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    doxygen_add_docs(doxy ${PROJECT_SOURCE_DIR}/inc ${PROJECT_SOURCE_DIR}/src)
endif()

# Add custom commands for code analysis. Enable cppcheck for Debug build type.
# This will run cppcheck only if the build type is Debug.
find_program(CPPCHECK cppcheck)
if(CPPCHECK AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_target(
        cppcheck
        COMMAND ${CPPCHECK}
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            --output-file=${CMAKE_BINARY_DIR}/cppcheck_report.txt
            --enable=all
            --suppressions-list=${CMAKE_SOURCE_DIR}/cmake/cppcheck_suppressions.txt
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cppcheck for code analysis"
        # Use VERBATIM to ensure the command is executed as written.
        # This is useful for handling paths with spaces or special characters.
        VERBATIM
    )
    set_property(TARGET cppcheck PROPERTY FOLDER "Analysis")
else()
    message(STATUS "Cppcheck not found or not enabled for Debug build type")
endif()
