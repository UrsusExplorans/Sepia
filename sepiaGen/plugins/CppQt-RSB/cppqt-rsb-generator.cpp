#include "cppqt-rsb-generator.h"
#include "headerDefs.h"

#include <QDebug>

#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QDomElement>

CppQt_RSB_Generator::CppQt_RSB_Generator(QString listFile) :
    BaseSepiaGenerator(listFile){ }

void CppQt_RSB_Generator::generateFiles(){
    if(!m_listFile.isEmpty()){
        prepareIncFiles();
        while(m_xmlFileCount > 0){
            m_xmlFile = m_xmlFiles.at(m_xmlFiles.size() - m_xmlFileCount);
            m_outputFileName = m_xmlFile.mid(0, m_xmlFile.size()-4);
            m_outputFileNameMasterH   = m_outputFileName + "master.h";
            m_outputFileNameMasterCpp = m_outputFileName + "master.cpp";
            m_outputFileNameSlaveH    = m_outputFileName + "slave.h";
            m_outputFileNameSlaveCpp  = m_outputFileName + "slave.cpp";

            readXmlFile();

            m_className = m_interfaceName + "Master";
            if(prepareOutputFile(m_outputFileNameMasterH)){
                qDebug() << "Generating" << m_outputFileNameMasterH;
                generateMasterHeaderFile();
                closeFile();
            }
            if(prepareOutputFile(m_outputFileNameMasterCpp)){
                qDebug() << "Generating" << m_outputFileNameMasterCpp;
                generateMasterSourceFile();
                closeFile();
            }
            appendToIncFile(m_outputFileNameMasterH);

            m_className = m_interfaceName + "Slave";
            if(prepareOutputFile(m_outputFileNameSlaveH)){
                qDebug() << "Generating" << m_outputFileNameSlaveH;
                generateSlaveHeaderFile();
                closeFile();
            }
            if(prepareOutputFile(m_outputFileNameSlaveCpp)){
                qDebug() << "Generating" << m_outputFileNameSlaveCpp;
                generateSlaveSourceFile();
                closeFile();
            }
            appendToIncFile(m_outputFileNameSlaveH);
            m_xmlFileCount--;
        }
        closeIncFiles();
    } else {
        m_outputFileName = m_xmlFile.mid(0, m_xmlFile.size()-4);
        m_outputFileNameMasterH   = m_outputFileName + "master.h";
        m_outputFileNameMasterCpp = m_outputFileName + "master.cpp";
        m_outputFileNameSlaveH    = m_outputFileName + "slave.h";
        m_outputFileNameSlaveCpp  = m_outputFileName + "slave.cpp";

        readXmlFile();
        m_className = m_interfaceName + "Master";
        if(prepareOutputFile(m_outputFileNameMasterH)){
            qDebug() << "Generating" << m_outputFileNameMasterH;
            generateMasterHeaderFile();
            closeFile();
        }

        if(prepareOutputFile(m_outputFileNameMasterCpp)){
            qDebug() << "Generating" << m_outputFileNameMasterCpp;
            generateMasterSourceFile();
            closeFile();
        }

        m_className = m_interfaceName + "Slave";
        if(prepareOutputFile(m_outputFileNameSlaveH)){
            qDebug() << "Generating" << m_outputFileNameSlaveH;
            generateSlaveHeaderFile();
            closeFile();
        }

        if(prepareOutputFile(m_outputFileNameSlaveCpp)){
            qDebug() << "Generating" << m_outputFileNameSlaveCpp;
            generateSlaveSourceFile();
            closeFile();
        }
    }
}

