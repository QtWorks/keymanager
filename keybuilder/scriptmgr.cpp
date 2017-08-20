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
#define DISLOCKPRO_TEMPLATE "qt_dislockpro_template_qt"
#define PROTEC_TEMPLATE "qt_protect_template_qt"
#define ABUSPLUS_TEMPLATE "qt_abusplus_template_qt"
#define AXI_TEMPLATE "qt_axi_template_qt"
#define VIERGE_TEMPLATE "qt_787svierge_qt"

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
        // Replace dxf templates
        replaceDXFTemplates(sCurrentText);

        // Do replacements
        foreach (Parameter *pParameter, vParameters)
        {
            if (pParameter != nullptr)
            {
                QString sParameterVariable = pParameter->variable();
                QString sParameterValue = pParameter->value();
                if (sParameterValue.isEmpty())
                    sParameterValue = pParameter->defaultValue();
                if ((pParameter->type() != PROPERTY_STRING) && (pParameter->type() != PROPERTY_BOOLEAN))
                    sCurrentText.replace(sParameterVariable, sParameterValue);
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

//-------------------------------------------------------------------------------------------------

void ScriptMgr::replaceDXFTemplates(QString &sCurrentText)
{
    QDir dataDirectory = Utils::dataDirectory();
    QString sDislockProTemplate = QString("\"%1\"").arg(dataDirectory.absoluteFilePath("disklockpro.dxf"));
    QString sProtecTemplate = QString("\"%1\"").arg(dataDirectory.absoluteFilePath("protec.dxf"));
    QString sAbusPlusTemplate = QString("\"%1\"").arg(dataDirectory.absoluteFilePath("abusplus.dxf"));
    QString sAxiTemplate = QString("\"%1\"").arg(dataDirectory.absoluteFilePath("axi.dxf"));
    QString sViergeTemplate = QString("\"%1\"").arg(dataDirectory.absoluteFilePath("787Svierge.dxf"));

    sCurrentText.replace(DISLOCKPRO_TEMPLATE, sDislockProTemplate);
    sCurrentText.replace(PROTEC_TEMPLATE, sProtecTemplate);
    sCurrentText.replace(ABUSPLUS_TEMPLATE, sAbusPlusTemplate);
    sCurrentText.replace(AXI_TEMPLATE, sAxiTemplate);
    sCurrentText.replace(VIERGE_TEMPLATE, sViergeTemplate);
}
