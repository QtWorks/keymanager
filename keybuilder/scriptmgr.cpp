// Qt
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

// Application
#include "scriptmgr.h"
#include "parameter.h"
#define VARIABLE_MARKER QString("")

//-------------------------------------------------------------------------------------------------

void ScriptMgr::generateScript(const QString &sInputScripFile, const QString &sOutputScriptFile, const QList<Parameter *> &vParameters)
{
    // Check input script file exists
    if (!QFileInfo::exists(sInputScripFile))
    {
        qDebug() << "ERROR: " << sInputScripFile << " DOES NOT EXIST";
        return;
    }

    QFile inputScriptFile(sInputScripFile);
    if (!inputScriptFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: CANNOT OPEN " << sInputScripFile;
        return;
    }
    QTextStream inStream(&inputScriptFile);
    QString sScurrentText = inStream.readAll();
    inputScriptFile.close();

    foreach (Parameter *pParameter, vParameters)
    {
        if (pParameter != nullptr)
        {
            QString sVariableName = pParameter->variable();
            QString sTargetString = VARIABLE_MARKER+sVariableName+VARIABLE_MARKER;
            sScurrentText = sScurrentText.replace(sTargetString, pParameter->value());
        }
    }

    QFile outputScriptFile(sOutputScriptFile);
    if (!outputScriptFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "ERROR: CANNOT OPEN " << sOutputScriptFile;
        return;
    }

    QTextStream outStream(&outputScriptFile);
    outStream << sScurrentText;
    outputScriptFile.close();
}
