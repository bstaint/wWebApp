TEMPLATE = app
CONFIG +=  c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    src/callback.h \
    src/blinkwidget.h \
    src/mainform.h \
    src/precompiled.h \
    src/shadowform.h \
    src/gdiplushelper.h \
    src/utility.h \
    src/zipui.h

SOURCES += \
    src/callback.cpp \
    src/blinkwidget.cpp \
    src/main.cpp \
    src/mainform.cpp \
    src/shadowform.cpp \
    src/utility.cpp \
    src/zipui.cpp

INCLUDEPATH += src/

PRECOMPILED_HEADER = src/precompiled.h

LIBS += -lgdiplus

RC_FILE = src/app.rc

win32-msvc*:QMAKE_CXXFLAGS += /utf-8

include(E:/3rdParty/parser/zlib.pri)
include(E:/3rdParty/parser/minizip.pri)
include(E:/3rdParty/gui/winlamb.pri)
include(E:/3rdParty/extras/miniblink.pri)
