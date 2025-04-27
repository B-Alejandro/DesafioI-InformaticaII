# TEMPLATE = app
# CONFIG += console c++17
# CONFIG -= app_bundle
# CONFIG -= qt

# SOURCES += \
#         main.cpp
QT += core gui
CONFIG += console c++17
SOURCES += main.cpp \
    bitabit.cpp \
    exportarimg.cpp \
    funvalidacion.cpp
HEADERS += \
    bitabit.h \
    exportarimg.h \
    funvalidacion.h
