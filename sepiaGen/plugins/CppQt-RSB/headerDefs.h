#ifndef HEADERDEFS_H
#define HEADERDEFS_H

#include <QString>

QString headerHeader =
    "#include \"rsbadapter.h\"\n\n"
    "#include <Qt>\n"
    "#include <QtCore>\n"
    "#include <QString>\n\n"
    "using namespace rsb;\n\n"
    "/** @addtogroup LibTAOs\n@{*/\n\n";
QString headerCallback =
    "    /** Implemented callback function for speech commands. See\n"
    "     RSBAdapter.callbackFunc() for details.\n    */\n"
    "    virtual void callbackFunc(boost::shared_ptr<std::string> e);\n\n";

QString sourceHeader =
    "\n#include <rsb/Informer.h>\n"
    "#include <rsb/Factory.h>\n"
    "#include <QDebug>\n"
    "#include <QStringList>\n\n"
    "using namespace std;\n\n";

QString sourceConstructor =
    "%1* %1::Inst(QString scope){\n"
    "    Scope m_scope = Scope(scope.toLatin1());\n"
    "    if(!pInstanceMap.contains(scope)){\n"
    "        pInstanceMap.insert(scope, new %1(m_scope));\n"
    "    }\n"
    "    return pInstanceMap.value(scope);\n"
    "}\n\n";

QString sourceCallbackHead =
    "void %1::callbackFunc(boost::shared_ptr<std::string> e){\n"
    "    //qDebug() << Q_FUNC_INFO;\n"
    "    QString content = QString(e->c_str());\n"
    "    QStringList parts = content.split(':');\n";

QString sourceEnd =
    "%1::%1(Scope scope, QObject *parent) :\n"
    "    QObject(parent), RSBAdapter(scope){\n"
//    "    m_scope = scope;\n"
    "}\n\n"
    "QMap<QString,%1*> %1::pInstanceMap = \n"
    "   QMap<QString,%1*>();\n\n";

#endif // HEADERDEFS_H
