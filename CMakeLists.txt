cmake_minimum_required(VERSION 3.5)
project (ParallelMatixMultiplication VERSION 1.0.0 LANGUAGES CXX )

#=== FINDING PACKAGES ===#

#--------------------------------
# This is for old cmake versions
set (CMAKE_CXX_STANDARD 14)
#--------------------------------

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )

#=== SETTING VARIABLES ===#
# Compiling flags
set( GCC_COMPILE_FLAGS "-Wall -fsanitize=address -g" )
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS} ${PREPROCESSING_FLAGS}" )

#Include dir
include_directories( lib )

#=== Main App ===

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(output src/auxiliar.cpp)