#ifndef CPPQT_RSB_GENERATOR_H
#define CPPQT_RSB_GENERATOR_H

#include "../../basesepiagenerator.h"

#include <Qt>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QXmlQuery>

#define ALWAYS_WRITE_OUTPUT false

class CppQt_RSB_Generator : public BaseSepiaGenerator {

public:
    CppQt_RSB_Generator(QString listFile);

//    void readXmlFile();
    void generateFiles();

    void generateMasterHeaderFile();
    void generateSlaveHeaderFile();
    void generateMasterHeaderSlots();
    void generateSlaveHeaderSlots();
    void generateMasterHeaderSignals();
    void generateSlaveHeaderSignals();

    void generateMasterSourceFile();
    void generateSlaveSourceFile();
    void generateSourceMasterCBEmits();
    void generateSourceMasterSlots();
    void generateSourceSlaveCBEmits();
    void generateSourceSlaveSlots();

    void generateHeaderSlot(QString slotDescription);
    void generateHeaderSignal(QString signalDescription);
    void generateSourceCBEmit(QString signalDescription);
    void generateSourceSlot(QString slotDescription);

    void clean();


private:
    QString m_outputFileName;
    QString m_outputFileNameMasterH;
    QString m_outputFileNameMasterCpp;
    QString m_outputFileNameSlaveH;
    QString m_outputFileNameSlaveCpp;
    QFile *m_headerIncFile;
    QFile *m_sourceIncFile;

    bool prepareOutputFile(QString filename);
    void prepareIncFiles();
    void appendToIncFile(QString filename);
    void closeIncFiles();
};

#endif // SEPIAGENERATOR_H
