######################################################################
# Automatically generated by qmake (2.01a) Sun Jun 30 02:00:24 2013
######################################################################

QT -= gui
CONFIG -= debug release debug_and_release
CONFIG += debug
CONFIG += console
QMAKE_CXXFLAGS_WARN_ON = -Wall -Wextra
QMAKE_CXXFLAGS_WARN_ON = -Werror    # treat warnings as errors
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_LFLAGS += -rdynamic
#=--------------------------------
#LIBS += -L. -lDanadamLog
#QMAKE_CXXFLAGS +=
#=--------------------------------
#QT += gui
#QT += network
#QT += sql
#=--------------------------------
TEMPLATE = app
TARGET = myprog
DEPENDPATH += . danadam
INCLUDEPATH += . danadam

# Input
HEADERS += danadam/danadam.h \
           danadam/ElapsedTimer.h \
           danadam/hex.h \
           danadam/itoa.h \
           danadam/loggercommon.h \
           danadam/loggerf.h \
           danadam/loggerqt.h \
           danadam/scopeguard.h \
           danadam/scopeguard_helper.h \
           danadam/stacktrace.h \
           danadam/daalgorithm.h \
           danadam/dafunctional.h \
           danadam/sfinae.h \

SOURCES += main.cpp
