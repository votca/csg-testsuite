cmake_minimum_required(VERSION 2.6)

project(testsuite)

set(PROJECT_VERSION "0.1.0-dev")

enable_testing()

add_subdirectory(tools)
add_custom_command(OUTPUT checks COMMAND ctest --force-new-ctest-process ${ARGS})
add_custom_target(install COMMAND ${CMAKE_COMMAND} -E echo "Nothing to install, just testing!" DEPENDS checks)
