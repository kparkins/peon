#ifndef PEON_GL_GAMMA_RAMP_H
#define PEON_GL_GAMMA_RAMP_H

#include <vector>

#include <GLFW/glfw3.h>

#include "log/Logger.h"

using std::vector;

namespace Peon {
    class GLGammaRamp {

    public:

        GLGammaRamp();
        GLGammaRamp(const vector<unsigned short> & r,
                    const vector<unsigned short> & g,
                    const vector<unsigned short> & b);
        ~GLGammaRamp();
  
        void SetValues(const vector<unsigned short> & r,
			           const vector<unsigned short> & g,
			           const vector<unsigned short> & b);
	protected:

        GLGammaRamp(const GLFWgammaramp* gammaRamp);

		vector<unsigned short> mRed;
		vector<unsigned short> mGreen;
		vector<unsigned short> mBlue;

        friend class GLDisplayDevice;

    };
}

#endif // PEON_GL_GAMMA_RAMP_H