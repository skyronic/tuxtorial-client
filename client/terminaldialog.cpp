#include "terminaldialog.h"
#include "ui_terminaldialog.h"
#include "qtermwidget.h"
#include <QDebug>

TerminalDialog::TerminalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerminalDialog)
{
    qDebug() << "The terminal dialog is activated";
    QFont font = QApplication::font ();
    font.setFamily ("monospace");
    font.setPointSize (10);

    ui->setupUi(this);
    termWidget = new QTermWidget(1);
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
    QString message = tr("You have closed the terminal.");
    termWidget->sendText (message);

}

void TerminalDialog::closeEvent (QCloseEvent *ev)
{
    // ignore the close event
    ev->ignore ();
    this->hide ();

    emit StepFinishFail ();
}
