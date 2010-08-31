#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

namespace Ui {
    class EditorWindow;
}

class QTimer;
class TerminalDialog;

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

private:
    Ui::EditorWindow *ui;

    // Screenshot helpers
    QSystemTrayIcon *systray;
    int screenshotTimeRemaining;
    QTimer *screenshotTimer;
    TerminalDialog *terminalDialog;


public slots:
    void StartCapture();
    void ShowWindow(QSystemTrayIcon::ActivationReason);
    void StartScreenshotCountdown();
    void ScreenshotTick();
    void CancelScreenshotCountdown();

    // Terminal slots
    void ShowTerminalDialog();
};

#endif // EDITORWINDOW_H
