#ifndef BASESEPIAGENERATOR_H
#define BASESEPIAGENERATOR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QXmlQuery>

class BaseSepiaGenerator : public QObject{
Q_OBJECT

public:
    BaseSepiaGenerator(QString listFile);

    virtual void readXmlFile();

    virtual void generateFiles() = 0;
    virtual void clean() = 0;

protected:
    bool addDebugLogging;

    QString m_binaryPath;
    QString m_listFile;
    QStringList m_xmlFiles;
    QString m_xmlFile;
    int m_xmlFileCount;
    QFile *m_file;
    QString m_xmlCode;

    QString m_interfaceName;
    QString m_interfaceScope;
    QString m_className;

    QXmlQuery m_query;

    virtual void getGeneralInterfaceDescription();
    QString generalQuery(QString query);
    QString getAttributeValue(QString attribute, QString codeSnippet);

    virtual bool isBinaryNewerThanOutput(QString filename); // TODO: apply to plugin, as well!
    virtual bool isXmlNewerThanOutput(QString filename); // TODO: apply to plugin, as well!

    virtual void writeToFile(QString content);
    virtual void closeFile();

};

#endif // BASESEPIAGENERATOR_H
