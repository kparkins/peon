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

        string GetName() const;
        bool IsConnected() const;
        int GetPhysicalWidth() const;
        int GetPhysicalHeight() const;
        ivec2 GetVirtualPosition() const;
        const GLVideoMode & GetVideoMode();
        const GLGammaRamp & GetGammaRamp();
        void SetGamma(float gamma);
        void SetGammaRamp(GLGammaRamp gammaRamp);
        vector<GLVideoMode> GetSupportedVideoModes() const;
    
        bool operator==(const GLDisplayDevice & other);

    private:

        explicit GLDisplayDevice(GLFWmonitor*  monitor);

        void DetectVideoModes();

        bool operator==(GLFWmonitor* monitor);

        int mWidth;
        int mHeight;
        string mName;
        GLFWmonitor* mMonitor;
        ivec2 mVirtualPosition;
        GLGammaRamp mGammaRamp;
        GLVideoMode mVideoMode;
        vector<GLVideoMode> mModes;
        
        friend class GLDisplayDeviceManager;
        friend class GLWindow;
    };

}


#endif //PEON_GL_DEVICE_H
