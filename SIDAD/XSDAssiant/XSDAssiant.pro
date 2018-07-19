#-------------------------------------------------
#
# Project created by QtCreator 2018-03-02T12:05:37
#
#-------------------------------------------------

QT       += core gui\
            xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XSDAssiant
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rightclickedmenu.cpp \
    clickeditem.cpp \
    xml_analysis.cpp \
    xml_create.cpp \
    mydockwidget.cpp \
    update_ui.cpp

HEADERS  += mainwindow.h \
    type.h \
    rightclickedmenu.h \
    clickeditem.h \
    xsd_type.h \
    xml_analysis.h \
    xml_create.h \
    mydockwidget.h \
    mytreeitem.h \
    update_ui.h

FORMS    +=
