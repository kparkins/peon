//
// Created by Kyle on 5/11/2016.
//

#include "GLDisplayDeviceManager.h"

int Peon::GLDisplayDeviceManager::mNumInstances = 0;
unordered_map<GLFWmonitor*, Peon::GLDisplayDevice*> Peon::GLDisplayDeviceManager::mKnownDevices;

Peon::GLDisplayDeviceManager::GLDisplayDeviceManager() {
    glfwSetMonitorCallback(GLDisplayDeviceManager::OnDeviceConfigurationChange);
    this->DetectDevices();
}

Peon::GLDisplayDeviceManager::~GLDisplayDeviceManager() {
 
}

void Peon::GLDisplayDeviceManager::OnDeviceConfigurationChange(GLFWmonitor *monitor, int event) {
    switch(event) {
        case GLFW_CONNECTED: {
            mKnownDevices[monitor] = new GLDisplayDevice(monitor);
            break;
        }
        case GLFW_DISCONNECTED: {
            if (mKnownDevices.find(monitor) != mKnownDevices.end()) {
                mKnownDevices[monitor]->OnDeviceDisconnect();
                mKnownDevices.erase(monitor);
            }
            break;
        }
        default:
            LOG_ERROR("Unknown monitor configuration change for : " << glfwGetMonitorName(monitor));
            break;
    }
}

Peon::GLDisplayDevice* Peon::GLDisplayDeviceManager::GetPrimaryDisplayDevice() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if(monitor == nullptr) {
        LOG_ERROR("Unable to retrieve the primary monitor.");
        return nullptr;
    }

    if(mKnownDevices.find(monitor) != mKnownDevices.end()) {
        return mKnownDevices[monitor];
    }

    GLDisplayDevice* device = new GLDisplayDevice(monitor);
    mKnownDevices[monitor] = device;
    return device;
}

vector<Peon::GLDisplayDevice*> Peon::GLDisplayDeviceManager::GetDisplayDevices() {
    this->DetectDevices();
    vector<GLDisplayDevice*> devices;
    for(auto pair : mKnownDevices) {
        devices.push_back(pair.second);
    }
    return devices;
}

void Peon::GLDisplayDeviceManager::DetectDevices() {
    int size;
    GLFWmonitor** deviceList = glfwGetMonitors(&size);
    for(int i = 0; i < size; ++i, ++deviceList) {
        if(mKnownDevices.find(*deviceList) == mKnownDevices.end()) {
            mKnownDevices[*deviceList] = new GLDisplayDevice(*deviceList);
        }
    }
}