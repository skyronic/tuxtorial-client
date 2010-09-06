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
TutorialHelper::TutorialHelper(QObject *parent) :
    QObject(parent)
{
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