void CppQt_RSB_Generator::generateMasterHeaderFile(){
  writeToFile(QString("#ifndef %1_H\n").arg(m_className));
  writeToFile(QString("#define %1_H\n\n").arg(m_className));
  writeToFile(headerHeader);
  writeToFile(QString("class %1").arg(m_className)
              + " : public QObject, public RSBAdapter {\nQ_OBJECT\n\npublic:\n");
  writeToFile(QString("    static %1*").arg(m_className)
              + " Inst(QString scope = QString(\""
              + m_interfaceScope +"\"));\n\n");
  writeToFile(headerCallback);

  generateMasterHeaderSlots();
  generateMasterHeaderSignals();

  writeToFile("protected:\n");
  writeToFile("    " + m_className + "(Scope scope = Scope(\"" + m_interfaceScope
              + "\"), QObject *parent=0);\n\n");
  writeToFile("private:\n");
  writeToFile("    static QMap<QString," + m_className + "*> pInstanceMap;\n");
//  writeToFile("    Scope m_scope;");
  writeToFile("\n\n};\n\n/**@}*/\n");
  writeToFile(QString("\n#endif // %1_H\n").arg(m_className));
}

void CppQt_RSB_Generator::generateSlaveHeaderFile(){
  writeToFile(QString("#ifndef %1_H\n").arg(m_className));
  writeToFile(QString("#define %1_H\n\n").arg(m_className));
  writeToFile(headerHeader);
  writeToFile(QString("class %1").arg(m_className)
              + " : public QObject, public RSBAdapter {\nQ_OBJECT\n\npublic:\n");
  writeToFile(QString("    static %1*").arg(m_className)
              + " Inst(QString scope = QString(\""
              + m_interfaceScope +"\"));\n\n");
//  writeToFile("    static Scope m_scope;\n\n");
  writeToFile(headerCallback);

  generateSlaveHeaderSlots();
  generateSlaveHeaderSignals();

  writeToFile("protected:\n");
  writeToFile("    " + m_className + "(Scope scope = Scope(\"" + m_interfaceScope
              + "\"), QObject *parent=0);\n\n");
  writeToFile("private:\n");
  writeToFile("    static QMap<QString," + m_className + "*> pInstanceMap;\n");
//  writeToFile("    static Scope m_scope;");
  writeToFile("\n\n};\n\n/**@}*/\n");
  writeToFile(QString("\n#endif // %1_H\n").arg(m_className));
}

void CppQt_RSB_Generator::generateMasterHeaderSlots(){
  QString xpath = "//interface/masterSlots/slot";
  QStringList slotsList = generalQuery(xpath).split("<slot", QString::SkipEmptyParts);
  if(!slotsList.isEmpty()){
    writeToFile("public slots:\n");
    QString slot;
    foreach (slot, slotsList){
      generateHeaderSlot(slot.remove("</slot>").trimmed());
    }
    writeToFile("\n");
  }
}

void CppQt_RSB_Generator::generateSlaveHeaderSlots(){
  QString xpath = "//interface/masterSignals/signal";
  QStringList slotsList = generalQuery(xpath).split("<signal", QString::SkipEmptyParts);
  if(!slotsList.isEmpty()){
    writeToFile("public slots:\n");
    QString slot;
    foreach (slot, slotsList){
      generateHeaderSlot(slot.remove("</signal>").trimmed());
    }
    writeToFile("\n");
  }
}

void CppQt_RSB_Generator::generateHeaderSlot(QString slotDescription){
    if(!slotDescription.isEmpty()){
      QStringList slotParts = slotDescription.split("\n");
      QString slotName = getAttributeValue("name", slotParts.takeFirst());
      writeToFile("    void " + slotName + "(");

      QString arg; int argnum = slotParts.size();
      foreach (arg, slotParts){
        argnum--;
        QString argType = getAttributeValue("type", arg);
        QString argName = getAttributeValue("name", arg);
        writeToFile(argType + " " + argName);
        if(argnum != 0){
          writeToFile(", ");
        }
      }
      writeToFile(");\n");
    }
}

