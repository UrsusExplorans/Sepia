#include "cppqt-rsb-generator-plugin.h"
#include "cppqt-rsb-generator.h"

BaseSepiaGenerator *CppQt_RSB_GeneratorPlugin::getGenerator(QString listFile){
    return new CppQt_RSB_Generator(listFile);
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(cppqt-rsb-generator-plugin, CppQt_RSB_GeneratorPlugin)
#endif
