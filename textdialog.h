#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>

namespace Ui {
    class TextDialog;
}

class TextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextDialog(QWidget *parent = 0);
    ~TextDialog();

private:
    Ui::TextDialog *ui;
    void closeEvent (QCloseEvent *);

private slots:
    void on_discardStepButton_clicked();
    void on_finishStepButton_clicked();
    void on_pasteButton_clicked();
    void on_openFileButton_clicked();

signals:
    void StepFinishSuccess();
    void StepFinishFail();
    void StepFinishNoRelease();

    void SetStepTextContent(QString content, QString syntaxType);
};

#endif // TEXTDIALOG_H