void CppQt_RSB_Generator::generateMasterHeaderSignals(){
    QString xpath = "//interface/masterSignals/signal";
    QStringList signalsList = generalQuery(xpath).split("<signal", QString::SkipEmptyParts);
    if(!signalsList.isEmpty()){
        writeToFile("signals:\n");
      QString signal;
      foreach (signal, signalsList){
        generateHeaderSignal(signal.remove("</signal>").trimmed());
      }
      writeToFile("\n");
    }
}

void CppQt_RSB_Generator::generateSlaveHeaderSignals(){
    QString xpath = "//interface/masterSlots/slot";
    QStringList signalsList = generalQuery(xpath).split("<slot", QString::SkipEmptyParts);
    if(!signalsList.isEmpty()){
        writeToFile("signals:\n");
      QString signal;
      foreach (signal, signalsList){
        generateHeaderSignal(signal.remove("</slot>").trimmed());
      }
      writeToFile("\n");
    }
}

void CppQt_RSB_Generator::generateHeaderSignal(QString signalDescription){
    if(!signalDescription.isEmpty()){
      QStringList signalParts = signalDescription.split("\n");
      QString signalName = getAttributeValue("name", signalParts.takeFirst());
      writeToFile("    void " + signalName + "(");

      QString arg; int argnum = signalParts.size();
      foreach (arg, signalParts){
        argnum--;
        QString argType = getAttributeValue("type", arg);
        QString argName = getAttributeValue("name", arg);
        writeToFile(argType + " " + argName);
        if(argnum != 0){
          writeToFile(", ");
        }
      }
      writeToFile(");\n");
    }
}


void CppQt_RSB_Generator::generateMasterSourceFile(){
  writeToFile(QString("#include \"%1\"\n").arg(m_outputFileNameMasterH.split("/").last()));
  writeToFile(sourceHeader);
//  writeToFile(QString("%1* %1::pInstance = NULL;\n\n").arg(m_className));
  writeToFile(sourceConstructor.arg(m_className));

  writeToFile(sourceCallbackHead.arg(m_className));
  generateSourceMasterCBEmits();
  generateSourceMasterSlots();

  writeToFile("\n" + sourceEnd.arg(m_className));
}

void CppQt_RSB_Generator::generateSlaveSourceFile(){
  writeToFile(QString("#include \"%1\"\n").arg(m_outputFileNameSlaveH.split("/").last()));
  writeToFile(sourceHeader);
//  writeToFile(QString("%1* %1::pInstance = NULL;\n\n").arg(m_className));
  writeToFile(sourceConstructor.arg(m_className));

  writeToFile(sourceCallbackHead.arg(m_className));
  generateSourceSlaveCBEmits();
  generateSourceSlaveSlots();

  writeToFile("\n" + sourceEnd.arg(m_className));
}

void CppQt_RSB_Generator::generateSourceMasterCBEmits(){
  QString xpath = "//interface/masterSignals/signal";
  QStringList signalsList = generalQuery(xpath).split("<signal", QString::SkipEmptyParts);
  if(!signalsList.isEmpty()){
    QString signal;
    foreach (signal, signalsList){
      generateSourceCBEmit(signal.remove("</signal>").trimmed());
    }
    writeToFile("}\n\n");
  }
}

void CppQt_RSB_Generator::generateSourceMasterSlots(){
  QString xpath = "//interface/masterSlots/slot";
  QStringList slotsList = generalQuery(xpath).split("<slot", QString::SkipEmptyParts);
  if(!slotsList.isEmpty()){
    QString slot;
    foreach (slot, slotsList){
      generateSourceSlot(slot.remove("</slot>").trimmed());
    }
  }
}

void CppQt_RSB_Generator::generateSourceSlaveCBEmits(){
  QString xpath = "//interface/masterSlots/slot";
  QStringList signalsList = generalQuery(xpath).split("<slot", QString::SkipEmptyParts);
  if(!signalsList.isEmpty()){
    QString signal;
    foreach (signal, signalsList){
      generateSourceCBEmit(signal.remove("</slot>").trimmed());
    }
    writeToFile("}\n\n");
  }
}

