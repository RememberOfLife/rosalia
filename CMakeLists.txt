cmake_minimum_required(VERSION 3.18)
set (CMAKE_C_STANDARD 11)

project(rosalia C)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if(NOT CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg].*")
    if(supported)
        message(STATUS "IPO/LTO enabled")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "IPO/LTO not supported: <${error}>")
    endif()
else()
    message(STATUS "IPO/LTO disabled in non-release build")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# find_package(OpenSSL 3 REQUIRED)

set(INCLUDES
    includes
    src
    "${CMAKE_BINARY_DIR}/build_generated"
)

set(SOURCES
    src/impl/alloc.c
    src/impl/argparse.c
    src/impl/config.c
    src/impl/heapstackify.c
    src/impl/jobs.cpp
    src/impl/math.c
    src/impl/noise.c
    src/impl/serialization.c
    src/impl/timestamp.c

    src/tests/base64.c
    src/tests/heapstackify.c
    src/tests/rand.c
    src/tests/raw_stream.c
    src/tests/serialization.c

    src/main.c
)

add_executable(rosalia_test "${SOURCES}" "${CMAKE_BINARY_DIR}/build_generated/generated/git_commit_hash.h")

# export git commit hash and dirty flag to generated file in the build/build_generated/generated dir
find_package(Git)
set(dollar "$") # next file is processed using configure_file so we need to escape dollars to use variables
configure_file(
    cmake/generate_git_commit_hash.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/generate_git_commit_hash.cmake
    ESCAPE_QUOTES
)
add_custom_target(add_generated_dir COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/build_generated/generated")
add_custom_target(generate_git_commit_hash DEPENDS add_generated_dir COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/generate_git_commit_hash.cmake" BYPRODUCTS "${CMAKE_BINARY_DIR}/build_generated/generated/git_commit_hash.h")
add_dependencies(rosalia_test generate_git_commit_hash)

target_compile_options(rosalia_test PRIVATE
    # "-Wall"
    # "-Wextra"
    # "-Werror" # warnings as errors
    "-Wfatal-errors" # stop after first error
)

target_include_directories(rosalia_test PRIVATE ${INCLUDES})

# target_link_options(rosalia_test PRIVATE -rdynamic)

target_link_libraries(rosalia_test Threads::Threads)

# target_link_libraries(rosalia_test OpenSSL::SSL)

set_target_properties(rosalia_test PROPERTIES EXPORT_COMPILE_COMMANDS true)
