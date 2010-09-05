#include "editorwindow.h"
#include "ui_editorwindow.h"
#include <QSystemTrayIcon>
#include <QList>
#include <QIcon>
#include <QTimer>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QDateTime>
#include <QListWidget>
#include <QListWidgetItem>
#include <QModelIndex>
#include <QMessageBox>
#include <QCloseEvent>

#include "terminaldialog.h"
#include "textdialog.h"
#include "keybindingthread.h"
#include "screenshotutils.h"
#include "steppreviewdialog.h"

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{

    // Initialize properties
    currentStep = 0;
    stepActive = 0;
    qsrand(QDateTime::currentDateTime ().toTime_t ());

    // Setting the root directory
    // start off with the temp directory
    rootDir = QDir::temp ();
    QString randString =QString::number (qrand());
    if(rootDir.mkdir (randString))
    {
        rootDir.cd (randString);
        qDebug () << "Created directory";

        // make an images directory as well
        rootDir.mkdir ("images");
    }
    else
        qDebug() << "ERROR! Unable to create root directory";

    terminalDialog = new TerminalDialog;
    terminalDialog->hide ();

    textDialog = new TextDialog;
    textDialog->hide ();

    keybindingThread = new KeybindingThread;
    keybindingThread->start ();

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
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(CleanUp()));
    connect(ui->stepListWidget, SIGNAL(activated(QModelIndex)), this, SLOT(StepListActivated(QModelIndex)));

    // Keybinding signals
    connect(keybindingThread, SIGNAL(KeybindingActivated(int)), this, SLOT(KeybindingActivated(int)));

    // Screenshot signals
    connect(ui->actionCapture_Screenshot, SIGNAL(triggered()), this, SLOT(StartScreenshotCountdown()));
    connect(systray, SIGNAL(messageClicked()), this, SLOT(CancelScreenshotCountdown()));
    connect(screenshotTimer, SIGNAL(timeout()), this, SLOT(ScreenshotTick()));

    // terminal widget signals
    connect(ui->actionCapture_Commands, SIGNAL(triggered()), this, SLOT(ShowTerminalDialog()));
    connect(terminalDialog, SIGNAL(StepFinishSuccess()), this, SLOT(StepFinishSuccess()));
    connect(terminalDialog, SIGNAL(StepFinishFail()), this, SLOT(StepFinishFail()));
    connect(terminalDialog, SIGNAL(StepFinishNoRelease()), this, SLOT(StepFinishNoRelease()));

    // text widget signals
    connect(ui->actionCapture_Text, SIGNAL(triggered()), this, SLOT(ShowTextDialog()));
    connect(textDialog, SIGNAL(StepFinishSuccess()), this, SLOT(StepFinishSuccess()));
    connect(textDialog, SIGNAL(StepFinishFail()), this, SLOT(StepFinishFail()));
    connect(textDialog, SIGNAL(StepFinishNoRelease()), this, SLOT(StepFinishNoRelease()));
    connect(textDialog, SIGNAL(SetStepTextContent(QString,QString)), this, SLOT(SetStepTextContent(QString,QString)));
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::StepListActivated (QModelIndex index)
{
    qDebug() << "The activated row is: " << index.row ();
    StepPreviewDialog *preview = new StepPreviewDialog(&steps.at(index.row()));
    preview->show ();

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

        ScreenshotUtils scrotUtils;
        QDir target(rootDir.absolutePath ());
        qDebug() << "The rootDir here is: " << rootDir;
        target.cd ("images");
        QString screenshotPath;
        if(scrotUtils.TakeAndSaveScreenshot (target , currentStep, screenshotPath))
        {
            systray->showMessage (tr("Captured screenshot successfully"), "", QSystemTrayIcon::Information, 1000);
            Step target;
            target.Type = Step::Screenshot;
            target.ScreenshotFileName = QString::number (currentStep) + ".png";
            target.ScreenshotPath =screenshotPath;
            steps.append (target);

            StepFinishSuccess ();
        }
        else
        {
            systray->showMessage (tr("Sorry, was unable to take screenshot"), "error details: ", QSystemTrayIcon::Critical, 1000);
            StepFinishFail();
        }

        stepActive = false;
    }
    else
    {
        QString message;
        QTextStream(&message) << "in " << screenshotTimeRemaining << " seconds";
        systray->showMessage (tr("Will take a screenshot"), message, QSystemTrayIcon::Information, screenshotTimeRemaining * 1000 - 200);
        screenshotTimeRemaining --;
    }
}

