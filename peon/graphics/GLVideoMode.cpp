//
// Created by Kyle on 5/12/2016.
//

#include "GLVideoMode.h"

Peon::GLVideoMode::GLVideoMode()
    : mRedBits(0),
    mGreenBits(0),
    mBlueBits(0),
    mRefreshRate(0),
    mWidth(0),
    mHeight(0) 
{}

Peon::GLVideoMode::GLVideoMode(const GLFWvidmode* videoMode) {
    this->SetValues(videoMode);
}

Peon::GLVideoMode::~GLVideoMode() {

}

void Peon::GLVideoMode::SetValues(const GLFWvidmode* videoMode) {
    assert(videoMode);
    mRedBits = static_cast<unsigned int>(videoMode->redBits);
    mGreenBits = static_cast<unsigned int>(videoMode->greenBits);
    mBlueBits = static_cast<unsigned int>(videoMode->blueBits);
    mRefreshRate = static_cast<unsigned int>(videoMode->refreshRate);
    mWidth = static_cast<unsigned int>(videoMode->width);
    mHeight = static_cast<unsigned int>(videoMode->height);
}

unsigned int Peon::GLVideoMode::GetRedBitDepth() const {
    return mRedBits;
}

unsigned int Peon::GLVideoMode::GetGreenBitDepth() const {
    return mGreenBits;
}

unsigned int Peon::GLVideoMode::GetBlueBitDepth() const {
    return mBlueBits;
}

unsigned int Peon::GLVideoMode::GetRefreshRate() const {
    return mRefreshRate;
}

unsigned int Peon::GLVideoMode::GetWidth() const {
    return mWidth;
}

unsigned int Peon::GLVideoMode::GetHeight() const {
    return mHeight;
}

