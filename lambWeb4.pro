TEMPLATE = app
CONFIG +=  c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    src/blinkwidget.h \
    src/mainform.h \
    src/precompiled.h \
    src/shadowform.h \
    src/gdiplushelper.h \
    src/utility.h

SOURCES += \
    src/blinkwidget.cpp \
    src/main.cpp \
    src/mainform.cpp \
    src/shadowform.cpp

INCLUDEPATH += src/

PRECOMPILED_HEADER = src/precompiled.h

LIBS += -lgdiplus

include(E:/3rdParty/gui/winlamb.pri)
include(E:/3rdParty/extras/miniblink.pri)
