#ifndef KEYBINDINGTHREAD_H
#define KEYBINDINGTHREAD_H

#include <QThread>

class KeybindingThread : public QThread
{
    Q_OBJECT
public:
    KeybindingThread();
    void run ();
    enum StepType
    {
        ScreenshotStep,
        ConsoleStep,
        TextStep,
        FinishStep,
    };
signals:
    void KeybindingActivated(int type);
};

#endif // KEYBINDINGTHREAD_H
