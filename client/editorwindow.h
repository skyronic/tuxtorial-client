#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "keybindingthread.h"
#include <QList>
#include <QDir>
#include <QModelIndex>
#include <QtNetwork/QNetworkReply>
#include "step.h"

namespace Ui {
    class EditorWindow;
}

class QTimer;
class TerminalDialog;
class TutorialHelper;
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
    void closeEvent (QCloseEvent *);

    // Slide data
    QDir rootDir;
    QList<Step> steps;
    int currentStep;

    TutorialHelper *tutorialHelper;




    bool stepActive;

    // Screenshot helpers
    QSystemTrayIcon *systray;
    int screenshotTimeRemaining;
    QTimer *screenshotTimer;

    // Terminal helpers
    TerminalDialog *terminalDialog;

    // Text helpers
    TextDialog *textDialog;

    void RefreshStepList();
    void ProcessAndStartUpload();


public slots:
    void StartCapture();
    void ShowWindow(QSystemTrayIcon::ActivationReason);
    void KeybindingActivated(int type);
    void CleanUp();
    void StepListActivated(QModelIndex index);


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
    void SetStepConsoleContent(QString content);

    // Text slots
    void ShowTextDialog();
    void SetStepTextContent(QString content, QString syntaxType);

    // network slots
    void UpdateNetworkCount(qint64 complete, qint64 total);
    void UploadFinished(QNetworkReply *reply);
    void PasswordVerifySuccess();
    void PasswordVerifyFail();
    void NetworkError (QNetworkReply::NetworkError error);


private slots:
    void on_uploadButton_clicked();
    void on_commandLinkButton_clicked();
};

#endif // EDITORWINDOW_H
