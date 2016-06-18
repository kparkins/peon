//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_DEVICE_MANAGER_H
#define PEON_GL_DEVICE_MANAGER_H

#include <mutex>
#include <vector>
#include <functional>
#include <unordered_map>

#include "log/Logger.h"
#include "common/TypeAliases.h"
#include "common/Uncopyable.h"
#include "GLDisplayDevice.h"

using std::mutex;
using std::vector;
using std::lock_guard;
using std::shared_ptr;
using std::unordered_map;
using std::placeholders::_1;
using std::placeholders::_2;

namespace Peon {

    // Note that since GLFW only issues callbacks on the main thread, we do not need to lock any of these
    // methods as long as this object is only used in the main thread
    class GLDisplayDeviceManager : private Uncopyable {

    public:

        static GLDisplayDeviceManager& GetInstance(); 
        Shared<GLDisplayDevice> GetPrimaryDisplayDevice();
        vector<Shared<GLDisplayDevice>> GetDisplayDevices();
        void SetDeviceDisonnectCallback(const function<void(Shared<GLDisplayDevice>)> & callback);
  
    private:

        GLDisplayDeviceManager();
        ~GLDisplayDeviceManager();

        void DetectDevices();
        static void OnDeviceConfigurationChange(GLFWmonitor* monitor, int event);

        function<void(Shared<GLDisplayDevice>)> mDisconnectCallback;
        unordered_map<GLFWmonitor*, Shared<GLDisplayDevice>> mKnownDevices;

    };
}

#endif //PEON_GL_DEVICE_MANAGER_H
