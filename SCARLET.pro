#-------------------------------------------------
#
# Project created by QtCreator 2016-11-11T20:05:08
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SCARLET
TEMPLATE = app

CONFIG += c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    queue/priorityqueue.cpp \
    comparable/comparable.cpp \
    comparable/huffman/huffmantree.cpp \
    comparable/teste.cpp \
    xml/streamparser.cpp \
    util/util.cpp \
    compress/compress.cpp \
    xml/element.cpp \
    comparable/huffman/huffelement.cpp \
    decompress/decompress.cpp

HEADERS  += mainwindow.h \
    tree/patricia.h \
    queue/priorityqueue.h \
    comparable/comparable.h \
    comparable/huffman/huffmantree.h \
    comparable/teste.h \
    xml/streamparser.h \
    util/util.h \
    compress/compress.h \
    xml/element.h \
    comparable/huffman/huffelement.h \
    decompress/decompress.h

FORMS    += mainwindow.ui
