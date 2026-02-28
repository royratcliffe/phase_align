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
            --output-file=${CMAKE_BINARY_DIR}/cppcheck_report.xml
            --xml
            --enable=all
            --enable=warning,style,performance,portability,information
            --suppressions-list=${CMAKE_SOURCE_DIR}/cppcheck_suppressions.txt
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

# Add custom commands for code coverage analysis. Enable lcov and genhtml if
# they are found. Only enable code coverage analysis for Debug build type, as it
# is typically used for development and testing.
# Note that code coverage analysis can add overhead to the build and runtime, so
# it is generally not recommended for Release builds.
#
# If lcov is found, add the necessary compiler and linker flags to enable code
# coverage analysis.
# Note that find_program will return the path to the executable if found, or
# NOTFOUND if not found. It will also cache the result, so subsequent calls will
# return the cached value. This allows us to check if lcov is available and
# conditionally add code coverage targets.
find_program(LCOV lcov)
if(LCOV AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(phase_align PRIVATE --coverage)
    target_compile_options(test_runner PRIVATE --coverage)
    target_link_options(phase_align PRIVATE --coverage)
    target_link_options(test_runner PRIVATE --coverage)

    # Add custom targets for code coverage analysis using lcov and genhtml.
    # These targets will run the tests, capture coverage data, and generate an HTML report.
    add_custom_target(ctest
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        DEPENDS test_runner
    )
    add_custom_target(lcov
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${LCOV} --capture --directory . --output-file coverage.info
        COMMAND ${LCOV} --remove coverage.info '/usr/*' '*/test*' --output-file coverage_filtered.info
        DEPENDS ctest
    )

    find_program(GENHTML genhtml)
    if(GENHTML)
        message(STATUS "lcov and genhtml found, code coverage targets will be available")
        add_custom_target(genhtml
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${GENHTML} coverage_filtered.info --output-directory coverage_report
            COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated in: ${CMAKE_BINARY_DIR}/coverage_report/index.html"
            DEPENDS lcov
        )
    else()
        message(STATUS "lcov and/or genhtml not found, code coverage targets will not be available")
    endif()
endif()
