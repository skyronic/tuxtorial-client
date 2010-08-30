#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

namespace Ui {
    class EditorWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

private:
    Ui::EditorWindow *ui;
    QSystemTrayIcon *systray;


public slots:
    void StartCapture();
    void ShowWindow(QSystemTrayIcon::ActivationReason);
};

#endif // EDITORWINDOW_H
