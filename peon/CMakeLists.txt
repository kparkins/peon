cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include_directories(
	"." 
	"${PROJECT_SOURCE_DIR}/extlib/include"
	"${PROJECT_SOURCE_DIR}/extlib/include/bullet"
)

file(GLOB_RECURSE PEON_SOURCES *.cpp *.inl)
file(GLOB_RECURSE PEON_HEADERS *.h)

set(PEON_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin")

add_library(peon STATIC ${GLAD_EXT} ${PEON_SOURCES} ${PEON_HEADERS})

find_package(OpenGL REQUIRED)

if(PEON_OS_WINDOWS AND PEON_COMPILER_MSVC)
	set(PEON_OUTPUT_DIR ${PEON_OUTPUT_DIR}/win/msvc/${ARCH_BITS})
	SetTargetOutputLocations(peon PEON_OUTPUT_DIR)
	target_link_libraries(peon ${OPENGL_LIBRARIES} glfw3 assimp BulletCollision_vs2010_x64_debug BulletDynamics_vs2010_x64_debug LinearMath_vs2010_x64_debug)
else()
	list(APPEND CMAKE_PREFIX_PATH /usr/local/lib)
	find_library(GLFW_LIB glfw)
	find_library(ASSIMP_LIB assimp)
	find_library(BULLET_COLLISION_LIB BulletCollision)
	find_library(BULLET_DYNAMICS_LIB BulletDynamics)
	find_library(LINEAR_MATH_LIB LinearMath)
	target_link_libraries(peon  ${OPENGL_LIBRARIES} ${GLFW_LIB} ${ASSIMP_LIB} ${BULLET_COLLISION_LIB} ${BULLET_DYNAMICS_LIB} ${LINEAR_MATH_LIB})
endif()


source_group("" FILES 
	"Peon.h"
	"Peon.cpp"
	${GLAD_EXT}
)


source_group("common" FILES
	"common/Grid.h"
	"common/Grid.cpp"
	"common/Macros.h"
	"common/Utility.h"
	"common/Utility.cpp"
	"common/TypeAliases.h"
	"common/Uncopyable.h"
	"common/CompileFlags.h"
	"common/CompileFlags.cpp"
)

source_group("entity" FILES
	"entity/Component.h"
	"entity/Component.cpp"
	"entity/ComponentIterator.h"
	"entity/ComponentPools.h"
	"entity/ComponentTypes.h"
	"entity/ComponentView.h"
	"entity/EcsTypes.h"
	"entity/EntityIterator.h"
	"entity/EntityIterator.cpp"
	"entity/EntityView.h"
	"entity/EntityView.cpp"
	"entity/Pool.h"
	"entity/Pool.cpp"
	"entity/PoolIterator.h"
	"entity/Scene.h"
	"entity/Scene.cpp"
)

source_group("event" FILES
	"event/Event.h"
	"event/Event.cpp"
	"event/Bus.h"
	"event/Bus.cpp"
	"event/Signal.h"
	"event/KeyEvent.h"
	"event/MouseEvent.h"
)

source_group("graphics" FILES
	"graphics/Atlas.h"
	"graphics/BPLightMapPass.h"
	"graphics/BPTexturedPass.h"
	"graphics/StaticColorPass.h"
	"graphics/Loaders.h"
	"graphics/Loaders.cpp"
	"graphics/Renderable.h"
	"graphics/Camera.h"
	"graphics/Plane.h"
	"graphics/Plane.cpp"
	"graphics/Sphere.h"
	"graphics/Sphere.cpp"
	"graphics/opengl/GLMonitor.h"
	"graphics/opengl/GLContext.h"
	"graphics/opengl/GLWindow.h"
	"graphics/opengl/GLViewport.h"
	"graphics/opengl/GLViewport.cpp"
	"graphics/opengl/GLVideoMode.h"
	"graphics/opengl/GLRenderer.h"
	"graphics/opengl/GLRenderer.cpp"
	"graphics/opengl/GLMonitor.cpp"
	"graphics/opengl/GLShader.h"
	"graphics/opengl/GLShader.cpp"
	"graphics/opengl/GLProgram.h"
	"graphics/opengl/GLProgram.cpp"
	"graphics/opengl/GLTexture2D.h"
	"graphics/opengl/GLTexture2D.cpp"
	"graphics/opengl/GLContext.cpp"
	"graphics/opengl/GLWindow.cpp"
	"graphics/opengl/GLVideoMode.cpp"
	"graphics/opengl/GLVertexBuffer.h"
	"graphics/opengl/GLVertexBuffer.cpp"
	"graphics/opengl/GLVertexArray.h"
	"graphics/opengl/GLVertexArray.cpp"
	"graphics/opengl/GLWindowOpts.h"
	"graphics/opengl/GLWindowOpts.cpp"
	"graphics/opengl/GLContextOpts.h"
	"graphics/opengl/GLContextOpts.cpp"
	"graphics/opengl/GraphicsExtensions.h"
)

source_group("input" FILES
	"input/Key.h"
	"input/FreelookController.cpp"
	"input/FreelookController.h"
)

source_group("log" FILES
	"log/Logfile.h"
	"log/Logger.h"
	"log/RotatingLogfile.h"
	"log/StdoutStream.h"
	"log/LogStream.h"
	"log/Logfile.cpp"
	"log/Logger.cpp"
	"log/RotatingLogfile.cpp"
	"log/StdoutStream.cpp"
)

source_group("physics" FILES
	"physics/CollisionShape.h"
	"physics/MotionState.cpp"
	"physics/MotionState.h"
	"physics/PhysicsSystem.cpp"
	"physics/PhysicsSystem.h"
	"physics/RigidBody.cpp"
	"physics/RigidBody.h"
)

source_group("profile" FILES 
	"profile/BlockTimer.h"
	"profile/BlockTimer.cpp"	
	"profile/Timer.h"
	"profile/Timer.cpp"
	"profile/TimeRecord.h"
	"profile/TimeRecord.cpp"
	"profile/RecordKeeper.h"
	"profile/RecordKeeper.cpp"
)
