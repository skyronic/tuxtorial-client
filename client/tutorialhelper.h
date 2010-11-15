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
    void StartUpload(QString username, QString password);
    void VerifyPassword (QString username, QString password);


    // TODO: This is bad practice. change to m_thingies
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QNetworkReply *authReply;
    QString title, desc;

private:
    QDir *rootDir;
    QList<Step> *steps;
    QString archivePath;
signals:
    void PasswordVerifySuccess ();
    void PasswordVerifyFail ();
public slots:
    void LoginRequestComplete();

};

#endif // TUTORIALHELPER_H
