#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
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

signals:

public slots:

};

#endif // STEP_H
