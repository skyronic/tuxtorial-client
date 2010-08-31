#include "terminaldialog.h"
#include "ui_terminaldialog.h"
#include "qtermwidget.h"

TerminalDialog::TerminalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerminalDialog)
{
    QFont font = QApplication::font ();
    font.setFamily ("monospace");
    font.setPointSize (10);

    ui->setupUi(this);
    termWidget = new QTermWidget(1);
    termWidget->setScrollBarPosition (QTermWidget::ScrollBarRight);
    termWidget->setTerminalFont (font);

    ui->terminalTargetLayout->addWidget (termWidget);
    termWidget->show ();
}

TerminalDialog::~TerminalDialog()
{
    delete ui;
}
