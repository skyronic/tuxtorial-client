#-------------------------------------------------
#
# Project created by QtCreator 2010-08-30T14:59:16
#
#-------------------------------------------------

QT       += core gui network
CONFIG += qt debug_and_release warn_on build_all
TEMPLATE = app
DESTDIR = ..
MOC_DIR = ../.moc

CONFIG(debug, debug|release) {
    OBJECTS_DIR = ../.objs_d
    LIBS 	+= -L.. ../libqtermwidget_d.a
    TARGET = tuxtorial_d
} else {
    OBJECTS_DIR = ../.objs
    LIBS 	+= -L.. ../libqtermwidget.a
    TARGET = tuxtorial
}

SOURCES += main.cpp\
    editorwindow.cpp \
    terminaldialog.cpp \
    textdialog.cpp \
    keybindingthread.cpp \
    screenshotutils.cpp \
    step.cpp \
    tutorialhelper.cpp \
    steppreviewdialog.cpp

HEADERS  += \
    editorwindow.h \
    terminaldialog.h \
    textdialog.h \
    keybindingthread.h \
    screenshotutils.h \
    step.h \
    tutorialhelper.h \
    steppreviewdialog.h

FORMS    += \
    editorwindow.ui \
    terminaldialog.ui \
    textdialog.ui \
    steppreviewdialog.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += ../lib

OTHER_FILES +=
