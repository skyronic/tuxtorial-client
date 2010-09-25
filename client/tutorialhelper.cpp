#include "tutorialhelper.h"
#include <QtGlobal>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QList>
#include "step.h"
#include <QXmlStreamWriter>
#include <QProcess>
#include <QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttp>
#include <QByteArray>
#include <QBuffer>
#include <QApplication>
#include <QCryptographicHash>
#include <QNetworkProxy>

TutorialHelper::TutorialHelper(QObject *parent) :
    QObject(parent)
{
    authReply = NULL;
}

void TutorialHelper::setParams (QDir *dir, QList<Step> *stps)
{
    rootDir = dir;
    steps = stps;
}

void TutorialHelper::SerializeToFile (){
    // do the xml serialization.
    QString output;
    QXmlStreamWriter stream(&output);
    stream.setAutoFormatting (true);
    stream.writeStartDocument ("1.0");
    stream.setCodec ("utf-16");
    stream.writeStartElement ("ClientTutorial");
    stream.writeTextElement ("Title", "The title goes here <br>");
    stream.writeStartElement ("Steps");
    // write each step
    for(int i = 0; i< steps->length (); i++)
    {
        Step target = steps->at(i);
        stream.writeStartElement ("ClientStep");
        switch(target.Type)
        {
        case Step::Screenshot:
            stream.writeTextElement ("StepType", "Screenshot");
            stream.writeTextElement ("ScreenshotFileName", target.ScreenshotFileName);
            break;
        case Step::Text:
            stream.writeTextElement ("StepType", "Text");
            stream.writeTextElement ("TextContent", target.TextContent);
            stream.writeTextElement ("SyntaxHighlight", target.SyntaxHighlight);
            break;
        case Step::Console:
            stream.writeTextElement ("StepType", "Console");
            stream.writeTextElement ("TextContent", target.TextContent);
            break;
        }
        stream.writeEndElement ();
    }
    stream.writeEndElement ();
    stream.writeEndElement ();
    stream.writeEndDocument ();

    // Save the serialized xml to a file
    QFile content(rootDir->filePath ("content.xml"));
    if(content.open (QIODevice::WriteOnly))
    {
        content.write (output.toUtf8 ());
    }
}

void TutorialHelper::CreateArchive ()
{
    QProcess *zipProcess = new QProcess();
    // get the dir name for later usage
    QString dirName = rootDir->dirName ();
    // run the program in /tmp or whatever is the parent of root
    zipProcess->setWorkingDirectory (rootDir->absolutePath ());

    // check if dirname.zip already exists
    if(QFile::exists (rootDir->absoluteFilePath (dirName + ".zip")))
        QFile::remove (rootDir->absoluteFilePath (dirName + ".zip"));


    QString program = "/usr/bin/zip";
    QStringList args;
    args << "-r" << dirName + ".zip" << ".";

    zipProcess->start (program, args);
    zipProcess->waitForFinished ();
    qDebug() << "Wrote " << dirName << ".zip";
    archivePath = rootDir->absoluteFilePath (dirName + ".zip");
}

void TutorialHelper::VerifyPassword (QString username, QString password)
{
    // First, get an MD5 hash from the password
    QByteArray hashedPass = QCryptographicHash::hash (password.toAscii (), QCryptographicHash::Md5);
    QString hashedString = QString(hashedPass.toHex ().constData ());

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    QUrl endpoint;
    endpoint.setUrl ("http://localhost:8080/User/VerifyLogin");
    endpoint.addQueryItem ("username", username);
    endpoint.addQueryItem ("password", hashedString);

    qDebug() << "Making request to: " << endpoint;

    request.setUrl (endpoint);
    authReply = manager->get (request);

    connect (authReply, SIGNAL(finished()), this, SLOT(LoginRequestComplete()));
}

void TutorialHelper::LoginRequestComplete()
{
    qDebug() << "The output is: " << authReply->readAll ();
}


void TutorialHelper::StartUpload ()
{
        QString bound;
        QString crlf;
        QString data;
        QByteArray dataToSend;
        QFile file(archivePath);
        file.open(QIODevice::ReadOnly);

        bound = "---------------------------7d935033608e2";
        crlf = 0x0d;
        crlf += 0x0a;
        data = "--" + bound + crlf + "Content-Disposition: form-data; name=\"file\"; ";
        data += "filename=\"uploadfile.zip\"";
        data += crlf + "Content-Type: application/octet-stream" + crlf + crlf;
        dataToSend.insert(0,data);
        dataToSend.append(file.readAll());
        dataToSend.append(crlf + "--" + bound + "--" + crlf);

        QUrl url("http://localhost:8000/upload");
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, tr("multipart/form-data; boundary=") + bound);
        file.close();

        manager = new QNetworkAccessManager();
        //connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(SlotRequestFinished(QNetworkReply*)));
        reply = manager->post(req, dataToSend);
        //connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(SlotSetProgressLevel(qint64, qint64)));
}

void TutorialHelper::UploadTemp ()
{
    // create new stub data
    qsrand(QDateTime::currentDateTime ().toTime_t ());

    // Setting the root directory
    // start off with the temp directory
    QDir tempRootDir = QDir::temp ();
    QString randString =QString::number (qrand());
    if(tempRootDir.mkdir (randString))
    {
        tempRootDir.cd (randString);
        qDebug () << "Created directory";

        // make an images directory as well
        tempRootDir.mkdir ("images");
    }
    else
        qDebug() << "ERROR! Unable to create root directory";
    this->rootDir = &tempRootDir;

    this->steps = new QList<Step>();

    Step t1, t2, t3, t4, t5, t6;
    t1.Type = Step::Screenshot;
    t1.ScreenshotFileName = "1.png";
    steps->append (t1);
    t2.Type = Step::Text;
    t2.TextContent = "Step 1 text";
    steps->append (t2);
    t3.Type = Step::Text;
    t3.TextContent = "Step 2 text";
    steps->append (t3);

    // Create some console commands
    ConsoleCommand c1, c2;
    c1.Command = "com1";
    c1.Path = "path1";
    c1.Output = "output1";
    c2.Command = "com2";
    c2.Path = "path2";
    c2.Output = "output2";
    t4.Type = Step::Console;
    t4.Commands.append (c1);
    t4.Commands.append (c2);
    steps->append (t4);

    this->SerializeToFile ();
    this->CreateArchive ();
    this->CleanUp ();
}

void TutorialHelper::CleanUp ()
{
    rootDir->rmdir (rootDir->absolutePath ());
}
