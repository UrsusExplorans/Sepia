#ifndef SEPIAGENERATORPLUGIN_H
#define SEPIAGENERATORPLUGIN_H

// RSB related
#include "basesepiagenerator.h"

// Qt related
#include <QString>

class SepiaGeneratorPluginInterface{
public:
  virtual ~SepiaGeneratorPluginInterface() {}

  virtual BaseSepiaGenerator* getGenerator(QString listFile) = 0;

  virtual QString getGeneratorName() = 0;

};

Q_DECLARE_INTERFACE(SepiaGeneratorPluginInterface,
    "de.ursusexplorans.SepiaGeneratorPluginInterface/1.0");

#endif // SEPIAGENERATORPLUGIN_H
