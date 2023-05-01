cmake_minimum_required(VERSION 3.15)
project(LowResourceProtocol C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB SOURCE_FILES
        src/**/**/*.h
        src/**/**/*.c
        src/**/*.h
        src/**/*.c
        src/*.h
        src/*.c
        )

file(GLOB TESTS_FILES
        tests/**/**/*.test.cpp
        tests/**/*.test.cpp
        tests/*.test.cpp
        )

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(LowResourceProtocol ${SOURCE_FILES} ${TESTS_FILES})
target_link_libraries(LowResourceProtocol gtest_main)

enable_testing()
add_test(NAME LowResourceProtocol_test COMMAND LowResourceProtocol)