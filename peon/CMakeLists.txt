cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

include_directories(
	"." 
	"${PROJECT_SOURCE_DIR}/extlib"
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

target_link_libraries(peon ${OPENGL_LIBRARIES} glfw3 assimp freetype2 SOIL)

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
)

source_group("event" FILES
	"event/Event.h"
	"event/EventDispatcher.h"
	"event/EventDispatcher.inl"
	"event/EventListener.h"
	"event/EventListener.inl"
	"event/WindowListener.h"
	"event/WindowListener.cpp"
	"event/WindowEvent.h"
	"event/WindowEvent.cpp"
)

source_group("graphics" FILES
	"graphics/GLMonitor.h"
	"graphics/GLContext.h"
	"graphics/GLWindow.h"
	"graphics/GLSurface.h"
	"graphics/GLSurface.cpp"
	"graphics/GLVideoMode.h"
	"graphics/GLMonitor.cpp"
	"graphics/GLShader.h"
	"graphics/GLShader.cpp"
	"graphics/GLProgram.h"
	"graphics/GLProgram.cpp"
	"graphics/GLTexture.h"
	"graphics/GLTexture.cpp"
	"graphics/GLContext.cpp"
	"graphics/GLWindow.cpp"
	"graphics/GLVideoMode.cpp"
	"graphics/GLVertexBuffer.h"
	"graphics/GLVertexBuffer.cpp"
	"graphics/GLWindowOpts.h"
	"graphics/GLWindowOpts.cpp"
	"graphics/GLContextOpts.h"
	"graphics/GLContextOpts.cpp"
	"graphics/GraphicsExtensions.h"
)

source_group("input" FILES
	"input/Keyboard.h"
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
