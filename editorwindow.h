#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "keybindingthread.h"
#include <QList>
#include <QDir>
#include "step.h"

namespace Ui {
    class EditorWindow;
}

class QTimer;
class TerminalDialog;
class TextDialog;

class EditorWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

private:
    Ui::EditorWindow *ui;
    KeybindingThread *keybindingThread;

    // Slide data
    QDir rootDir;
    QList<Step> steps;
    int currentStep;



    bool stepActive;

    // Screenshot helpers
    QSystemTrayIcon *systray;
    int screenshotTimeRemaining;
    QTimer *screenshotTimer;

    // Terminal helpers
    TerminalDialog *terminalDialog;

    // Text helpers
    TextDialog *textDialog;


public slots:
    void StartCapture();
    void ShowWindow(QSystemTrayIcon::ActivationReason);
    void KeybindingActivated(int type);
    void CleanUp();


    // Step slots
    void StepFinishSuccess ();
    void StepFinishFail ();
    void StepFinishNoRelease ();


    // Screenshot slots
    void StartScreenshotCountdown();
    void ScreenshotTick();
    void CancelScreenshotCountdown();

    // Terminal slots
    void ShowTerminalDialog();

    // Text slots
    void ShowTextDialog();
    void SetStepTextContent(QString content, QString syntaxType);
};

#endif // EDITORWINDOW_H
