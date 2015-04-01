TEMPLATE = app

include(config.pri)

target.path = $$INSTALL_PATH/bin
INSTALLS += target

CONFIG += qt console
CONFIG -= gui app_bundle

HEADERS += sepiageneratorplugin.h basesepiagenerator.h
SOURCES += sepiaGen.cpp basesepiagenerator.cpp
