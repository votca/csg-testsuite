cmake_minimum_required(VERSION 2.6)

project(testsuite)

set(PROJECT_VERSION "0.1.0-dev")

add_subdirectory(tools)
add_custom_target(install COMMAND ${CMAKE_COMMAND} -E echo "Nothing to install!")
