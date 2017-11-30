#-------------------------------------------------
#
# Project created by QtCreator 2014-12-02T13:16:42
#
#-------------------------------------------------

QT       += core xml network
 
greaterThan(QT_MAJOR_VERSION, 4)

TARGET = sn_termo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    can.cpp \
    can_200api.cpp \
    canthread.cpp \
    measurdata.cpp \
    data_t.cpp \
    log.cpp \
    logthread.cpp \
    framelog.cpp \
    xmlconf.cpp \
    api_func.cpp \
    modbusserver.cpp \
    abstractmodbusdevice.cpp \
    convertregisters.cpp \
    modbusslavedevice.cpp \
    can_iei/pci_1680.cpp


HEADERS  += mainwindow.h \
    can.h \
    can_200api.h \
    can_codes.h \
    canthread.h \
    measurdata.h \
    data_t.h \
    log.h \
    logthread.h \
    framelog.h \
    xmlconf.h \
    api_func.h \
    modbusserver.h \
    abstractmodbusdevice.h \
    convertregisters.h \
    mobus_conf.h \
    modbusslavedevice.h \
    can_iei/pci_1680.h \
    can_iei/commdef.h

