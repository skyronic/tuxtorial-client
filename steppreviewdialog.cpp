#include "steppreviewdialog.h"
#include "ui_steppreviewdialog.h"
#include "step.h"
#include <QWidget>
#include <QImage>
#include <QTextEdit>
#include <QGraphicsView>
#include <QLabel>
#include <QPixmap>

StepPreviewDialog::StepPreviewDialog(const Step *_target, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepPreviewDialog)
{
    ui->setupUi(this);
    QTextEdit *textWidget;
    QPixmap pic;
    QLabel *disp;

    // set the preview widget in accordance to the type
    switch(_target->Type)
    {
    case Step::Text:
    case Step::Console:
        textWidget = new QTextEdit(_target->TextContent);
        textWidget->setReadOnly (true);
        ui->verticalLayout->addWidget (textWidget, 1);
        textWidget->show ();
        break;
    case Step::Screenshot:
        pic = QPixmap::fromImage (QImage(_target->ScreenshotPath));
        disp = new QLabel();
        disp->size ().setHeight (250);
        disp->size ().setWidth (430);
        ui->verticalLayout->addWidget (disp, 1);
        disp->show();
        disp->setPixmap (pic.scaled(disp->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        break;
    default:
        break;


    }

}

StepPreviewDialog::~StepPreviewDialog()
{
    delete ui;
}

void StepPreviewDialog::on_buttonBox_accepted()
{
    this->close ();
}
