cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

include_directories(
	"." 
	"${PROJECT_SOURCE_DIR}/extlib/inc"
)

file(GLOB_RECURSE PEON_SOURCES *.cpp)
file(GLOB_RECURSE PEON_HEADERS *.h)

set(PEON_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin")

add_library(peon STATIC ${PEON_SOURCES} ${PEON_HEADERS})

find_package(OpenGL REQUIRED)

if(PEON_OS_WINDOWS AND PEON_COMPILER_MSVC)
	set(PEON_OUTPUT_DIR ${PEON_OUTPUT_DIR}/win/msvc/${ARCH_BITS})
	SetTargetOutputLocations(peon PEON_OUTPUT_DIR)
endif()

target_link_libraries(peon  ${OPENGL_LIBRARIES} glfw3 glew32s assimp freetype2 SOIL)

source_group("" FILES 
	"CompileFlags.h"
	"Peon.h"
	"Peon.cpp"
)

source_group("common" FILES
	"common/Grid.h"
	"common/Grid.cpp"
	"common/Macros.h"
	"common/Utility.h"
	"common/Utility.cpp"
)

source_group("graphics" FILES
	"graphics/GLDisplayDevice.h"
	"graphics/GLDisplayDeviceManager.h"
	"graphics/GLGammaRamp.h"
	"graphics/GLProgram.h"
	"graphics/GLRenderContext.h"
	"graphics/GLRenderWindow.h"
	"graphics/GLShader.h"
	"graphics/GLVideoMode.h"
	"graphics/GLDisplayDevice.cpp"
	"graphics/GLDisplayDeviceManager.cpp"
	"graphics/GLGammaRamp.cpp"
	"graphics/GLProgram.cpp"
	"graphics/GLRenderContext.cpp"
	"graphics/GLRenderWindow.cpp"
	"graphics/GLShader.cpp"
	"graphics/GLVideoMode.cpp"
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

source_group("profile" FILES "")
