#ifndef TUTORIALHELPER_H
#define TUTORIALHELPER_H

#include <QObject>
#include <QList>

class Step;
class QDir;
class QNetworkAccessManager;
class QNetworkReply;

class TutorialHelper : public QObject
{
    Q_OBJECT
public:
    explicit TutorialHelper(QObject *parent = 0);
    void setParams(QDir *dir, QList<Step> *stps);
    void UploadTemp ();
    void SerializeToFile ();
    void CreateArchive ();
    void CleanUp ();
    void StartUpload();

    QNetworkAccessManager *manager;
    QNetworkReply *reply;

private:
    QDir *rootDir;
    QList<Step> *steps;
    QString archivePath;
signals:

public slots:

};

#endif // TUTORIALHELPER_H
