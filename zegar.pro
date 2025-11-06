#-------------------------------------------------
#
# Project created by QtCreator 2020-05-05T23:21:54
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += staticlib
CONFIG += static

#QMAKE_CXXFLAGS += -std=c++17 -g
QMAKE_CXXFLAGS += -std=c++17

TARGET = zegar
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


TRANSLATIONS += polski2.ts
LUPDATE += -no-obsolete


# Automatyczna kompilacja translacji do .qm
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_NAME} -qm ${QMAKE_FILE_OUT}
updateqm.CONFIG += no_link target_predeps

QMAKE_EXTRA_COMPILERS += updateqm




SOURCES += main.cpp\
        mainwindow.cpp \
    alarmy.cpp \
    tedytor_alarmow_dlg.cpp \
    tjeden_alarm.cpp \
    togloszenie_alarmu.cpp \
    topcje_dlg.cpp \
    tpreferred_hands.cpp

HEADERS  += mainwindow.h \
    alarmy.h \
    tedytor_alarmow_dlg.h \
    tjeden_alarm.h \
    togloszenie_alarmu.h \
    topcje_dlg.h \
    tpreferred_hands.h

FORMS    += mainwindow.ui \
    alarmy.ui \
    tedytor_alarmow_dlg.ui \
    togloszenie_alarmu.ui \
    topcje_dlg.ui



RESOURCES += \
    translations.qrc \
    zegar_elementy.qrc


DISTFILES += polski2.qm \
                content/*.png


DISTFILES += \
    content/iwc_schaffhausen_short.png \
    jak_robic_wskazowki.txt \
    how_to_make_translation.txt

