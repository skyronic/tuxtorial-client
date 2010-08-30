#include "editorwindow.h"
#include "ui_editorwindow.h"
#include <QSystemTrayIcon>
#include <QList>
#include <QIcon>

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);

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
