TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    eval.c \
    board.c \
    search.c \
    data.c \
    utils.c

HEADERS += \
    defs.h \
    data.h \
    protos.h