void EditorWindow::StartScreenshotCountdown ()
{
    if(!stepActive)
    {
        screenshotTimeRemaining = 5;
        screenshotTimer->start (1000);
    }
}

void EditorWindow::CancelScreenshotCountdown()
{
    screenshotTimer->stop ();
    stepActive = false;
    StepFinishFail ();
}

void EditorWindow::ShowTerminalDialog ()
{
    if(!stepActive)
    {
        stepActive = true;
        terminalDialog->show ();
    }
}

void EditorWindow::ShowTextDialog ()
{
    if(!stepActive)
    {
        stepActive = true;
        textDialog->show ();
    }
}

void EditorWindow::StepFinishSuccess ()
{
    qDebug() << "Step finished successfully";
    stepActive = false;
    currentStep ++;
    RefreshStepList ();
}

void EditorWindow::StepFinishFail ()
{
    qDebug() << "Step did not finish successfully";
    stepActive = false;
}

void EditorWindow::StepFinishNoRelease ()
{
    qDebug() << "Step finished. No release";
    stepActive = true;
    currentStep ++;
    RefreshStepList ();
}


void EditorWindow::KeybindingActivated (int type)
{
    switch(type)
    {
        case KeybindingThread::ConsoleStep:
            ShowTerminalDialog ();
            break;

        case KeybindingThread::TextStep:
            ShowTextDialog ();
            break;

        case KeybindingThread::ScreenshotStep:
            StartScreenshotCountdown ();
            break;
    }
}

void EditorWindow::SetStepTextContent (QString content, QString syntaxType)
{
    Step target;
    target.TextContent = content;
    target.SyntaxHighlight = syntaxType;
    target.Type = Step::Text;

    // Append this to the list of steps
    steps.append (target);
}

void EditorWindow::CleanUp ()
{
    int x = 0;
    QString dirname = rootDir.dirName ();
    rootDir.cdUp ();
    rootDir.rmdir (dirname);
    qDebug() << "Removed the temporary files.";
    QApplication::quit ();
}

void EditorWindow::on_commandLinkButton_clicked()
{
    this->hide ();
}

void EditorWindow::RefreshStepList ()
{
    // clean up the list widget as we'll redraw the list
    // this is a lazier way than going and keeping my step lists
    // and the view widget in sync.
    qDebug() << "Refreshing the step list now";
    QList<QListWidgetItem*> rowItems;
    for(int i=0; i<ui->stepListWidget->count (); i++)
    {
        QListWidgetItem *rowItem  = ui->stepListWidget->item (i);
        rowItems.append (rowItem);
    }

    // gather all the items first and then delete them. My naive guess
    // is that if I remove the widget straight away, then the item(i)
    // function will start to behave differently

    for(int j=0; j < rowItems.count (); j++)
    {
        ui->stepListWidget->removeItemWidget (rowItems.at (j));
        delete rowItems.at(j); // clean up memoy
    }

    // now add rows for each step
    for(int k = 0; k < steps.count (); k++)
    {
        Step toAdd = steps.at (k);
        QListWidgetItem *itemToAdd = new QListWidgetItem();
        switch(toAdd.Type)
        {
        case Step::Screenshot:
            itemToAdd->setText ("Screenshot");
            itemToAdd->setIcon (QIcon(":/icons/helpicon.png"));
            break;
        case Step::Console:
            itemToAdd->setText ("Console");
            itemToAdd->setIcon (QIcon(":/icons/helpicon.png"));
            break;
        case Step::Text:
            itemToAdd->setText ("Text");
            itemToAdd->setIcon (QIcon(":/icons/helpicon.png"));
            break;
        default:
            qDebug() << "Error! unknown step type";
        }
        ui->stepListWidget->addItem (itemToAdd);
    }
}

// Execute this while closing the app
void EditorWindow::closeEvent (QCloseEvent *ev)
{
    // show a dialog asking whether to go to
    QMessageBox msgBox;
    msgBox.setText ("Quit tuxtorial");
    msgBox.setInformativeText ("Do you want to quit or hide to system tray");

    QPushButton *hideButton = msgBox.addButton ("Hide", QMessageBox::RejectRole);
    QPushButton *quitButton = msgBox.addButton ("Quit", QMessageBox::AcceptRole);

    msgBox.exec ();
    if(msgBox.clickedButton () == hideButton)
    {
        ev->ignore ();
        this->hide ();
    }
    else
    {
        CleanUp ();
    }
}
