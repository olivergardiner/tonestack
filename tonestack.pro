QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    circuit.cpp \
    fender.cpp \
    main.cpp \
    marshall.cpp \
    marshall18.cpp \
    moonlight.cpp \
    potentiometer.cpp \
    tonestackcalculator.cpp \
    vox.cpp

HEADERS += \
    circuit.h \
    fender.h \
    fender.h \
    marshall.h \
    marshall18.h \
    moonlight.h \
    ngspice/sharedspice.h \
    potentiometer.h \
    tonestackcalculator.h \
    vox.h

FORMS += \
    tonestackcalculator.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/ngspice/ -lngspice

INCLUDEPATH += $$PWD/ngspice
DEPENDPATH += $$PWD/ngspice
