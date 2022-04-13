cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include_directories(
	"." 
	"${PROJECT_SOURCE_DIR}/extlib/include"
)

file(GLOB_RECURSE PEON_SOURCES *.cpp *.inl)
file(GLOB_RECURSE PEON_HEADERS *.h)

set(PEON_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin")

add_library(peon STATIC ${GLAD_EXT} ${PEON_SOURCES} ${PEON_HEADERS})

find_package(OpenGL REQUIRED)

if(PEON_OS_WINDOWS AND PEON_COMPILER_MSVC)
	set(PEON_OUTPUT_DIR ${PEON_OUTPUT_DIR}/win/msvc/${ARCH_BITS})
	SetTargetOutputLocations(peon PEON_OUTPUT_DIR)
endif()

target_link_libraries(peon ${OPENGL_LIBRARIES} glfw3 assimp)

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

source_group("event" FILES
	"event/Event.h"
	"event/EventDispatcher.h"
	"event/EventDispatcher.inl"
	"event/EventListener.h"
	"event/WindowListener.h"
	"event/KeyEventListener.h"
)

source_group("graphics" FILES
	"graphics/Camera.h"
	"graphics/FreeLookCamera.h"
	"graphics/Sphere.h"
	"graphics/Sphere.cpp"
	"graphics/opengl/GLMonitor.h"
	"graphics/opengl/GLContext.h"
	"graphics/opengl/GLWindow.h"
	"graphics/opengl/GLSurface.h"
	"graphics/opengl/GLSurface.cpp"
	"graphics/opengl/GLVideoMode.h"
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
