#ifndef CPPQT_RSB_GENERATORPLUGIN_H
#define CPPQT_RSB_GENERATORPLUGIN_H

#include "../../sepiageneratorplugin.h"
#include <QtGlobal>

class CppQt_RSB_GeneratorPlugin : public QObject, SepiaGeneratorPluginInterface {
Q_OBJECT

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "de.ursusexplorans.de.SepiaGeneratorPluginInterface" FILE "cppqt-rsb-generator-plugin.json")
#endif
Q_INTERFACES(SepiaGeneratorPluginInterface)

public:
    BaseSepiaGenerator* getGenerator(QString listFile);

    QString getGeneratorName(){ return "CppQt-RSB-Generator"; }

};

#endif // CPPQT_RSB_GENERATORPLUGIN_H
