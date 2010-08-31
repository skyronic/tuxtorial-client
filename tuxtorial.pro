#-------------------------------------------------
#
# Project created by QtCreator 2010-08-30T14:59:16
#
#-------------------------------------------------

QT       += core gui
TARGET = tuxtorial
TEMPLATE = app

CONFIG(debug, debug|release) {
    LIBS 	+= -L.. libqtermwidget_d.a
} else {
    LIBS 	+= -L.. libqtermwidget.a
}

SOURCES += main.cpp\
    editorwindow.cpp \
    terminaldialog.cpp \
    textdialog.cpp

HEADERS  += \
    editorwindow.h \
    terminaldialog.h \
    textdialog.h

FORMS    += \
    editorwindow.ui \
    terminaldialog.ui \
    textdialog.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += lib

OTHER_FILES +=
