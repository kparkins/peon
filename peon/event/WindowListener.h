#ifndef PEON_WINDOW_LISTENER_H
#define PEON_WINDOW_LISTENER_H

#include "event/WindowEvent.h"
#include "event/EventListener.h"

namespace Peon {

    
    class WindowListener : public EventListener<WindowEvent> {
        
    public:

        virtual void WindowResized();
        virtual void WindowMoved();
        virtual void WindowDamaged();
        virtual void WindowClosed();
        virtual void WindowFocused();
        virtual void WindowMinimized();
        virtual void WindowFramebufferChanged();

    };

}

#endif