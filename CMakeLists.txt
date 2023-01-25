cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(Peon)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/util.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

peon_set_option(CMAKE_BUILD_TYPE Debug STRING "Choose the build type (Debug or Release)")

if(PEON_COMPILER_MSVC)
	set(PEON_COMPILE_FLAGS "/W4 /wd4100 /wd4201 /wd4127")	
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PEON_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PEON_COMPILE_FLAGS}")

set(GLAD_EXT "${CMAKE_SOURCE_DIR}/extlib/include/glad/glad.h"
			 "${CMAKE_SOURCE_DIR}/extlib/include/glad/glad.c")

set(PEON_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
if(PEON_OS_WINDOWS AND PEON_COMPILER_MSVC)
	string(TOLOWER "${PEON_BUILD_TYPE}" PEON_BUILD_TYPE)
	link_directories("${PROJECT_SOURCE_DIR}/extlib/msvc/${ARCH_BITS}/${PEON_BUILD_TYPE}")
endif()

add_subdirectory(peon)
add_subdirectory(game)