void CppQt_RSB_Generator::generateSourceSlaveSlots(){
  QString xpath = "//interface/masterSignals/signal";
  QStringList signalsList = generalQuery(xpath).split("<signal", QString::SkipEmptyParts);
  if(!signalsList.isEmpty()){
    QString signal;
    foreach (signal, signalsList){
      generateSourceSlot(signal.remove("</signal>").trimmed());
    }
//    writeToFile("}");
    writeToFile("\n\n");
  }
}

void CppQt_RSB_Generator::generateSourceCBEmit(QString signalDescription){
    if(!signalDescription.isEmpty()){
      QStringList signalParts = signalDescription.split("\n");
      QString signalName = getAttributeValue("name", signalParts.takeFirst());
      writeToFile("    if(parts.size() == " + QString::number(signalParts.size()+1)
                  + " && parts[0]==\"" + signalName + "\"){\n");
      writeToFile("        emit(" + signalName + "(");
      QString arg; int argnum = signalParts.size();
      if(argnum > 0){
        writeToFile("\n");
      }
      foreach (arg, signalParts){
        argnum--;
        QString argType = getAttributeValue("type", arg);
        QString argName = getAttributeValue("name", arg);
        if(argType == "QString"){
          writeToFile("            parts["
                      + QString::number(signalParts.size() - argnum)
                      + "]");
        }
        if(argType == "bool"){
          writeToFile("            (parts["
                      + QString::number(signalParts.size() - argnum)
                      + "] == \"true\")");
        }
        if(argType == "int"){
          writeToFile("            parts["
                      + QString::number(signalParts.size() - argnum)
                      + "].toInt()");
        }
        if(argType == "double"){
          writeToFile("            parts["
                      + QString::number(signalParts.size() - argnum)
                      + "].toDouble()");
        }
        if(argType == "float"){
          writeToFile("            parts["
                      + QString::number(signalParts.size() - argnum)
                      + "].toFloat()");
        }
        if(argType == "QRectF"){
          QString curPart = QString::number(signalParts.size() - argnum);
          writeToFile(QString("            QRectF(\n")
                      + "              parts[" + curPart + "].split(\";\")[0].toDouble(),\n"
                      + "              parts[" + curPart + "].split(\";\")[1].toDouble(),\n"
                      + "              parts[" + curPart + "].split(\";\")[2].toDouble(),\n"
                      + "              parts[" + curPart + "].split(\";\")[3].toDouble())"
                      );
        }
        if(argnum != 0){
          writeToFile(",");
        }
        writeToFile("   // " + argName + "\n");
      }
      if(signalParts.size() > 0){
        writeToFile("        ");
      }
      writeToFile("));\n    }\n");
    }
}

void CppQt_RSB_Generator::generateSourceSlot(QString slotDescription){
    if(!slotDescription.isEmpty()){
      QStringList slotParts = slotDescription.split("\n");
      QString slotName = getAttributeValue("name", slotParts.takeFirst());
      writeToFile("void " + m_className + "::" + slotName + "(");

      QString arg; int argnum = slotParts.size();
      foreach (arg, slotParts){
        argnum--;
        QString argType = getAttributeValue("type", arg);
        QString argName = getAttributeValue("name", arg);
        writeToFile(argType + " " + argName);
        if(argnum != 0){
          writeToFile(", ");
        }
      }
      writeToFile("){\n");
      writeToFile("    Scope curScope = Scope(pInstanceMap.key(this).toLatin1());\n");
      if(addDebugLogging){
        writeToFile("    qDebug() << Q_FUNC_INFO << QString::fromStdString(curScope.toString());\n");
      }
      writeToFile("    sendWithRSB(curScope.toString().c_str(), \"" + slotName
                  + ":\"");

      argnum = slotParts.size();
      foreach (arg, slotParts){
        argnum--;
        QString argType = getAttributeValue("type", arg);
        QString argName = getAttributeValue("name", arg);
        if(argnum == slotParts.size()-1){
          writeToFile("\n        +       ");
        } else {
          writeToFile("\n        + \":\" + ");
        }
        if(argType == "int" || argType == "double" || argType == "float"){
          writeToFile("QString::number(" + argName + ")");
        }
        if(argType == "QRectF"){
          writeToFile("QString::number(" + argName + ".x()) + \";\"\n");
          writeToFile("              + QString::number(" + argName + ".y()) + \";\"\n");
          writeToFile("              + QString::number(" + argName + ".width()) + \";\"\n");
          writeToFile("              + QString::number(" + argName + ".height())");
        }
        if(argType == "QString"){
          writeToFile(argName);
        }
        if(argType == "bool"){
          writeToFile("(" + argName + " ? \"true\" : \"false\")");
        }
      }

      writeToFile(");\n}\n\n");
    }
}

