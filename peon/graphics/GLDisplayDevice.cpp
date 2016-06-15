//
// Created by Kyle on 5/11/2016.
//

#include "GLDisplayDevice.h"
#include "GLDisplayDeviceManager.h"

Peon::GLDisplayDevice::GLDisplayDevice(GLFWmonitor* monitor)
        : mMonitor(monitor),
          mCurrentMode(new GLVideoMode())
{
    int size;
    const GLFWvidmode* videoMode = glfwGetVideoModes(mMonitor, &size);
    for(int i = 0; i < size; ++i, ++videoMode) {
       mModes.push_back(Peon::GLVideoMode(videoMode));
    }
    glfwGetMonitorPhysicalSize(mMonitor, &mWidth, &mHeight);
    glfwGetMonitorPos(mMonitor, &mVirtualPosition.x, &mVirtualPosition.y);
    mName = string(glfwGetMonitorName(mMonitor));
    mGammaRamp = GLGammaRamp(glfwGetGammaRamp(mMonitor));
}

Peon::GLDisplayDevice::~GLDisplayDevice() {
    GLDisplayDeviceManager::mKnownDevices.erase(mMonitor);
}

void Peon::GLDisplayDevice::SetDeviceDisconnectCallback(function<void(GLDisplayDevice*)> callback) {
    mDisconnectCallback = callback;
}

bool Peon::GLDisplayDevice::IsConnected() {
    return mMonitor != nullptr;
}

Peon::GLVideoMode Peon::GLDisplayDevice::GetVideoMode() {
    return *mCurrentMode;
}

void Peon::GLDisplayDevice::OnDeviceDisconnect() {
    if(mDisconnectCallback) {
        mDisconnectCallback(this);
    }
}

int Peon::GLDisplayDevice::GetHeight() {
    return mHeight;
}

int Peon::GLDisplayDevice::GetWidth() {
    return mWidth;
}

string Peon::GLDisplayDevice::GetName() {
    return mName;
}

ivec2 Peon::GLDisplayDevice::GetVirtualPosition() {
    return mVirtualPosition;
}

Peon::GLGammaRamp Peon::GLDisplayDevice::GetGammaRamp() {
    const GLFWgammaramp* ramp = glfwGetGammaRamp(mMonitor);
    if(ramp == nullptr) {
        return mGammaRamp;
    }
    auto & red = mGammaRamp.mRed;
    auto & green = mGammaRamp.mGreen;
    auto & blue = mGammaRamp.mBlue;
    for(unsigned int i = 0; i < ramp->size; ++i) {
        red[i] = ramp->red[i];
        green[i] = ramp->green[i];
        blue[i] = ramp->blue[i];
    }
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

vector<Peon::GLVideoMode> Peon::GLDisplayDevice::GetSupportedVideoModes() {
    return mModes;
}
