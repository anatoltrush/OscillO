QT       += core gui widgets network charts

CONFIG += c++17

SOURCES += \
    config.cpp \
    display.cpp \
    estim.cpp \
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
    estim.h \
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
    estim.ui \
    info.ui \
    logger.ui \
    mainwindow.ui

win32: {
RC_FILE = myapp.rc
}
