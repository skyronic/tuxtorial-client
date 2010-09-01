#ifndef STEP_H
#define STEP_H

#include <QObject>

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
};

#endif // STEP_H
