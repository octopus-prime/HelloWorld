cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	message(STATUS "compiler=gcc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	message(STATUS "compiler=msvc")
	add_compile_options(/Ox /arch=bmi2)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox /arch=bmi2")
endif()

project(HelloWorld CXX)

file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ./src/*.cpp ./src/*.hpp)

add_executable(HelloWorld ${SOURCES})

message(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
