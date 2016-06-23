#ifndef PEON_GL_WINDOW_SETTINGS_H
#define PEON_GL_WINDOW_SETTINGS_H

#include <string>

#include "Peon.h"

#include "GLMonitor.h"

using std::string;

namespace Peon {
    class GLWindowSettings {
    public:

        void ApplySettings() const;

        bool resizeable = true;
        bool visible = true;
        bool decorated = true;
        bool focused = true;
        bool autoIconify = true;
        bool floating = false;
        bool maximized = false;;
        string title = "Default";

    };
}
#endif