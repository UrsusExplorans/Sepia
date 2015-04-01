#include "basesepiagenerator.h"

#include <QDebug>

#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QDomElement>

BaseSepiaGenerator::BaseSepiaGenerator(QString listFile){
    addDebugLogging = false;
    if(listFile.endsWith(".xmlLst")){
        m_listFile = listFile;
        QFile file(listFile);
         if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
          qDebug() << "File cannot be opened:" << listFile;
          return;
        }
        while(!file.atEnd()){
            m_xmlFiles.append(file.readLine().trimmed());
        }
        file.close();
        m_xmlFiles.removeDuplicates();
        m_xmlFiles = m_xmlFiles.filter(".xml");
        m_xmlFileCount = m_xmlFiles.size();
        qDebug() << "Files to generate:" << m_xmlFiles.size();
        m_listFile = listFile;
    } else {
        m_xmlFile = listFile;
    }
    m_binaryPath =  QCoreApplication::applicationFilePath();
}

void BaseSepiaGenerator::readXmlFile(){
    QDomDocument doc("interface");
    QFile file(m_xmlFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "File cannot be opened:" << m_xmlFile;
        return;
    }

    m_query.bindVariable("interface", &file);

    getGeneralInterfaceDescription();
}

void BaseSepiaGenerator::getGeneralInterfaceDescription(){
    QString codeLine;
    codeLine = generalQuery("//interface");
    codeLine = codeLine.split("\n")[0];

    m_interfaceName = getAttributeValue("name", codeLine);
    m_interfaceScope = getAttributeValue("scope", codeLine);
}

QString BaseSepiaGenerator::getAttributeValue(QString attribute, QString codeSnippet){
    QString ret = "";
    QStringList snippetParts = codeSnippet.split(" ", QString::SkipEmptyParts);
    QString part;
    foreach(part, snippetParts){
        if(part.contains(attribute + "=")){
            ret = part.remove(attribute + "=").remove("\"").remove("'")
                    .remove("/>");
            ret = ret.remove(">").trimmed();
        }
    }
    return ret;
}

bool BaseSepiaGenerator::isBinaryNewerThanOutput(QString filename){
    bool ret = false;
    QFileInfo bin(m_binaryPath);
    QFileInfo out(filename);
    if(!out.exists()){
        ret = true;
    } else {
        if(bin.lastModified() > out.lastModified()){
            ret = true;
        }
    }
    return ret;
}

bool BaseSepiaGenerator::isXmlNewerThanOutput(QString filename){
    bool ret = false;
    QFileInfo out(filename);
    QFileInfo xml(m_xmlFile);
    if(!xml.exists()){
        qDebug() << "invalid XML file path" << m_binaryPath;
        if(!out.exists()){
            ret = true;
        }
    } else {
        if(xml.lastModified() > out.lastModified()){
            ret = true;
        }
    }
    return ret;
}

void BaseSepiaGenerator::writeToFile(QString content){
    m_file->write(content.toUtf8());
}

void BaseSepiaGenerator::closeFile(){
    m_file->close();
}


QString BaseSepiaGenerator::generalQuery(QString query){
    m_query.setQuery("declare variable $interface external;"
                     "doc($interface)" + query);
    QString result;
    bool ok = m_query.evaluateTo(&result);
    if(!ok){
      qDebug() << "Something went wrong:" << m_xmlFile << query;
    }
    return result;
}
