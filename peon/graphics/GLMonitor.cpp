//
// Created by Kyle on 5/11/2016.
//

#include "GLMonitor.h"

Peon::GLMonitor::GLMonitor(GLFWmonitor* monitor)
    : mMonitor(monitor),
    mVideoMode(GLVideoMode()) {
    int size;
    const GLFWvidmode* videoMode = glfwGetVideoModes(mMonitor, &size);
    for (int i = 0; i < size; ++i, ++videoMode) {
        mModes.push_back(GLVideoMode());
    }
    glfwGetMonitorPhysicalSize(mMonitor, &mWidth, &mHeight);
    glfwGetMonitorPos(mMonitor, &mPosition.x, &mPosition.y);
    mName = string(glfwGetMonitorName(mMonitor));
}

Peon::GLMonitor::~GLMonitor() {

}

bool Peon::GLMonitor::IsConnected() const {
    return mMonitor != nullptr;
}

const Peon::GLVideoMode & Peon::GLMonitor::GetVideoMode() {

    return mVideoMode;
}


int Peon::GLMonitor::GetPhysicalHeight() const {
    return mHeight;
}

int Peon::GLMonitor::GetPhysicalWidth() const {
    return mWidth;
}

string Peon::GLMonitor::GetName() const {
    return mName;
}

ivec2 Peon::GLMonitor::GetPosition() const {
    return mPosition;
}

void Peon::GLMonitor::SetGamma(float gamma) {
    glfwSetGamma(mMonitor, gamma);
}

vector<Peon::GLVideoMode> Peon::GLMonitor::GetVideoModes() const {
    return mModes;
}

bool Peon::GLMonitor::operator==(const GLMonitor & other) {
    return mMonitor == other.mMonitor;
}

Peon::GLMonitor Peon::GLMonitor::GetPrimaryMonitor() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        LOG_ERROR("Unable to find primary monitor.");
        return GLMonitor();
    }
    return GLMonitor(monitor);
}

vector<Peon::GLMonitor> Peon::GLMonitor::GetMonitors() {
    int numMonitors = 0;
    vector<GLMonitor> devices;
    GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);
    for (int i = 0; i < numMonitors; ++i) {
        devices.push_back(GLMonitor(*monitors));
    }
    return devices;
}

