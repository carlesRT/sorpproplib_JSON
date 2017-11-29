#-------------------------------------------------
#
# Project created by QtCreator 2017-06-11T20:18:31
#
#-------------------------------------------------

QT       -= gui

CONFIG += c++11
TARGET = sorpPropLib
TEMPLATE = lib

DEFINES += SORPPROPLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        sorpproplib.cpp \
    ref_water.cpp \
    eqn_nrtl.cpp \
    pair_template.cpp \
    pair_water_libr.cpp \
    eqn_duhring.cpp \
    eqn_antoine.cpp \
    eqn_toth.cpp \
    eqn_template.cpp \
    pair_water_silicagel.cpp \
    pair_water_naoh.cpp \
    ref_co2.cpp \
    ref_template.cpp \
    pair_co2_silicagel.cpp \
    ref_propylene.cpp \
    pair_co2_carbon.cpp \
    pair_propylene_zeolite.cpp \
    pair_co2_zeolite.cpp \
    pair_water_zeolite.cpp \
    pair_propylene_carbon.cpp \
    ref_propane.cpp \
    pair_propane_carbon.cpp \
    pair_propane_zeolite.cpp \
    ref_butene.cpp \
    pair_butene_zeolite.cpp \
    ref_hfo1234ze.cpp \
    pair_hfo1234ze_carbon.cpp \
    ref_hfc134a.cpp \
    pair_hfc134a_carbon.cpp \
    pair_water_licl.cpp \
    eos_water.cpp \
    pair_water_cacl2.cpp

HEADERS += \
        sorpproplib.h \
        sorpproplib_global.h \ 
    ref_water.h \
    eqn_nrtl.h \
    pair_template.h \
    pair_water_libr.h \
    eqn_duhring.h \
    eqn_antoine.h \
    eqn_toth.h \
    eqn_template.h \
    pair_water_silicagel.h \
    pair_water_naoh.h \
    ref_co2.h \
    ref_template.h \
    pair_co2_silicagel.h \
    ref_propylene.h \
    pair_co2_carbon.h \
    pair_co2_zeolite.h \
    pair_propylene_zeolite.h \
    pair_water_zeolite.h \
    pair_propylene_carbon.h \
    ref_propane.h \
    pair_propane_carbon.h \
    pair_propane_zeolite.h \
    ref_butene.h \
    pair_butene_zeolite.h \
    ref_hfo1234ze.h \
    pair_hfo1234ze_carbon.h \
    ref_hfc134a.h \
    pair_hfc134a_carbon.h \
    pair_water_licl.h \
    eos_water.h \
    pair_water_cacl2.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
