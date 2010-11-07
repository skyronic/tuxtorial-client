#include <QDebug>
#include "keybindingthread.h"
#include <X11/Xlib.h>

KeybindingThread::KeybindingThread()
{

}

void KeybindingThread::run ()
{
    Display *dpy = XOpenDisplay(0);
    XEvent e;
    if(!dpy)
        return;

    Window root = DefaultRootWindow(dpy);

    unsigned int SKey = XKeysymToKeycode (dpy, XStringToKeysym ("S"));
    unsigned int TKey = XKeysymToKeycode (dpy, XStringToKeysym ("T"));
    unsigned int CKey = XKeysymToKeycode (dpy, XStringToKeysym ("V"));

    XGrabKey (dpy, SKey, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey (dpy, TKey, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey (dpy, CKey, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);

    XSelectInput (dpy, root, KeyPressMask);
    for(;;)
    {
        XNextEvent (dpy, &e);

        if(e.type == KeyPress){
            if(e.xkey.state & ShiftMask && e.xkey.state && Mod4Mask)
            {
                if(e.xkey.keycode == SKey)
                    emit KeybindingActivated (KeybindingThread::ScreenshotStep);
                else if(e.xkey.keycode == TKey)
                    emit KeybindingActivated (KeybindingThread::TextStep);
                else if(e.xkey.keycode == CKey)
                    emit KeybindingActivated (KeybindingThread::ConsoleStep);
            }
        }
    }
}
