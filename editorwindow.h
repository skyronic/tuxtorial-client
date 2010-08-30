#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

namespace Ui {
    class EditorWindow;
}

class QTimer;

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


public slots:
    void StartCapture();
    void ShowWindow(QSystemTrayIcon::ActivationReason);
    void StartScreenshotCountdown();
    void ScreenshotTick();
    void CancelScreenshotCountdown();
};

#endif // EDITORWINDOW_H
