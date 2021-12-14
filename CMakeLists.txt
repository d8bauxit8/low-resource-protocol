cmake_minimum_required(VERSION 3.15)
project(LowResourceProtocol C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

file(GLOB SOURCE_FILES
        src/**/*.h
        src/**/*.c
        src/*.h
        src/*.c
        )

file(GLOB TESTS_FILES
        tests/**/*.test.cpp
        tests/*.test.cpp
        )

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(LowResourceProtocol ${SOURCE_FILES} ${TESTS_FILES})
target_link_libraries(LowResourceProtocol gtest_main)

enable_testing()
add_test(NAME LowResourceProtocol_test COMMAND LowResourceProtocol)