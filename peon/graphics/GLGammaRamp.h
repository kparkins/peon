#ifndef PEON_GL_GAMMA_RAMP_H
#define PEON_GL_GAMMA_RAMP_H

#include <array>

#include <GLFW/glfw3.h>

#include "log/Logger.h"

using std::array;

#define PEON_GL_GAMMA_RAMP_SIZE 256

namespace Peon {
    class GLGammaRamp {

    public:

        GLGammaRamp();
        GLGammaRamp(const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & r,
                    const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & g,
                    const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & b);
        ~GLGammaRamp();

    protected:

        void SetValues(const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & r,
                       const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & g,
                       const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & b);

        GLGammaRamp(const GLFWgammaramp* gammaRamp);

        array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> mRed;
        array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> mGreen;
        array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> mBlue;

        friend class GLDisplayDevice;

    };
}

#endif // PEON_GL_GAMMA_RAMP_H