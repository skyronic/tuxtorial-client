#ifndef STEPPREVIEWDIALOG_H
#define STEPPREVIEWDIALOG_H

#include <QDialog>
class Step;

namespace Ui {
    class StepPreviewDialog;
}

class StepPreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StepPreviewDialog(const Step *_target, QWidget *parent = 0);
    ~StepPreviewDialog();

private:
    Ui::StepPreviewDialog *ui;
    Step *target;
};

#endif // STEPPREVIEWDIALOG_H
