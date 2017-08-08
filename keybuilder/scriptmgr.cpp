// Qt
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

// Application
#include "scriptmgr.h"
#include "parameter.h"
#include "constants.h"

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
    QString sCurrentText = inStream.readAll();
    inputScriptFile.close();

    foreach (Parameter *pParameter, vParameters)
    {
        if (pParameter != nullptr)
        {
            QString sVariableName = pParameter->variable();
            if ((pParameter->type() != PROPERTY_STRING) && (pParameter->type() != PROPERTY_BOOLEAN))
                sCurrentText.replace(sVariableName, pParameter->value());
            else
            {
                QString sQuotedVariable = QString("\"%1\"").arg(pParameter->value());
                sCurrentText.replace(sVariableName, sQuotedVariable);
            }
        }
    }

    QFile outputScriptFile(sOutputScriptFile);
    if (!outputScriptFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "ERROR: CANNOT OPEN " << sOutputScriptFile;
        return;
    }

    QTextStream outStream(&outputScriptFile);
    outStream << sCurrentText;
    outputScriptFile.close();
}
