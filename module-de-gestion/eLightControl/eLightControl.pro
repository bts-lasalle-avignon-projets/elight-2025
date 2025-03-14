QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    communicationbasededonnees.cpp \
    communicationsegments.cpp \
    interfacehommemachine.cpp \
    main.cpp \
    elightcontrol.cpp \
    menunavigation.cpp \
    pageaccueil.cpp \
    pagegestionscenario.cpp \
    pageguide.cpp \
    pageparametres.cpp

HEADERS += \
    communicationbasededonnees.h \
    communicationsegments.h \
    elightcontrol.h \
    interfacehommemachine.h \
    menunavigation.h \
    pageaccueil.h \
    pagegestionscenario.h \
    pageguide.h \
    pageparametres.h

# Les defines pour la version release (sans debug)
#CONFIG(release, debug|release):DEFINES+=QT_NO_DEBUG_OUTPUT RASPBERRY_PI
# Les defines pour la version debug
CONFIG(debug, debug|release):DEFINES+=
