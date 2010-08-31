#include "textdialog.h"
#include "ui_textdialog.h"

TextDialog::TextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDialog)
{
    ui->setupUi(this);
}

TextDialog::~TextDialog()
{
    delete ui;
}
