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

    XGrabKey (dpy, SKey, AnyModifier, root, True, GrabModeSync, GrabModeSync);
    XGrabKey (dpy, TKey, AnyModifier, root, True, GrabModeSync, GrabModeSync);
    XGrabKey (dpy, CKey, AnyModifier, root, True, GrabModeSync, GrabModeSync);

    qDebug () << "Running keybinder thread";

    XSelectInput (dpy, root, KeyPressMask);
    for(;;)
    {
        XNextEvent (dpy, &e);
        bool passThrough = true;

        if(e.type == KeyPress){
            qDebug () << "Keypress found";
            if(e.xkey.state & ShiftMask && e.xkey.state & Mod4Mask)
            {
                if(e.xkey.keycode == SKey)
                    emit KeybindingActivated (KeybindingThread::ScreenshotStep);
                else if(e.xkey.keycode == TKey)
                    emit KeybindingActivated (KeybindingThread::TextStep);
                else if(e.xkey.keycode == CKey)
                    emit KeybindingActivated (KeybindingThread::ConsoleStep);
                if(e.xkey.keycode == SKey || e.xkey.keycode == TKey || e.xkey.keycode == CKey)
                {
                    passThrough = false;
                    XAllowEvents (dpy, AsyncKeyboard, e.xkey.time);
                }
            }
        }
        if(passThrough)
        {
            XAllowEvents (dpy, ReplayKeyboard, e.xkey.time);
            XFlush (dpy);
        }
    }
}
