#include "terminaldialog.h"
#include "ui_terminaldialog.h"
#include "qtermwidget.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>

const QString stepFile = "/tmp/.ttstep";
TerminalDialog::TerminalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerminalDialog)
{
    QFile ttCommandScript(":/static/ttcommand.sh");
    if(ttCommandScript.open (QIODevice::ReadOnly))
    {
        QString content(ttCommandScript.readAll ());
    }
    else
    {
        qDebug () << "Unable to open the command shell script";
    }
    ui->setupUi(this);
    termWidget = NULL;
    ReloadTerminal ();

    // set up the stepfile
    if(QFile::exists (stepFile))
    {
        QFile::remove (stepFile);
    }
}

void TerminalDialog::ReloadTerminal ()
{
    bool redrewTerminalFlag = false;
    if(termWidget != NULL)
    {
        qDebug() << "Removing terminal widget from layout.";
        ui->terminalTargetLayout->removeWidget (termWidget);
        delete termWidget;
        redrewTerminalFlag = true;
    }

    QFont font = QApplication::font ();
    font.setFamily ("monospace");
    font.setPointSize (10);
    termWidget = new QTermWidget(1);
    if(redrewTerminalFlag)
    {
        QString message = "#Please don't quit the terminal session. Use the discard and finish buttons so that the environment is consistent and the tutorial is easier to follow.\r";
        termWidget->sendText (message);
    }
    termWidget->setScrollBarPosition (QTermWidget::ScrollBarRight);
    termWidget->setTerminalFont (font);
    connect(termWidget, SIGNAL(finished()), this, SLOT(TerminalClosed()));

    ui->terminalTargetLayout->addWidget (termWidget);
    termWidget->show ();
}

TerminalDialog::~TerminalDialog()
{
    delete ui;
}

void TerminalDialog::on_TerminalDialog_destroyed()
{
    qDebug () << "Terminal dialog has been destroyed. this isn't right :(";
}

void TerminalDialog::TerminalClosed ()
{
    qDebug () << "The terminal has been closed";
    ReloadTerminal();
}

void TerminalDialog::closeEvent (QCloseEvent *ev)
{
    // ignore the close event
    ev->ignore ();
    this->hide ();

    emit StepFinishFail ();
}

void TerminalDialog::on_discardStepButton_clicked()
{
    if(QFile::exists (stepFile))
        QFile::remove (stepFile);
    this->hide ();
    emit StepFinishFail ();
}

void TerminalDialog::on_finishStepButton_clicked()
{
    // first, process the step
    if(QFile::exists (stepFile))
    {
        QFile stepFileObj(stepFile);
        if(stepFileObj.open (QIODevice::ReadOnly))
        {
            QString content(stepFileObj.readAll ());
            emit SetStepConsoleContent (content);
        }
        else
        {
            qDebug() << "Unable to open the step file. you should never see this.";
            return;
        }
        stepFileObj.close ();
        QFile::remove (stepFile);
    }
    else
    {
        // treat it as a failure.
        if(ui->keepDialogOpenCheckBox->checkState () == Qt::Checked)
            return;// do nothing at all
        else
        {
            emit StepFinishFail ();
            this->hide ();
        }
        return; // close
    }
    if(ui->keepDialogOpenCheckBox->checkState () == Qt::Checked)
    {
        emit StepFinishNoRelease ();
    }
    else
    {
        this->hide ();
        emit StepFinishSuccess ();
    }
}
