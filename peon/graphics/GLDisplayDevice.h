//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_DEVICE_H
#define PEON_GL_DEVICE_H

#include <GLFW/glfw3.h>

#include <mutex>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_set>

#include "common/Macros.h"
#include <glm/glm.hpp>

#include "log/Logger.h"
#include "GLVideoMode.h"
#include "GLGammaRamp.h"

using std::mutex;
using std::string;
using std::function;
using std::lock_guard;
using std::unordered_set;

using glm::ivec2;

namespace Peon {

    class GLDisplayDevice {

    public:

        ~GLDisplayDevice();

        string GetName();
        bool IsConnected();
        int GetWidth();
        int GetHeight();
        ivec2 GetVirtualPosition();
        GLVideoMode GetVideoMode();
        GLGammaRamp GetGammaRamp();
        void SetGamma(float gamma);
        void SetGammaRamp(GLGammaRamp gammaRamp);
        vector<GLVideoMode> GetSupportedVideoModes();
       
    private:

        GLDisplayDevice(GLFWmonitor*  monitor);

        void OnDeviceDisconnect();
        void DetectVideoModes();

        int mWidth;
        int mHeight;
        string mName;
        GLFWmonitor* mMonitor;
        ivec2 mVirtualPosition;
        GLGammaRamp mGammaRamp;
        GLVideoMode mVideoMode;
        vector<GLVideoMode> mModes;
        
        friend class GLDisplayDeviceManager;

    };

}


#endif //PEON_GL_DEVICE_H
