#include "tutorialhelper.h"

TutorialHelper::TutorialHelper(QObject *parent) :
    QObject(parent)
{
}

void TutorialHelper::setParams (QDir *dir, QList<Step> *stps)
{
    rootDir = dir;
    steps = stps;
}
