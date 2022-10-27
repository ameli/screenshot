# =====================================================================================
#
#       Filename:  CMakeLists.txt
#
#    Description:  Takes screenshots of a vtk file
#
#        Version:  1.0
#        Created:  08/13/2014 03:08:27 PM
#       Revision:  none
#       Compiler:  gcc
#
#         Author:  Siavash Ameli
#   Organization:  University Of California, Berkeley
#
# =====================================================================================

cmake_minimum_required(VERSION 3.12)
project(Screenshot CXX)

# ===
# VTK
# ===

find_package(VTK REQUIRED)
if(NOT VTK_FOUND)
    message(FATAL_ERROR "VTK not found.")
endif(NOT VTK_FOUND)

# ================
# Source Inclusion
# ================

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXECUTABLE_NAME "screenshot")
add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCE_DIR}/main.cxx)
target_link_libraries(${EXECUTABLE_NAME} ${VTK_LIBRARIES})

# ==================
# Output Directories
# ==================

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE PATH "Directory for all executables")
