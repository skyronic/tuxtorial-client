#include "editorwindow.h"
#include "ui_editorwindow.h"
#include <QSystemTrayIcon>
#include <QList>
#include <QIcon>
#include <QTimer>
#include <QTextStream>
#include "terminaldialog.h"

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{

    terminalDialog = new TerminalDialog;
    terminalDialog->hide ();

    ui->setupUi(this);
    screenshotTimer = new QTimer();

    systray = new QSystemTrayIcon();
    systray->setIcon (QIcon(":/icons/helpicon.png"));
    systray->setContextMenu (new QMenu());
    systray->contextMenu ()->addAction (ui->actionCapture_Screenshot);
    systray->contextMenu ()->addAction (ui->actionCapture_Text);
    systray->contextMenu ()->addAction (ui->actionCapture_Commands);
    systray->contextMenu ()->addSeparator ();
    systray->contextMenu ()->addAction (ui->actionExit);
    systray->show ();

    // Set the three screenshot options:

    // Connect the actions to corresponding signals
    connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(StartCapture()));
    connect(systray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(ShowWindow(QSystemTrayIcon::ActivationReason)));
    connect(ui->actionCapture_Screenshot, SIGNAL(triggered()), this, SLOT(StartScreenshotCountdown()));
    connect(systray, SIGNAL(messageClicked()), this, SLOT(CancelScreenshotCountdown()));
    connect(screenshotTimer, SIGNAL(timeout()), this, SLOT(ScreenshotTick()));

    // terminal widget signals
    connect(ui->actionCapture_Commands, SIGNAL(triggered()), this, SLOT(ShowTerminalDialog()));
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::StartCapture ()
{
    this->hide ();
}

void EditorWindow::ShowWindow (QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        this->show ();
    }
}

void EditorWindow::ScreenshotTick ()
{
    if(screenshotTimeRemaining == 0)
    {
        // Take the screenshot
        screenshotTimer->stop();
    }
    else
    {
        QString message;
        QTextStream(&message) << "in " << screenshotTimeRemaining << " seconds";
        systray->showMessage ("Will take a screenshot", message, QSystemTrayIcon::Information, screenshotTimeRemaining * 1000 - 200);
        screenshotTimeRemaining --;
    }
}

void EditorWindow::StartScreenshotCountdown ()
{
    screenshotTimeRemaining = 5;
    screenshotTimer->start (1000);
}

void EditorWindow::CancelScreenshotCountdown()
{
    screenshotTimer->stop ();
}

void EditorWindow::ShowTerminalDialog ()
{
    terminalDialog->show ();

}
