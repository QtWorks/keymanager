// Qt
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QScriptEngine>
#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QDebug>

// Application
#include "parametermgr.h"
#include "parameter.h"
#include "constants.h"
#include "scriptmgr.h"
#include "helper.h"
#include "utils.h"

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
        QString sParameterName = xParameterNode.attributes()[PROPERTY_NAME].simplified();
        if (sParameterName.isEmpty())
        {
            QString sMsg = QString("FOUND A PARAMETER WITH AN EMPTY NAME: %1").arg(xParameterNode.toString());
            logWarning(sMsg);
        }
        QString sParameterType = xParameterNode.attributes()[PROPERTY_TYPE].simplified();
        if (sParameterType.isEmpty())
        {
            QString sMsg = QString("PARAMETER: %1 HAS NO TYPE. DEFAULTING TO STRING").arg(sParameterName);
            logWarning(sMsg);
            sParameterType = PROPERTY_STRING;
        }
        QString sParameterVariable = xParameterNode.attributes()[PROPERTY_VARIABLE].simplified();
        if (sParameterVariable.isEmpty() && (sParameterType != PROPERTY_TABLE))
        {
            QString sMsg = QString("PARAMETER: %1 HAS NO ASSOCIATED VARIABLE").arg(sParameterName);
            logError(sMsg);
            continue;
        }
        QString sParameterUI = xParameterNode.attributes()[PROPERTY_UI].simplified();
        if (sParameterUI.isEmpty())
        {
            QString sMsg = QString("PARAMETER VARIABLE: %1 HAS NO DEFINED UI").arg(sParameterVariable);
            logWarning(sMsg);
        }
        QString sDefaultValue = xParameterNode.attributes()[PROPERTY_DEFAULT].simplified();
        if (sDefaultValue.isEmpty())
        {
            sDefaultValue = VALUE_DEFAULT_VALUE;
            if (sParameterUI == WIDGET_DOUBLE_TRIPLET)
                sDefaultValue = VALUE_DEFAULT_TRIPLET_VALUE;
            QString sMsg = QString("PARAMETER VARIABLE: %1 HAS NO DEFAULT VALUE. DEFAULTING TO 0").arg(sParameterVariable);
            logWarning(sMsg);
        }
        QString sAutoScript = xParameterNode.attributes()[PROPERTY_AUTO].simplified();
        if (sAutoScript.isEmpty())
        {
            QString sMsg = QString("PARAMETER VARIABLE: %1 HAS NO AUTOSCRIPT").arg(sParameterVariable);
            logInfo(sMsg);
        }
        QString sEnabledCondition = xParameterNode.attributes()[PROPERTY_ENABLED].simplified();
        if (sEnabledCondition.isEmpty())
        {
            QString sMsg = QString("PARAMETER VARIABLE: %1 HAS NO ENABLED CONDITION").arg(sParameterVariable);
            logInfo(sMsg);
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
    QString sVariableMethod = xParameter.attributes()[PROPERTY_VARIABLE_METHOD].simplified();
    QString sAutoScript = xParameter.attributes()[PROPERTY_AUTO].simplified();
    QString sEnabledCondition = xParameter.attributes()[PROPERTY_ENABLED].simplified();
    QString sDefaultValue = xParameter.attributes()[PROPERTY_DEFAULT].simplified();
    QString sUnsetValue = xParameter.attributes()[PROPERTY_UNSET].simplified();

    // Compute number of columns based on column labels and column variables
    int nColumns = qMin(lColumnLabels.size(), lColumnVariables.size());
    lColumnLabels = lColumnLabels.mid(0, nColumns);
    lColumnVariables = lColumnVariables.mid(0, nColumns);

    if (nColumns > 0)
    {
        // Check we have the right number of default values:
        QStringList lDefaultValues;
        if (sDefaultValue.isEmpty())
        {
            for (int i=0; i<nColumns; i++)
                lDefaultValues << VALUE_DEFAULT_VALUE;
        }
        else
        if (sDefaultValue.contains(","))
        {
            lDefaultValues = sDefaultValue.split(",");
            if (lDefaultValues.size() != nColumns)
            {
                lDefaultValues.clear();
                for (int i=0; i<nColumns; i++)
                    lDefaultValues << VALUE_DEFAULT_VALUE;
            }
        }
        else lDefaultValues << sDefaultValue;

        // Check we have the right number of unset values:
        QStringList lUnsetValues;
        if (sUnsetValue.isEmpty())
        {
            for (int i=0; i<nColumns; i++)
                lUnsetValues << VALUE_DEFAULT_VALUE;
        }
        else
        if (sUnsetValue.contains(","))
        {
            lUnsetValues = sUnsetValue.split(",");
            if (lUnsetValues.size() != nColumns)
            {
                lUnsetValues.clear();
                for (int i=0; i<nColumns; i++)
                    lUnsetValues << VALUE_DEFAULT_VALUE;
            }
        }
        else lUnsetValues << sUnsetValue;

        QString sTargetRow = xParameter.attributes()[PROPERTY_TARGET_ROW].simplified();
        int nRows = xParameter.attributes()[PROPERTY_NROWS].toInt();
        QString sTargetVariable = xParameter.attributes()[PROPERTY_TARGET_VARIABLE].simplified();

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
                        m_hParameters[sFormattedVariable] = new Parameter(sFormattedVariable.toUpper(), sParameterType, sFormattedVariable, lDefaultValues[iColumn], sAutoScript, sEnabledCondition, false);
                        m_hParameters[sFormattedVariable]->setUnsetValue(lUnsetValues[iColumn]);
                    }
                }
                else
                // Compute variable name using method2
                if (sVariableMethod == PROPERTY_VARIABLE_METHOD2)
                {
                    sFormattedVariable = identifyTargetVariable_method2(sTargetVariable, iRow);
                    if (!m_hParameters.contains(sFormattedVariable))
                    {
                        m_hParameters[sFormattedVariable] = new Parameter(sFormattedVariable.toUpper(), sParameterType, sFormattedVariable, lDefaultValues[iColumn], sAutoScript, sEnabledCondition, false);
                        m_hParameters[sFormattedVariable]->setUnsetValue(lUnsetValues[iColumn]);
                    }
                }
            }
        }
    }
    else logError("CANNOT CREATE A TABLE WITH 0 COLUMN");
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
    foreach (QString sParameterVariable, vVariableNames)
    {
        Parameter *pParameter = getParameterByVariableName(sParameterVariable);
        if (pParameter == nullptr)
        {
            QString sMsg = QString("CAN'T EVALUATE: %1 SINCE PARAMETER VARIABLE: %2 DOES NOT EXIST").arg(sAutoScript).arg(sParameterVariable);
            logError(sMsg);
            bSuccess = false;
            break;
        }
        sMatchedScript.replace(sParameterVariable, pParameter->value());
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
    if (!sEnabledCondition.isEmpty())
    {
        QVector<QString> vVariableNames = extractVariableNames(sEnabledCondition);
        QString sMatchedScript = sEnabledCondition;
        bSuccess = true;
        foreach (QString sParameterVariable, vVariableNames)
        {
            Parameter *pParameter = getParameterByVariableName(sParameterVariable);
            if (pParameter == nullptr)
            {
                QString sMsg = QString("CAN'T EVALUATE: %1 SINCE PARAMETER VARIABLE: %2 DOES NOT EXIST").arg(sEnabledCondition).arg(sParameterVariable);
                logError(sMsg);
                bSuccess = false;
                break;
            }
            if (pParameter->type() == PROPERTY_STRING)
            {
                sMatchedScript.replace("&quot;", "\"");
                QString sQuotedString = QString("\"%1\"").arg(pParameter->value());
                sMatchedScript.replace(sParameterVariable, sQuotedString);
            }
            else
                sMatchedScript.replace(sParameterVariable, pParameter->value());
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
    }
    return true;
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
    return true;
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::setParameterValue(const QString &sParameterVariable, const QString &sVariableValue)
{
    Parameter *pParameter = m_hParameters[sParameterVariable];
    if (pParameter != nullptr)
        pParameter->setValue(sVariableValue);
    else
    {
        QString sMsg = QString("CAN'T FIND PARAMETER VARIABLE: %1").arg(sParameterVariable);
        logError(sMsg);
    }
}

//-------------------------------------------------------------------------------------------------

QString ParameterMgr::getParameterValue(const QString &sParameterVariable) const
{
    Parameter *pParameter = getParameterByVariableName(sParameterVariable);
    if (pParameter != nullptr)
        return pParameter->value();
    return QString("");
}

//-------------------------------------------------------------------------------------------------

bool ParameterMgr::exportParametersToSCAD(const QString &sOuputFileName)
{
    QString sInFile = ":/data/script_in.scad";
    if (!ScriptMgr::generateScript(sInFile, sOuputFileName, m_hParameters.values()))
        return false;
    QVector<QString> vUnReplacedVariables;
    if (!checkIfAllVariablesReplaced(sOuputFileName, vUnReplacedVariables))
    {
        logWarning("COULD NOT REPLACE THE FOLLOWING VARIABLES:");
        foreach (QString sUnReplacedVariable, vUnReplacedVariables)
            logWarning(sUnReplacedVariable);
    }
    else logInfo("ALL VARIABLES SUCCESSFULLY REPLACED");
    return true;
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::exportParametersToTXT(const QString &sOuputFileName)
{
    QFile outputParametersFile(sOuputFileName);
    if (outputParametersFile.open(QIODevice::WriteOnly))
    {
        QTextStream outStream(&outputParametersFile);
        for (QHash<QString, Parameter *>::iterator it=m_hParameters.begin(); it!=m_hParameters.end(); ++it)
        {
            Parameter *pParameter = it.value();
            if (pParameter != nullptr)
            {
                QString sParameterValue = pParameter->value();
                QString sParameterLine = QString("%1 = %2").arg(pParameter->variable(), sParameterValue);
                outStream << sParameterLine << "\n";
            }
        }
        outputParametersFile.close();
        QString sMsg = QString("PARAMETERS EXPORTED TO TEXT FILE: %1").arg(sOuputFileName);
        logInfo(sMsg);
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::importParametersFromTXT(const QString &sInputFileName)
{
    QFile inputParametersFile(sInputFileName);
    if (inputParametersFile.open(QIODevice::ReadOnly))
    {
        QTextStream inStream(&inputParametersFile);
        while (!inStream.atEnd())
        {
            QString sLine = inStream.readLine();
            QStringList lSplitted = sLine.split("=");
            if (lSplitted.size() == 2)
            {
                QString sParameterVariable = lSplitted.first().simplified();
                QString sVariableValue = lSplitted[1].simplified();
                Parameter *pParameter = getParameterByVariableName(sParameterVariable);
                if (pParameter != nullptr)
                {
                    emit updateWidgetValue(sParameterVariable, sVariableValue);
                }
                else
                {
                    QString sMsg = QString("%1 PARAMETER VARIABLE IS UNKNOWN").arg(sParameterVariable);
                    logError(sMsg);
                }

            }
        }
        inputParametersFile.close();
        QString sMsg = QString("PARAMETERS SUCCESSFULLY IMPORTED FROM: %1").arg(sInputFileName);
        logInfo(sMsg);
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

Parameter *ParameterMgr::getParameterByVariableName(const QString &sParameterVariable) const
{
    return m_hParameters[sParameterVariable];
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::setController(Controller *pController)
{
    m_pController = pController;
}

//-------------------------------------------------------------------------------------------------

bool ParameterMgr::checkIfAllVariablesReplaced(const QString &sFileName, QVector<QString> &vUnReplacedVariables) const
{
    vUnReplacedVariables.clear();
    QString sFileContents = Utils::loadFile(sFileName);
    if (!sFileContents.isEmpty())
    {
        QVector<QString> vUnReplacedVariables = extractVariableNames(sFileContents);
        return vUnReplacedVariables.isEmpty();
    }
    return true;
}
