#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

// Application
class Parameter;

class ScriptMgr
{
public:
    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Generate script
    static bool generateScript(const QString &sInputScripFile, const QString &sOutputScriptFile, const QList<Parameter *> &vParameters);

    //! Read DXF templates
    static void replaceDXFTemplates(QString &sCurrentText);
};

#endif // SCRIPTMGR_H
