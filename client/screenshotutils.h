#ifndef SCREENSHOTUTILS_H
#define SCREENSHOTUTILS_H

#include <QObject>
QT_BEGIN_NAMESPACE
class QDir;
QT_END_NAMESPACE

class ScreenshotUtils : public QObject
{
    Q_OBJECT
public:
    explicit ScreenshotUtils(QObject *parent = 0);
    bool TakeAndSaveScreenshot(QDir dir, int stepNumber, QString &screenshotPath);

signals:

public slots:

};

#endif // SCREENSHOTUTILS_H
