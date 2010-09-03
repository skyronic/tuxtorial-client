#ifndef TUTORIALHELPER_H
#define TUTORIALHELPER_H

#include <QObject>
#include <QList>

class Step;
class QDir;

class TutorialHelper : public QObject
{
    Q_OBJECT
public:
    explicit TutorialHelper(QObject *parent = 0);
    void setParams(QDir *dir, QList<Step> *stps);
    void UploadTemp ();

private:
    QDir *rootDir;
    QList<Step> *steps;
signals:

public slots:

};

#endif // TUTORIALHELPER_H
