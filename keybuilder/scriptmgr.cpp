// Qt
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

// Application
#include "scriptmgr.h"
#include "parameter.h"
#include "constants.h"
#include "helper.h"
#include "utils.h"

//-------------------------------------------------------------------------------------------------

bool ScriptMgr::generateScript(const QString &sInputScripFile, const QString &sOutputScriptFile, const QList<Parameter *> &vParameters)
{
    // Check input script file exists
    if (!QFileInfo::exists(sInputScripFile))
    {
        QString sMsg = QString("%1 DOES NOT EXIST").arg(sInputScripFile);
        logError(sMsg);
        return false;
    }

    QString sCurrentText = Utils::loadFile(sInputScripFile);
    if (!sCurrentText.isEmpty())
    {
        // Do replacements
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

        // Save file
        if (Utils::saveFile(sCurrentText, sOutputScriptFile))
        {
            QString sMsg = QString("SCAD SCRIPT SUCCESSFULLY EXPORTED IN: %1").arg(sOutputScriptFile);
            logInfo(sMsg);
            return true;
        }
        else
        {
            QString sMsg = QString("COULD NOT EXPORT SCAD SCRIPT TO: %1").arg(sOutputScriptFile);
            logError(sMsg);
            return false;
        }
    }

    return false;
}
