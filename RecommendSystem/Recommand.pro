TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp \
    leveldbapi.cpp \
    user.cpp \
    def.cpp


INCLUDEPATH += $$PWD/leveldb/include/ $$PWD/leveldb/helpers/
LIBS += $$PWD/leveldb/libleveldb.a \
        $$PWD/leveldb/libmemenv.a
windows{
LIBS+= -lshlwapi
}

HEADERS += \
    leveldbapi.h \
    user.h \
    def.h
