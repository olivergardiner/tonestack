QT       += core gui printsupport help

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    capacitor.cpp \
    circuit.cpp \
    circuitelement.cpp \
    helpbrowser.cpp \
    helptextviewer.cpp \
    inductor.cpp \
    main.cpp \
    namedialog.cpp \
    potentiometer.cpp \
    resistor.cpp \
    settingsdialog.cpp \
    tonestackcalculator.cpp

HEADERS += \
    capacitor.h \
    circuit.h \
    circuitelement.h \
    helpbrowser.h \
    helptextviewer.h \
    inductor.h \
    namedialog.h \
    ngspice/sharedspice.h \
    potentiometer.h \
    potsweep.h \
    resistor.h \
    settingsdialog.h \
    tonestackcalculator.h

FORMS += \
    helpbrowser.ui \
    namedialog.ui \
    settingsdialog.ui \
    tonestackcalculator.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/ngspice/ -lngspice

INCLUDEPATH += $$PWD/ngspice
DEPENDPATH += $$PWD/ngspice
