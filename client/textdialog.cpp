#include "textdialog.h"
#include "ui_textdialog.h"
#include <QClipboard>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QCloseEvent>

TextDialog::TextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDialog)
{
    ui->setupUi(this);
    connect(this, SIGNAL(rejected()), this, SLOT(on_discardStepButton_clicked()));
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

void TextDialog::closeEvent (QCloseEvent *ev)
{
    ev->ignore ();
    this->hide ();
    ui->textEdit->clear ();

    // closing the window is a failed step
    emit StepFinishFail ();

    qDebug() << "User tried to close the dialog";
}

void TextDialog::on_finishStepButton_clicked()
{
    // set the text content for the step
    emit SetStepTextContent (ui->textEdit->document ()->toPlainText (), "plain");
    ui->textEdit->clear ();

    // Shouldwe keep the dialog open
    if(ui->keepDialogOpenCheckbox->checkState () == Qt::Checked)
    {
        emit StepFinishNoRelease();
    }
    else
    {
        emit StepFinishSuccess ();
        this->hide ();
    }
}

void TextDialog::on_discardStepButton_clicked()
{
    ui->textEdit->clear ();
    this->hide ();
    emit StepFinishFail ();
}
