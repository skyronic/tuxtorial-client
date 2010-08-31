#include <QtGui/QApplication>
#include "editorwindow.h"
#include "textdialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //EditorWindow editor;
    //editor.show ();
    TextDialog target;
    target.show ();

    return a.exec();
}
