#ifndef STEP_H
#define STEP_H

#include <QObject>
#include <QList>
struct ConsoleCommand
{
    QString Command;
    QString Path;
    QString Output;
};

class Step
{
public:
    Step();
    enum StepType
    {
        Screenshot,
        Console,
        Text
    };
    StepType Type;

    // Text parameters
    QString TextContent;
    QString SyntaxHighlight;

    // Screenshot parameters
    QString ScreenshotFileName;

    // console parameters
    QList<ConsoleCommand> Commands;
};

#endif // STEP_H
