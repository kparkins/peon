cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include_directories(
	"src"
	"${PROJECT_SOURCE_DIR}/peon" 
	"${PROJECT_SOURCE_DIR}/extlib/include"
)

set(GAME_SOURCES 
	"src/main.cpp"
	"src/Entity.h"
	"src/Entity.cpp"
	"src/Pool.h"
	"src/Pool.cpp"
	"src/Component.h"
	"src/Component.cpp"
	"src/Scene.h"
	"src/Loaders.h"
	"src/Loaders.cpp"
	"src/Scene.cpp"
	"src/SceneView.h"
	"src/Atlas.h"
	"src/physics/CollisionShape.h"
	"src/physics/MotionState.h"
	"src/physics/MotionState.cpp"
	"src/physics/PhysicsSystem.cpp"
	"src/physics/PhysicsSystem.h"
	"src/physics/RigidBody.h"
	"src/physics/RigidBody.cpp"
)

set(TEST_SOURCES
	"test/test_pool.cpp"
)

set(GAME_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(test_game ${GAME_SOURCES} ${TEST_SOURCES})
add_executable(game "src/main.cpp" ${GLAD_EXT} ${GAME_SOURCES})

target_link_libraries(test_game  gtestd BulletCollision_vs2010_x64_debug BulletDynamics_vs2010_x64_debug LinearMath_vs2010_x64_debug peon)

if(PEON_OS_WINDOWS AND PEON_COMPILER_MSVC)
	target_link_libraries(game BulletCollision_vs2010_x64_debug BulletDynamics_vs2010_x64_debug LinearMath_vs2010_x64_debug peon)

	set(GAME_OUTPUT_DIR ${GAME_OUTPUT_DIR}/win/msvc/${ARCH_BITS})	

	SetTargetOutputLocations(game GAME_OUTPUT_DIR)
	SetTargetOutputLocations(test_game GAME_OUTPUT_DIR)

	string(REPLACE "/" "\\" GAME_RES_LINK_PATH ${GAME_OUTPUT_DIR}/${PEON_BUILD_TYPE}/res)

	string(REPLACE "/" "\\" GAME_RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res")

	add_custom_command(TARGET game POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/extlib/msvc/${ARCH_BITS}/${PEON_BUILD_TYPE}/assimp-vc130-mtd.dll ${GAME_OUTPUT_DIR}/${PEON_BUILD_TYPE})
	add_custom_command(TARGET game POST_BUILD COMMAND if not exist ${GAME_RES_LINK_PATH} MKLINK /d ${GAME_RES_LINK_PATH} ${GAME_RES_PATH})
else()
	target_link_libraries(game peon)
endif()

source_group("src" FILES
	${GLAD_EXT}
	"src/main.cpp"
	"src/Entity.h"
	"src/Entity.cpp"
	"src/Pool.h"
	"src/Pool.cpp"
	"src/Component.h"
	"src/Component.cpp"
	"src/Scene.h"
	"src/Atlas.h"
	"src/Loaders.h"
	"src/Scene.cpp"
	"src/SceneView.h"
	"src/physics/CollisionShape.h"
	"src/physics/MotionState.h"
	"src/physics/MotionState.cpp"
	"src/physics/PhysicsSystem.cpp"
	"src/physics/PhysicsSystem.h"
	"src/physics/RigidBody.h"
	"src/physics/RigidBody.cpp"
) 

source_group("test" FILES 
	${GLAD_EXT}
	"test/test_pool.cpp"
)
