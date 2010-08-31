#include "textdialog.h"
#include "ui_textdialog.h"
#include <QClipboard>
#include <QFileDialog>
#include <QFile>
#include <QDebug>

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

void TextDialog::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName (this, tr("Open text file"), "", tr("All Files (*.*)"));

    qDebug () << "Opening " << fileName;
    QString content;
    if(QFile::exists (fileName))
    {
        QFile targetfile(fileName);
        if(!targetfile.open (QIODevice::ReadOnly | QIODevice::Text))
            return;
        content = targetfile.readAll ();
    }

    ui->textEdit->setText (content);
}

void TextDialog::on_pasteButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard ();

    // Get both clipboard and X11 selection
    QString clipString = clipboard->text (QClipboard::Clipboard);
    QString selectString = clipboard->text (QClipboard::Selection);

    QString targetString = "";
    if(clipString.length () > 0)
        targetString = clipString;
    else if(selectString.length () > 0)
        targetString = selectString;

    ui->textEdit->setText (targetString);
}
