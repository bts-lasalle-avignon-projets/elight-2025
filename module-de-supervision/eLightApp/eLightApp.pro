QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    communicationbasededonnees.cpp \
    communicationwifi.cpp \
    editionsalle.cpp \
    historique.cpp \
    main.cpp \
    elight.cpp \
    salle.cpp

HEADERS += \
    communicationbasededonnees.h \
    communicationwifi.h \
    config.h \
    editionsalle.h \
    elight.h \
    historique.h \
    salle.h

COPIES += ressources
ressources.files = $$PWD/ressources/*
ressources.path = $$OUT_PWD/ressources
INSTALLS += ressources

COPIES += configIni
configIni.files = $$PWD/config-base-de-donnees.ini
configIni.path = $$OUT_PWD
INSTALLS += configIni

# Les defines pour la version release (sans debug)
#CONFIG(release, debug|release):DEFINES+=QT_NO_DEBUG_OUTPUT RASPBERRY_PI
# Les defines pour la version debug
CONFIG(debug, debug|release):DEFINES+=
