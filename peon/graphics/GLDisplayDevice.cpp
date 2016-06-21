//
// Created by Kyle on 5/11/2016.
//

#include "GLDisplayDevice.h"

Peon::GLDisplayDevice::GLDisplayDevice(GLFWmonitor* monitor)
    : mMonitor(monitor),
    mVideoMode(GLVideoMode(glfwGetVideoMode(monitor))) {
    int size;
    const GLFWvidmode* videoMode = glfwGetVideoModes(mMonitor, &size);
    for (int i = 0; i < size; ++i, ++videoMode) {
        mModes.push_back(GLVideoMode(videoMode));
    }
    glfwGetMonitorPhysicalSize(mMonitor, &mWidth, &mHeight);
    glfwGetMonitorPos(mMonitor, &mVirtualPosition.x, &mVirtualPosition.y);
    mName = string(glfwGetMonitorName(mMonitor));
    mGammaRamp = GLGammaRamp(glfwGetGammaRamp(mMonitor));
}

Peon::GLDisplayDevice::~GLDisplayDevice() {

}

bool Peon::GLDisplayDevice::IsConnected() const {
    return mMonitor != nullptr;
}

const Peon::GLVideoMode & Peon::GLDisplayDevice::GetVideoMode() {
    mVideoMode = GLVideoMode(glfwGetVideoMode(mMonitor));
    return mVideoMode;
}


int Peon::GLDisplayDevice::GetPhysicalHeight() const {
    return mHeight;
}

int Peon::GLDisplayDevice::GetPhysicalWidth() const {
    return mWidth;
}

string Peon::GLDisplayDevice::GetName() const {
    return mName;
}

ivec2 Peon::GLDisplayDevice::GetVirtualPosition() const {
    return mVirtualPosition;
}

const Peon::GLGammaRamp & Peon::GLDisplayDevice::GetGammaRamp() {
    const GLFWgammaramp* ramp = glfwGetGammaRamp(mMonitor);
    if (ramp == nullptr) {
        return mGammaRamp;
    }

    auto & red = mGammaRamp.mRed;
    auto & green = mGammaRamp.mGreen;
    auto & blue = mGammaRamp.mBlue;

    red.resize(ramp->size);
    green.resize(ramp->size);
    blue.resize(ramp->size);

    std::memcpy(static_cast<void*>(&red[0]), static_cast<void*>(ramp->red), ramp->size);
    std::memcpy(static_cast<void*>(&green[0]), static_cast<void*>(ramp->green), ramp->size);
    std::memcpy(static_cast<void*>(&blue[0]), static_cast<void*>(ramp->blue), ramp->size);

    return mGammaRamp;
}

void Peon::GLDisplayDevice::SetGamma(float gamma) {
    glfwSetGamma(mMonitor, gamma);
}

void Peon::GLDisplayDevice::SetGammaRamp(Peon::GLGammaRamp gammaRamp) {
    GLFWgammaramp ramp;
    ramp.size = static_cast<unsigned int>(gammaRamp.mRed.size());
    ramp.red = &gammaRamp.mRed[0];
    ramp.green = &gammaRamp.mGreen[0];
    ramp.blue = &gammaRamp.mBlue[0];
    glfwSetGammaRamp(mMonitor, &ramp);
    mGammaRamp = gammaRamp;
}

vector<Peon::GLVideoMode> Peon::GLDisplayDevice::GetSupportedVideoModes() const {
    return mModes;
}

bool Peon::GLDisplayDevice::operator==(const GLDisplayDevice & other) {
    return mMonitor == other.mMonitor;
}