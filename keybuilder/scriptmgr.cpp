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
    if (inputScriptFile.open(QIODevice::ReadOnly))
    {
        QTextStream inStream(&inputScriptFile);
        QString sCurrentText = inStream.readAll();
        inputScriptFile.close();

        foreach (Parameter *pParameter, vParameters)
        {
            if (pParameter != nullptr)
            {
                QString sParameterVariable = pParameter->variable();
                if ((pParameter->type() != PROPERTY_STRING) && (pParameter->type() != PROPERTY_BOOLEAN))
                    sCurrentText.replace(sParameterVariable, pParameter->value());
                else
                {
                    QString sQuotedVariable = QString("\"%1\"").arg(pParameter->value());
                    sCurrentText.replace(sParameterVariable, sQuotedVariable);
                }
            }
        }

        QFile outputScriptFile(sOutputScriptFile);
        if (outputScriptFile.open(QIODevice::WriteOnly))
        {
            QTextStream outStream(&outputScriptFile);
            outStream << sCurrentText;
            outputScriptFile.close();
            qDebug() << "RESULT EXPORTED IN " << sOutputScriptFile;
        }
    }
}
