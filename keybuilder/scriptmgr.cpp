// Qt
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

// Application
#include "scriptmgr.h"
#include "parameter.h"

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

            if (sVariableName == "qt_extra_material_shape_01_use_qt")
            {
                int x = 0;
                qDebug() << sCurrentText.contains("qt_extra_material_shape_01_use_qt");
            }

            sCurrentText.replace(sVariableName, pParameter->value());
            qDebug() << sCurrentText.contains("qt_extra_material_shape_01_use_qt");
            qDebug() << sCurrentText.contains(sVariableName);
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