void CppQt_RSB_Generator::clean(){
    if(!m_listFile.isEmpty()){
        while(m_xmlFileCount > 0){
            m_xmlFile = m_xmlFiles.at(m_xmlFiles.size() - m_xmlFileCount);
            m_outputFileName = m_xmlFile.mid(0, m_xmlFile.size()-4);
            m_outputFileNameMasterH   = m_outputFileName + "master.h";
            m_outputFileNameMasterCpp = m_outputFileName + "master.cpp";
            m_outputFileNameSlaveH    = m_outputFileName + "slave.h";
            m_outputFileNameSlaveCpp  = m_outputFileName + "slave.cpp";

            readXmlFile();

            qDebug() << "Removing" << m_outputFileNameMasterH;
            QFile::remove(m_outputFileNameMasterH);
            qDebug() << "Removing" << m_outputFileNameMasterCpp;
            QFile::remove(m_outputFileNameMasterCpp);
            qDebug() << "Removing" << m_outputFileNameSlaveH;
            QFile::remove(m_outputFileNameSlaveH);
            qDebug() << "Removing" << m_outputFileNameSlaveCpp;
            QFile::remove(m_outputFileNameSlaveCpp);

            m_xmlFileCount--;
        }
    }
}


bool CppQt_RSB_Generator::prepareOutputFile(QString filename){
    bool ret = false;
    if((isBinaryNewerThanOutput(filename) || isXmlNewerThanOutput(filename)) ||
            ALWAYS_WRITE_OUTPUT){
        m_file = new QFile(filename);
        if(m_file->open(QFile::WriteOnly | QFile::Truncate)){
            ret = true;
//            qDebug() << "file open";
        } else {
            qDebug() << "unable to open file: " << filename;
        }
    } else {
      qDebug() << "nothing to do:" << filename << "is up to date.";
    }
    return ret;
}

void CppQt_RSB_Generator::prepareIncFiles(){
    QString filename = QString(m_listFile).replace(".xmlLst", "Headers.pri");
    m_headerIncFile = new QFile(filename);
    if(!m_headerIncFile->open(QFile::WriteOnly | QFile::Truncate)){
        qDebug() << "unable to open file: " << filename;
    }
    m_headerIncFile->write("HEADERS +=");

    filename = QString(m_listFile).replace(".xmlLst", "Sources.pri");
    m_sourceIncFile = new QFile(filename);
    if(!m_sourceIncFile->open(QFile::WriteOnly | QFile::Truncate)){
        qDebug() << "unable to open file: " << filename;
    }
    m_sourceIncFile->write("SOURCES +=");
}

void CppQt_RSB_Generator::appendToIncFile(QString filename){
    QString incFile = filename.split(".").at(0);
    m_headerIncFile->write((" \\\n" + incFile + ".h").toUtf8());
    m_sourceIncFile->write((" \\\n" + incFile + ".cpp").toUtf8());
}

void CppQt_RSB_Generator::closeIncFiles(){
    m_headerIncFile->close();
    m_sourceIncFile->close();
}
