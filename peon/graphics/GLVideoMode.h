//
// Created by Kyle on 5/12/2016.
//

#ifndef PEON_GL_VIDEO_MODE_H
#define PEON_GL_VIDEO_MODE_H


#include <GLFW/glfw3.h>
#include <assert.h>

namespace Peon {
    class GLVideoMode {

    public:

        GLVideoMode();
        ~GLVideoMode();

        unsigned int GetRedBitDepth() const;
        unsigned int GetGreenBitDepth() const;
        unsigned int GetBlueBitDepth() const;
        unsigned int GetRefreshRate() const;
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;

    protected:

        explicit GLVideoMode(const GLFWvidmode* videoMode);

        void SetValues(const GLFWvidmode* videoMode);

        unsigned int mRedBits;
        unsigned int mGreenBits;
        unsigned int mBlueBits;
        unsigned int mRefreshRate;
        unsigned int mWidth;
        unsigned int mHeight;

        friend class GLDisplayDevice;
    };
}

#endif //PEON_GLVIDEOMODE_H
