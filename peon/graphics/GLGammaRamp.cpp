#include "GLGammaRamp.h"

Peon::GLGammaRamp::GLGammaRamp() {

}

Peon::GLGammaRamp::GLGammaRamp(const vector<unsigned short> & r,
    const vector<unsigned short> & g,
    const vector<unsigned short> & b) {
    this->SetValues(r, g, b);
}

Peon::GLGammaRamp::~GLGammaRamp() {

}

void Peon::GLGammaRamp::SetValues(const vector<unsigned short> & r,
    const vector<unsigned short> & g,
    const vector<unsigned short> & b) {
    if (r.size() != g.size() || r.size() != b.size()) {
        LOG_ERROR("Gamma ramp did not have equal number of values for each color channel.");
        return;
    }
    mRed = r;
    mGreen = g;
    mBlue = b;
}

Peon::GLGammaRamp::GLGammaRamp(const GLFWgammaramp* gammaRamp) {
    mRed.resize(gammaRamp->size);
    mGreen.resize(gammaRamp->size);
    mBlue.resize(gammaRamp->size);

    memcpy(static_cast<void*>(&mRed[0]), static_cast<void*>(gammaRamp->red), gammaRamp->size);
    memcpy(static_cast<void*>(&mGreen[0]), static_cast<void*>(gammaRamp->green), gammaRamp->size);
    memcpy(static_cast<void*>(&mBlue[0]), static_cast<void*>(gammaRamp->blue), gammaRamp->size);
}