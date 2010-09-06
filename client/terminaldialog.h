#ifndef TERMINALDIALOG_H
#define TERMINALDIALOG_H

#include <QDialog>

namespace Ui {
    class TerminalDialog;
}
class QTermWidget;
class QTemporaryFile;

class TerminalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TerminalDialog(QWidget *parent = 0);
    ~TerminalDialog();

private:
    Ui::TerminalDialog *ui;
    QTermWidget *termWidget;
    void closeEvent (QCloseEvent *ev);
    void ReloadTerminal();

signals:
    void StepFinishSuccess();
    void StepFinishFail();
    void StepFinishNoRelease();
    void SetStepConsoleContent(QString content);


private slots:
    void on_finishStepButton_clicked();
    void on_discardStepButton_clicked();
    void on_TerminalDialog_destroyed();
public slots:
    void TerminalClosed();
};

#endif // TERMINALDIALOG_H
