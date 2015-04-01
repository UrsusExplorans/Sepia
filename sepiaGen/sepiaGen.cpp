#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include "sepiageneratorplugin.h"
#include "basesepiagenerator.h"

QMap<QString, SepiaGeneratorPluginInterface*> m_generatorPlugins;

void loadPlugins(){
    QDir pluginsDir(qApp->applicationDirPath());
    qDebug() << pluginsDir.absolutePath();
    pluginsDir.cd("sepia-plugins");
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)){
      qDebug() << pluginsDir.absoluteFilePath(fileName);
      QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
      QObject *plugin = pluginLoader.instance(); // TODO: does not seem to work...
      if(!pluginLoader.isLoaded()){
        qDebug() << pluginLoader.errorString();
      } else {
        qDebug() << "successfully loaded\n";
      }
      if(plugin){
        SepiaGeneratorPluginInterface *generator
            = qobject_cast<SepiaGeneratorPluginInterface *>(plugin);
        if(generator){
            m_generatorPlugins.insert(generator->getGeneratorName(), generator);
        }
      }
    }

}


int main(int argc, char **args){
    QCoreApplication app(argc, args);

    loadPlugins();

    if(app.arguments().count() < 3){
        std::cout << "Sepia Interface Generator\n"
                     "Needs an interface description file"
                     "and a generator plugin name for generating code.\n"
                  << std::flush;
        return 0;
    }

    QString listFile = app.arguments().filter(".xmlLst")[0];
    QString plugin = app.arguments().filter("-Generator")[0];
    qDebug() << "Generating files from" << listFile << "using" << plugin;
    BaseSepiaGenerator *sepiaGen =
            m_generatorPlugins.value(plugin)->getGenerator(listFile);
    if(app.arguments().contains("-clean")){
        sepiaGen->clean();
    } else {
        sepiaGen->generateFiles();
    }

    return 0;
}
