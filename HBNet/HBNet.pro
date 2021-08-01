QT -= gui
QT += network

TEMPLATE = lib
DEFINES += HBNET_LIBRARY

CONFIG += c++11
DESTDIR = ../../Apps/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hblocalclient.cpp \
    hblocalserver.cpp \
    hbnet.cpp \
    hbsocketthread.cpp

HEADERS += \
    HBNet_global.h \
    hblocalclient.h \
    hblocalserver.h \
    hbnet.h \
    hbsocketthread.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
