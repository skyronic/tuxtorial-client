#include <QtGui/QApplication>
#include "editorwindow.h"
#include "textdialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EditorWindow editor;


    return a.exec();
}
