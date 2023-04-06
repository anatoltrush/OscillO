QT       += core gui widgets network charts

CONFIG += c++17

SOURCES += \
    config.cpp \
    display.cpp \
    hantek.cpp \
    info.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    techlogger.cpp \
    wrapjson.cpp \
    wrapserver.cpp

HEADERS += \
    config.h \
    display.h \
    entity.h \
    hantek.h \
    info.h \
    logger.h \
    mainwindow.h \
    techlogger.h \
    wrapjson.h \
    wrapserver.h

FORMS += \
    config.ui \
    display.ui \
    info.ui \
    logger.ui \
    mainwindow.ui

win32: {
RC_FILE = myapp.rc
}
