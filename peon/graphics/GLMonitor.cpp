//
// Created by Kyle on 5/11/2016.
//

#include "GLMonitor.h"

Peon::GLMonitor::GLMonitor(GLFWmonitor* monitor) : mMonitor(monitor) {

}

Peon::GLMonitor::~GLMonitor() {

}

void Peon::GLMonitor::SetGamma(float gamma) {
    glfwSetGamma(mMonitor, gamma);
}

void Peon::GLMonitor::SetGammaRamp(GLGammaRamp ramp) {
    GLFWgammaramp r;
    r.size = ramp.size;
    r.red = ramp.red;
    r.green = ramp.green;
    r.blue = ramp.blue;
    glfwSetGammaRamp(mMonitor, &r);
}

string Peon::GLMonitor::GetName() const {
    return string(glfwGetMonitorName(mMonitor));
}

ivec2 Peon::GLMonitor::GetPosition() const {
    ivec2 position;
    glfwGetMonitorPos(mMonitor, &position.x, &position.y);
    return position;
}

ivec2 Peon::GLMonitor::GetPhysicalSize() const {
    ivec2 size;
    glfwGetMonitorPhysicalSize(mMonitor, &size.x, &size.y);
    return size;
}

Peon::GLGammaRamp Peon::GLMonitor::GetGammaRamp() const {
    GLGammaRamp ramp;
    const GLFWgammaramp* r = glfwGetGammaRamp(mMonitor);
    ramp.size = r->size;
    ramp.red = r->red;
    ramp.green = r->green;
    ramp.blue = r->blue;
    return ramp;
}

Peon::GLVideoMode Peon::GLMonitor::GetVideoMode() const {
    const GLFWvidmode* mode = glfwGetVideoMode(mMonitor);
    return GLVideoMode(mode->width, mode->height, mode->refreshRate);
}

vector<Peon::GLVideoMode> Peon::GLMonitor::GetVideoModes() const {
    int size;
    vector<GLVideoMode> modes;
    const GLFWvidmode* videoMode = glfwGetVideoModes(mMonitor, &size);
    for (int i = 0; i < size; ++i, ++videoMode) {
        modes.push_back(GLVideoMode(videoMode->width, videoMode->height, videoMode->refreshRate));
    }
    return modes;
}

bool Peon::GLMonitor::IsConnected() const {
    return mMonitor != nullptr;
}

bool Peon::GLMonitor::operator==(const GLMonitor & other) {
    return mMonitor == other.mMonitor;
}

Peon::GLMonitor Peon::GLMonitor::GetPrimaryMonitor() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        LOG_ERROR("Unable to find primary monitor.");
        return GLMonitor(nullptr);
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

