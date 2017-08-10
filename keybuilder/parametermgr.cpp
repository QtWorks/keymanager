// Qt
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QScriptEngine>
#include <QFile>
#include <QDebug>

// Application
#include "parametermgr.h"
#include "parameter.h"
#include "constants.h"
#include "scriptmgr.h"

//-------------------------------------------------------------------------------------------------

ParameterMgr::ParameterMgr(QObject *parent) : QObject(parent),
    m_pController(nullptr)
{

}

//-------------------------------------------------------------------------------------------------

ParameterMgr::~ParameterMgr()
{
    qDeleteAll(m_hParameters);
    m_hParameters.clear();
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::parseSingleBlock(const CXMLNode &xBlock)
{
    // Parse block parameters
    QVector<CXMLNode> vParameterNodes = xBlock.getNodesByTagName(TAG_PARAMETER);
    foreach (CXMLNode xParameterNode, vParameterNodes)
    {
        QString sParameterName = xParameterNode.attributes()[PROPERTY_NAME];
        if (sParameterName.simplified().isEmpty())
        {
            qDebug() << "WARNING: FOUND A PARAMETER WITH AN EMPTY NAME";
        }
        QString sParameterType = xParameterNode.attributes()[PROPERTY_TYPE];
        if (sParameterType.simplified().isEmpty())
        {
            qDebug() << "WARNING: FOUND A PARAMETER WITH AN UNDEFINED TYPE, DEFAULTING TO STRING";
            sParameterType = PROPERTY_STRING;
        }
        QString sParameterVariable = xParameterNode.attributes()[PROPERTY_VARIABLE];
        if (sParameterVariable.simplified().isEmpty() && (sParameterType != PROPERTY_TABLE))
        {
            qDebug() << "ERROR: FOUND A PARAMETER WITH AN UNDEFINED VARIABLE";
            continue;
        }
        QString sDefaultValue = xParameterNode.attributes()[PROPERTY_DEFAULT];
        if (sDefaultValue.simplified().isEmpty())
        {
            sDefaultValue = PROPERTY_DEFAULT_VALUE;
            qDebug() << "WARNING: FOUND A PARAMETER WITH AN UNDEFINED DEFAULT VALUE. Defaulting to \"0\"";
        }
        QString sAutoScript = xParameterNode.attributes()[PROPERTY_AUTO];
        if (sAutoScript.simplified().isEmpty())
        {
            qDebug() << "INFORMATION: FOUND A PARAMETER WITH AN UNDEFINED AUTO SCRIPT";
        }
        QString sParameterUI = xParameterNode.attributes()[PROPERTY_UI];
        if (sParameterUI.simplified().isEmpty())
        {
            qDebug() << "WARNING: FOUND A PARAMETER WITH AN UNDEFINED UI";
        }
        QString sEnabledCondition = xParameterNode.attributes()[PROPERTY_ENABLED];
        if (sEnabledCondition.simplified().isEmpty())
        {
            qDebug() << "INFORMATION: FOUND A PARAMETER WITH AN UNDEFINED ENABLED CONDTION";
        }

        // Special case for table
        if (sParameterType == PROPERTY_TABLE)
        {
            parseTableParameters(xParameterNode);
        }
        else
        {
            if (sParameterUI != WIDGET_DXF_OR_STL_FILE_PICKER)
            {
                if (!m_hParameters.contains(sParameterVariable))
                    m_hParameters[sParameterVariable] = new Parameter(sParameterName, sParameterType, sParameterVariable, sDefaultValue, sAutoScript, sEnabledCondition);
            }
        }
    }

    // Parse child blocks
    QVector<CXMLNode> vChildBlocks = xBlock.getNodesByTagName(TAG_BLOCK);
    foreach (CXMLNode xChildBlock, vChildBlocks)
        parseSingleBlock(xChildBlock);
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::parseTableParameters(const CXMLNode &xParameter)
{
    QString sColumnLabels = xParameter.attributes()[PROPERTY_COLUMN_LABELS].simplified();
    QStringList lColumnLabels = sColumnLabels.split(",");
    QString sColumnVariables = xParameter.attributes()[PROPERTY_COLUMN_VARIABLES].simplified();
    QStringList lColumnVariables = sColumnVariables.split(",");
    QString sParameterType = PROPERTY_DOUBLE;
    QString sVariableMethod = xParameter.attributes()[PROPERTY_VARIABLE_METHOD];
    QString sAutoScript = xParameter.attributes()[PROPERTY_AUTO];
    QString sEnabledCondition = xParameter.attributes()[PROPERTY_ENABLED];
    if (lColumnLabels.size() == lColumnVariables.size())
    {
        QString sTargetRow = xParameter.attributes()[PROPERTY_TARGET_ROW].simplified();
        int nRows = xParameter.attributes()[PROPERTY_NROWS].toInt();
        QString sTargetVariable = xParameter.attributes()[PROPERTY_TARGET_VARIABLE];

        for (int iRow=0; iRow<nRows; iRow++)
        {
            QString sRowNumber = QString::number(iRow+1);
            if (sRowNumber.length() < 2)
                sRowNumber = "0"+sRowNumber;
            for (int iColumn=0; iColumn<lColumnVariables.size(); iColumn++)
            {
                QString sFormattedVariable("");

                // Compute variable name using method1
                if (sVariableMethod == PROPERTY_VARIABLE_METHOD1)
                {
                    sFormattedVariable = identifyTargetVariable_method1(sTargetVariable, lColumnVariables, sTargetRow, iColumn, iRow);
                    if (!m_hParameters.contains(sFormattedVariable))
                    {
                        m_hParameters[sFormattedVariable] = new Parameter(sFormattedVariable.toUpper(), sParameterType, sFormattedVariable, PROPERTY_DEFAULT_VALUE, sAutoScript, sEnabledCondition);
                    }
                }
                else
                // Compute variable name using method2
                if (sVariableMethod == PROPERTY_VARIABLE_METHOD2)
                {
                    sFormattedVariable = identifyTargetVariable_method2(sTargetVariable, iRow);
                    if (!m_hParameters.contains(sFormattedVariable))
                    {
                        m_hParameters[sFormattedVariable] = new Parameter(sFormattedVariable.toUpper(), sParameterType, sFormattedVariable, PROPERTY_DEFAULT_VALUE, sAutoScript, sEnabledCondition);
                    }
                }
            }
        }
    }
    else qDebug() << "ERROR: CAN'T PARSE PARAMETER TABLE";
}

//-------------------------------------------------------------------------------------------------

QString ParameterMgr::identifyTargetVariable_method1(const QString &sTargetVariable, const QStringList &lColumnVariables, const QString &sTargetRow, int iColumn, int iRow)
{
    QString sRowNumber = QString::number(iRow+1);
    if (sRowNumber.length() < 2)
        sRowNumber = "0"+sRowNumber;
    QString sFormattedVariable = QString(sTargetVariable).arg(sTargetRow).arg(sRowNumber).arg(lColumnVariables[iColumn]);
    return sFormattedVariable;
}

//-------------------------------------------------------------------------------------------------

QString ParameterMgr::identifyTargetVariable_method2(const QString &sTargetVariable, int iRow)
{
    QString sRowNumber = QString::number(iRow+1);
    if (sRowNumber.length() < 2)
        sRowNumber = "0"+sRowNumber;
    QString sFormattedVariable = QString(sTargetVariable).arg(sRowNumber);
    return sFormattedVariable;
}

//-------------------------------------------------------------------------------------------------

QVector<QString> ParameterMgr::extractVariableNames(const QString &sInputString)
{
    QVector<QString> vVariableNames;
    QRegularExpression regExp("qt_(.*?)_qt");
    QRegularExpressionMatchIterator i = regExp.globalMatch(sInputString);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString sCaptured = match.captured(0);
        if (match.hasMatch() && !vVariableNames.contains(sCaptured))
            vVariableNames << sCaptured;
    }
    return vVariableNames;
}

//-------------------------------------------------------------------------------------------------

QString ParameterMgr::evaluateAutoScript(const QString &sAutoScript, bool &bSuccess)
{
    QVector<QString> vVariableNames = extractVariableNames(sAutoScript);
    QString sMatchedScript = sAutoScript;
    bSuccess = true;
    foreach (QString sParameterVariableName, vVariableNames)
    {
        Parameter *pParameter = getParameterByVariableName(sParameterVariableName);
        if (pParameter == nullptr)
        {
            qDebug() << "ERROR: CAN'T EVALUATE " << sAutoScript << " SINCE VARIABLE " << sParameterVariableName << " DOES NOT EXIST";
            bSuccess = false;
            break;
        }
        sMatchedScript.replace(sParameterVariableName, pParameter->value());
    }
    if (bSuccess)
    {
        bSuccess = false;
        QScriptEngine expression;
        QScriptValue xResult = expression.evaluate(sMatchedScript);
        if (xResult.isNumber())
        {
            bSuccess = true;
            return xResult.toString();
        }
    }
    return QString();
}

//-------------------------------------------------------------------------------------------------

bool ParameterMgr::evaluateEnabledCondition(const QString &sEnabledCondition, bool &bSuccess)
{
    QVector<QString> vVariableNames = extractVariableNames(sEnabledCondition);
    QString sMatchedScript = sEnabledCondition;
    bSuccess = true;
    foreach (QString sParameterVariableName, vVariableNames)
    {
        Parameter *pParameter = getParameterByVariableName(sParameterVariableName);
        if (pParameter == nullptr)
        {
            qDebug() << "ERROR: CAN'T EVALUATE " << sEnabledCondition << " SINCE VARIABLE " << sParameterVariableName << " DOES NOT EXIST";
            bSuccess = false;
            break;
        }
        if (pParameter->type() == PROPERTY_STRING)
        {
            sMatchedScript.replace("&quot;", "\"");
            QString sQuotedString = QString("\"%1\"").arg(pParameter->value());
            sMatchedScript.replace(sParameterVariableName, sQuotedString);
        }
        else
            sMatchedScript.replace(sParameterVariableName, pParameter->value());
    }
    if (bSuccess)
    {
        bSuccess = false;
        QScriptEngine expression;
        QScriptValue xResult = expression.evaluate(sMatchedScript);
        if (xResult.isBoolean())
        {
            bSuccess = true;
            return xResult.toBool();
        }
    }
    return false;
}

//-------------------------------------------------------------------------------------------------

bool ParameterMgr::loadMenu1Parameters()
{
    // Retrieve root node
    m_xMenu1Node = CXMLNode::loadXMLFromFile(":/data/menu1.xml");
    if (m_xMenu1Node.nodes().isEmpty())
        return false;

    // Parse
    parseSingleBlock(m_xMenu1Node);

    return true;
}

//-------------------------------------------------------------------------------------------------

bool ParameterMgr::loadMenu2Parameters()
{
    // Retrieve root node
    m_xMenu2Node = CXMLNode::loadXMLFromFile(":/data/menu2.xml");
    if (m_xMenu2Node.nodes().isEmpty())
        return false;

    // Parse
    parseSingleBlock(m_xMenu2Node);

    return true;
}

//-------------------------------------------------------------------------------------------------

bool ParameterMgr::loadMenu3Parameters()
{
    // Retrieve root node
    m_xMenu3Node = CXMLNode::loadXMLFromFile(":/data/menu3.xml");
    if (m_xMenu3Node.nodes().isEmpty())
        return false;

    // Parse
    parseSingleBlock(m_xMenu3Node);

    return true;
}
//-------------------------------------------------------------------------------------------------

bool ParameterMgr::loadSettingsParameters()
{
    // Retrieve root node
    m_xSettingsNode = CXMLNode::loadXMLFromFile(":/data/settings.xml");
    if (m_xSettingsNode.nodes().isEmpty())
        return false;

    // Parse
    parseSingleBlock(m_xSettingsNode);
    qDebug() << "INFORMATION: IDENTIFIED " << m_hParameters.size();

    return true;
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::setParameterValue(const QString &sParameterName, const QString &sParameterValue)
{
    Parameter *pParameter = m_hParameters[sParameterName];
    if (pParameter != nullptr)
        pParameter->setValue(sParameterValue);
    else qDebug() << "ERROR: CAN'T FIND PARAMETER " << sParameterName;
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::generateScript()
{
    QString sInFile = ":/data/script_in.scad";
    QString sOutFile = "script_out.scad";
    ScriptMgr::generateScript(sInFile, sOutFile, m_hParameters.values());

    QFile outputScriptFile(sOutFile);
    if (outputScriptFile.open(QIODevice::ReadOnly))
    {
        QTextStream outStream(&outputScriptFile);
        QString sOutputString = outStream.readAll();
        outputScriptFile.close();
        QVector<QString> vUnReplacedVariables = extractVariableNames(sOutputString);
        if (!vUnReplacedVariables.isEmpty())
        {
            qDebug() << "COULD NOT REPLACE THE FOLLOWING VARIABLES: ";
            foreach (QString sUnReplacedVariable, vUnReplacedVariables)
                qDebug() << sUnReplacedVariable << "\n";
        }
        else qDebug() << "ALL VARIABLES SUCCESSFULLY REPLACED";
    }
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &ParameterMgr::menu1Node() const
{
    return m_xMenu1Node;
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &ParameterMgr::menu2Node() const
{
    return m_xMenu2Node;
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &ParameterMgr::menu3Node() const
{
    return m_xMenu3Node;
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &ParameterMgr::settingsNode() const
{
    return m_xSettingsNode;
}

//-------------------------------------------------------------------------------------------------

Parameter *ParameterMgr::getParameterByVariableName(const QString &sParameterVariableName) const
{
    return m_hParameters[sParameterVariableName];
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::setController(Controller *pController)
{
    m_pController = pController;
}
