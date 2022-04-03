/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

//#include <vld.h>

#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Peon.h"

#include "graphics/GLProgram.h"
#include "graphics/GLContext.h"
#include "graphics/GLWindow.h"
#include "log/StdoutStream.h"
#include "profile/BlockTimer.h"
#include "event/EventDispatcher.h"
#include "event/Event.h"
#include "profile/RecordKeeper.h"

using std::cout;
using std::endl;
using std::cerr;
using std::unordered_map;

using namespace Peon;
using namespace glm;

static float triangleVertices[18] = {
        0.5f, -0.5f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.5f, 0.f, 0.f, 1.f, 0.f,
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f
};

template<int N, typename Type, unsigned int GL_TYPE>
struct VertexAttribute {
    static const int GLtype = GL_TYPE;
    static const int NumElements = N;
    Type type[N];
};

typedef VertexAttribute<4, GLdouble, GL_DOUBLE> Attribute4d;
typedef VertexAttribute<3, GLdouble, GL_DOUBLE> Attribute3d;
typedef VertexAttribute<2, GLdouble, GL_DOUBLE> Attribute2d;

typedef VertexAttribute<4, GLfloat, GL_FLOAT> Attribute4f;
typedef VertexAttribute<3, GLfloat, GL_FLOAT> Attribute3f;
typedef VertexAttribute<2, GLfloat, GL_FLOAT> Attribute2f;

template<typename T>
inline int CalculateStride(T attribute) {
    return sizeof(attribute);
}

template<typename T, typename... Attributes>
inline int CalculateStride(T first, Attributes&&... attributes) {
    return sizeof(first) + CalculateStride(attributes...);
}

template<typename T>
inline void MapAttribPointers(int index, int stride, uint64_t offset, T first) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, T::NumElements, T::GLtype, GL_FALSE, stride, reinterpret_cast<GLvoid*>(offset));
}

template<typename T, typename... Attributes>
inline void MapAttribPointers(int index, int stride, uint64_t offset, T first, Attributes&&... attributes) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, T::NumElements, T::GLtype, GL_FALSE, stride, reinterpret_cast<GLvoid*>(offset));
    MapAttribPointers(index + 1, stride, offset + sizeof(T), attributes...);
}

template<typename... Attributes>
void CreateVertexBuffer(int size, const void* data, Attributes&&... attributes) {
    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    
    int stride = CalculateStride(attributes...);
    MapAttribPointers(0, stride, 0, attributes...);

    glBindVertexArray(0);
}
namespace Peon {
    
    class GLBuffer : private Uncopyable {

    public:

        enum Usage : GLenum {
            STREAM_DRAW = GL_STREAM_DRAW,
            STREAM_READ = GL_STREAM_READ,
            STREAM_COPY = GL_STREAM_COPY,
            STATIC_DRAW = GL_STATIC_DRAW,
            STATIC_READ = GL_STATIC_READ,
            STATIC_COPY = GL_STATIC_COPY,
            DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
            DYNAMIC_READ = GL_DYNAMIC_READ,
            DYNAMIC_COPY = GL_DYNAMIC_COPY
        };

        enum Target : GLenum {
            ARRAY = GL_ARRAY_BUFFER,
            COPY_READ = GL_COPY_READ_BUFFER,
            COPY_WRITE = GL_COPY_WRITE_BUFFER,
            DRAW_INDIRECT = GL_DRAW_INDIRECT_BUFFER,
            ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER,
            PIXEL_PACK = GL_PIXEL_PACK_BUFFER,
            PIXEL_UNPACK = GL_PIXEL_UNPACK_BUFFER,
            TEXTURE = GL_TEXTURE_BUFFER,
            TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER
        };

        enum class Access : GLenum {
            READ = GL_READ_ONLY,
            WRITE = GL_WRITE_ONLY,
            READ_WRITE = GL_READ_WRITE
        };


        GLBuffer() {
            mSize = 0;
            mMap = nullptr;
            glGenBuffers(1, &mId);
        }

        ~GLBuffer() {
            glDeleteBuffers(1, &mId);
        }

        void Bind(Target target, unsigned int index) {
            glBindBufferBase(static_cast<GLenum>(target), index, mId);
        }

        void Bind(Target target, unsigned int index, GLintptr offset, GLsizei size) {
            glBindBufferRange(static_cast<GLenum>(target), index, mId, offset, size);

        }

        unsigned int GetSize() {
            return mSize;
        }

        void SetData(void* data, unsigned int size) {
            if (mSize) {
                glDeleteBuffers(1, &mId);
                glGenBuffers(1, &mId);
            }
            mSize = size; 
            glBindBuffer(mTarget, mId);
            glBufferData(mTarget, size, data, mUsage);
        }

        void GetSubData() {

        }

        void SetSubData() {

        }

        void* Map(GLenum access) {
            assert(!mMap);
            mMap = glMapBuffer(mTarget, access);
            return mMap;
        }

        void Unmap() {
            assert(mMap);
            glUnmapBuffer(mTarget);
        }

        void* MapRange(GLintptr offset, GLsizeiptr length, GLbitfield access) {
            assert(!mMap);
            mMap = glMapBufferRange(mTarget, offset, length, access);
            return mMap;
        }



    protected:

        GLuint mId;
        GLenum mTarget;
        GLenum mUsage;
        unsigned int mSize;
        void* mMap;

    };
}


class Game {

public:

    Game() {

    }

    ~Game() {
        delete mWindow;
        delete mProgram;
        glDeleteBuffers(1, &mVbo);
        glDeleteVertexArrays(1, &mVao);
    }

    void Initialize() {
        PEON_INITIALIZE;
  
        gLogger.AddStream(MakeUnique<StdoutStream>());
        gLogger.SetLogLevel(LogLevel::TRACE);

        GLContextSettings ctxSettings;
        ctxSettings.forwardCompatible = false;
        ctxSettings.profile = PEON_OPENGL_CORE_PROFILE;
        ctxSettings.versionMajor = 4;
        ctxSettings.versionMinor = 0;
        
        GLVideoMode videoMode(640, 480);
        mWindow = new GLWindow(videoMode, ctxSettings);

        CreateVertexBuffer(12, nullptr, Attribute3f(), Attribute2f());

        mProgram = new GLProgram();
        mProgram->AddShader(GL_VERTEX_SHADER, "res/shaders/PassThrough.vert");
        mProgram->AddShader(GL_FRAGMENT_SHADER, "res/shaders/PassThrough.frag");
        mProgram->LinkProgram();
        
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile("res/models/mosesly4.3ds", aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_GenUVCoords);
        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LOG_ERROR("Could not load model.");
        }

        glGenBuffers(1, &mVbo);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);

        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(0));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<GLvoid*>(sizeof(float) * 3));

        glBindVertexArray(0);

        ivec2 size = mWindow->GetFramebufferSize();
        glViewport(0, 0, size.x, size.y);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glClearColor(0.f, 0.f, 0.f, 0.f);
    }

    void Run() {

        while (mWindow->IsOpen()) {
            glClear(GL_COLOR_BUFFER_BIT);
            mProgram->Enable();
            glBindVertexArray(mVao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            mProgram->Disable();
            mWindow->SwapBuffers();
        }

    }

private:

    GLuint mVao;
    GLuint mVbo;
    
    GLProgram* mProgram;
    GLWindow* mWindow;

};


int main(int argc, char* argv[]) {
    Game g;
    g.Initialize();
    g.Run();

    return 0;
}


