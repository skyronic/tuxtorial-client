#include "screenshotutils.h"
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QtGui>
#include <QFile>
#include <QDir>
#include <QtGlobal>
#include <QtDebug>
ScreenshotUtils::ScreenshotUtils(QObject *parent) :
    QObject(parent)
{
}

bool ScreenshotUtils::TakeAndSaveScreenshot (QDir rootPath, int stepNumber, QString& screenshotPath)
{
    QPixmap screenshotMap;
    qDebug() << "The rootpath is " << rootPath.absolutePath ();
    screenshotMap = QPixmap::grabWindow (QApplication::desktop ()->winId ());
    QFile scrotFile(rootPath.filePath (QString::number (stepNumber) + tr(".jpg")));
    if(scrotFile.open (QFile::WriteOnly))
    {
        qDebug() << "Saved screenshot to: " << scrotFile.fileName ();
        screenshotMap.save (&scrotFile);
        screenshotPath = scrotFile.fileName();
    }
    else
    {
        return false;
    }
    return true;
}
