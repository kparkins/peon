//
// Created by Kyle on 5/11/2016.
//

#include "GLDisplayDeviceManager.h"

Peon::GLDisplayDeviceManager::GLDisplayDeviceManager() {
    glfwSetMonitorCallback(GLDisplayDeviceManager::OnDeviceConfigurationChange);
    this->DetectDevices();
}

Peon::GLDisplayDeviceManager::~GLDisplayDeviceManager() {
    mKnownDevices.clear();
}

void Peon::GLDisplayDeviceManager::OnDeviceConfigurationChange(GLFWmonitor *monitor, int event) {
    auto & deviceManager = GLDisplayDeviceManager::GetInstance();
    auto & knownDevices = deviceManager.mKnownDevices;
    switch (event) {
        case GLFW_CONNECTED: {
            knownDevices[monitor] = Shared<GLDisplayDevice>(new GLDisplayDevice(monitor));
            LOG_INFO(knownDevices[monitor]->GetName() << " connected.");
            break;
        }
        case GLFW_DISCONNECTED: {
            if (knownDevices.find(monitor) != knownDevices.end()) {
                auto & DisconnectCallback = deviceManager.mDisconnectCallback;
                Shared<GLDisplayDevice> & device = knownDevices[monitor];
                if (DisconnectCallback) {
                    DisconnectCallback(device);
                }
                LOG_INFO(device->GetName() << " disconnected.");
                knownDevices.erase(monitor);
            }
            break;
        }
        default: {
            LOG_ERROR("Unknown monitor configuration change for : " << glfwGetMonitorName(monitor));
            break;
        }
    }
}

Peon::GLDisplayDeviceManager& Peon::GLDisplayDeviceManager::GetInstance() {
    static GLDisplayDeviceManager deviceManager;
    return deviceManager;
}

shared_ptr<Peon::GLDisplayDevice> Peon::GLDisplayDeviceManager::GetPrimaryDisplayDevice() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor == nullptr) {
        LOG_ERROR("Unable to retrieve the primary monitor.");
        return nullptr;
    }

    if (mKnownDevices.find(monitor) != mKnownDevices.end()) {
        return mKnownDevices[monitor];
    }

    Shared<GLDisplayDevice> device = Shared<GLDisplayDevice>(new GLDisplayDevice(monitor));
    LOG_INFO(" Found new primary display device - " << device->GetName());
    mKnownDevices[monitor] = device;
    return device;
}

vector<Peon::Shared<Peon::GLDisplayDevice>> Peon::GLDisplayDeviceManager::GetDisplayDevices() {
    this->DetectDevices();
    vector<Shared<GLDisplayDevice>> devices;
    for (auto pair : mKnownDevices) {
        devices.push_back(pair.second);
    }
    return devices;
}

void Peon::GLDisplayDeviceManager::SetDeviceDisonnectCallback(const function<void(Shared<GLDisplayDevice>)> & callback) {
    mDisconnectCallback = callback;
}

void Peon::GLDisplayDeviceManager::DetectDevices() {
    int size;
    GLFWmonitor** deviceList = glfwGetMonitors(&size);
    for (int i = 0; i < size; ++i, ++deviceList) {
        if (mKnownDevices.find(*deviceList) == mKnownDevices.end()) {
            Shared<GLDisplayDevice> device = Shared<GLDisplayDevice>(new GLDisplayDevice(*deviceList));
            LOG_INFO("Found new display device - " << device->GetName());
        }
    }
}