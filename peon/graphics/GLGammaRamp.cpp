#include "GLGammaRamp.h"

Peon::GLGammaRamp::GLGammaRamp(){

}

Peon::GLGammaRamp::GLGammaRamp(const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & r,
                               const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & g,
                               const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & b)
{
    this->SetValues(r, g, b);
}

Peon::GLGammaRamp::~GLGammaRamp() {

}

void Peon::GLGammaRamp::SetValues(const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & r,
                                  const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & g,
                                  const array<unsigned short, PEON_GL_GAMMA_RAMP_SIZE> & b)
{
    if(r.size() != g.size() || r.size() != b.size()) {
        LOG_ERROR("Gamma ramp did not have equal number of values for each color channel.");
        return;
    }
    mRed = r;
    mGreen = g;
    mBlue = b;
}

Peon::GLGammaRamp::GLGammaRamp(const GLFWgammaramp* gammaRamp) {
    size_t size = gammaRamp->size;
    for(unsigned int i = 0; i < size; ++i) {
        mRed[i] = gammaRamp->red[i];
        mGreen[i] = gammaRamp->green[i];
        mBlue[i] = gammaRamp->blue[i];
    }
}