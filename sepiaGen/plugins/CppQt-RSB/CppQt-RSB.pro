TEMPLATE = lib
CONFIG += plugin debug c++11
TARGET = cppqt-rsb-generator-plugin
DESTDIR = ../plugins

include(../../config.pri)

INCLUDEPATH += . ../..
HEADERS = ../../sepiageneratorplugin.h ../../basesepiagenerator.h \
    cppqt-rsb-generator-plugin.h \
    cppqt-rsb-generator.h headerDefs.h
DEPENDPATH += . ../..
SOURCES = ../../basesepiagenerator.cpp \
    cppqt-rsb-generator-plugin.cpp \
    cppqt-rsb-generator.cpp

# install
#target.path = $$INSTALL_PATH/bin/plugins
#INSTALLS += target

OTHER_FILES += \
    cppqt-rsb-generator-plugin.json

target.path = $$INSTALL_PATH/bin/sepia-plugins
INSTALLS += target
