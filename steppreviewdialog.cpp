#include "steppreviewdialog.h"
#include "ui_steppreviewdialog.h"
#include "step.h"
#include <QWidget>
#include <QImage>
#include <QTextEdit>

StepPreviewDialog::StepPreviewDialog(const Step *_target, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StepPreviewDialog)
{
    ui->setupUi(this);

    // set the preview widget in accordance to the type
    switch(_target->Type)
    {
    case Step::Text:
        QTextEdit *textWidget = new QTextEdit(_target->TextContent, ui->previewWidget);
        textWidget->setReadOnly (true);
        textWidget->show ();
        break;
    }

}

StepPreviewDialog::~StepPreviewDialog()
{
    delete ui;
}
