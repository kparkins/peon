//
// Created by Kyle on 5/11/2016.
//

#ifndef PEON_GL_DEVICE_H
#define PEON_GL_DEVICE_H


#include <mutex>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_set>

#include <glm/glm.hpp>

#include "log/Logger.h"
#include "common/Macros.h"

#include "GLVideoMode.h"

using std::mutex;
using std::string;
using std::function;
using std::lock_guard;
using std::unordered_set;

using glm::ivec2;

namespace Peon {

    typedef struct GLGammaRamp {
        int size;
        unsigned short* red;
        unsigned short* green;
        unsigned short* blue;
    }GLGammaRamp;

    class GLMonitor {

    public:

        ~GLMonitor();

        void SetGamma(float gamma);
        void SetGammaRamp(GLGammaRamp ramp);

        string GetName() const;
        ivec2 GetPosition() const;
        ivec2 GetPhysicalSize() const;
        GLGammaRamp GetGammaRamp() const;
        GLVideoMode GetVideoMode() const;
        vector<GLVideoMode> GetVideoModes() const;
     
        bool IsConnected() const;

        bool operator==(const GLMonitor & other);

        static GLMonitor GetPrimaryMonitor();
        static vector<GLMonitor> GetMonitors();

    private:

        explicit GLMonitor(GLFWmonitor*  monitor);

        GLFWmonitor* mMonitor;
        
        friend class GLWindow;

    };

}


#endif //PEON_GL_DEVICE_H
