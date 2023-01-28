cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include_directories(
	"src"
	"${PROJECT_SOURCE_DIR}/peon" 
	"${PROJECT_SOURCE_DIR}/extlib/include"
	"${PROJECT_SOURCE_DIR}/extlib/include/bullet"
)

set(GAME_SOURCES 
	"src/main.cpp"

)

set(GAME_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(game "src/main.cpp" ${GLAD_EXT} ${GAME_SOURCES})

if(PEON_OS_WINDOWS AND PEON_COMPILER_MSVC)
	target_link_libraries(game BulletCollision_vs2010_x64_debug BulletDynamics_vs2010_x64_debug LinearMath_vs2010_x64_debug peon)

	set(GAME_OUTPUT_DIR ${GAME_OUTPUT_DIR}/win/msvc/${ARCH_BITS})	

	SetTargetOutputLocations(game GAME_OUTPUT_DIR)

	string(REPLACE "/" "\\" GAME_RES_LINK_PATH ${GAME_OUTPUT_DIR}/${PEON_BUILD_TYPE}/res)

	string(REPLACE "/" "\\" GAME_RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res")

	add_custom_command(TARGET game POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/extlib/msvc/${ARCH_BITS}/${PEON_BUILD_TYPE}/assimp-vc130-mtd.dll ${GAME_OUTPUT_DIR}/${PEON_BUILD_TYPE})
	add_custom_command(TARGET game POST_BUILD COMMAND if not exist ${GAME_RES_LINK_PATH} MKLINK /d ${GAME_RES_LINK_PATH} ${GAME_RES_PATH})
else()
	list(APPEND CMAKE_PREFIX_PATH /usr/local/lib)
	find_library(GLFW_LIB glfw)
	find_library(BULLET_COLLISION_LIB BulletCollision)
	find_library(BULLET_DYNAMICS_LIB BulletDynamics)
	find_library(LINEAR_MATH_LIB LinearMath)
	target_link_libraries(game peon ${GLFW_LIB} ${BULLET_COLLISION_LIB} ${BULLET_DYNAMICS_LIB} ${LINEAR_MATH_LIB})
endif()

source_group("src" FILES
	${GLAD_EXT}
	"src/main.cpp"
) 