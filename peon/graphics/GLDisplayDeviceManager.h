//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_DEVICE_MANAGER_H
#define PEON_GL_DEVICE_MANAGER_H

#include <mutex>
#include <vector>
#include <unordered_map>

#include "log/Logger.h"
#include "GLDisplayDevice.h"

using std::mutex;
using std::vector;
using std::lock_guard;
using std::unordered_map;

namespace Peon {

    // Note that since GLFW only issues callbacks on the main thread, we do not need to lock any of these
    // methods as long as this object is only used in the main thread
    class GLDisplayDeviceManager {

    public:

        GLDisplayDeviceManager();
        ~GLDisplayDeviceManager();

        GLDisplayDevice* GetPrimaryDisplayDevice();
        vector<GLDisplayDevice*> GetDisplayDevices();

    protected:

        void DetectDevices();
        static void OnDeviceConfigurationChange(GLFWmonitor* monitor, int event);

        static int mNumInstances;
        static unordered_map<GLFWmonitor*, GLDisplayDevice*> mKnownDevices;

        friend class GLDisplayDevice;
    };
}

#endif //PEON_GL_DEVICE_MANAGER_H
