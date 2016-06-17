cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

include_directories(
	"inc"
	"${PROJECT_SOURCE_DIR}/peon" 
	"${PROJECT_SOURCE_DIR}/extlib/inc"
)

file(GLOB_RECURSE GAME_HEADERS src/*.h)
file(GLOB_RECURSE GAME_SOURCES src/*.cpp)

set(GAME_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(game ${GAME_HEADERS} ${GAME_SOURCES})

target_link_libraries(game peon)

if(PEON_OS_WINDOWS AND PEON_COMPILER_MSVC)
	
	set(GAME_OUTPUT_DIR ${GAME_OUTPUT_DIR}/win/msvc/${ARCH_BITS})	

	SetTargetOutputLocations(game GAME_OUTPUT_DIR)

	string(REPLACE "/" "\\" GAME_RES_LINK_PATH ${GAME_OUTPUT_DIR}/${PEON_BUILD_TYPE}/res)

	string(REPLACE "/" "\\" GAME_RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res")

	add_custom_command(TARGET game POST_BUILD COMMAND if not exist ${GAME_RES_LINK_PATH} MKLINK /d ${GAME_RES_LINK_PATH} ${GAME_RES_PATH})
endif()
