QT       += core gui widgets network charts

CONFIG += c++17

SOURCES += \
    config.cpp \
    display.cpp \
    hantek.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    config.h \
    display.h \
    entity.h \
    hantek.h \
    mainwindow.h

FORMS += \
    config.ui \
    display.ui \
    mainwindow.ui

win32: {
RC_FILE = myapp.rc
}
