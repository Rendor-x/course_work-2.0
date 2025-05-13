QT += core gui sql widgets

TARGET = library7
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           addform.cpp \
           editform.cpp

HEADERS += mainwindow.h \
           addform.h \
           editform.h

FORMS += mainwindow.ui \
         addform.ui \
         editform.ui

# Для правильной генерации UI-классов
CONFIG += c++17
