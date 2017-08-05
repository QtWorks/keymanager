// Qt
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QScriptEngine>
#include <QDebug>

// Application
#include "parametermgr.h"
#include "parameter.h"
#include "constants.h"
#include "scriptmgr.h"

//-------------------------------------------------------------------------------------------------

ParameterMgr::ParameterMgr(QObject *parent) : QObject(parent)
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
        QString sParameterType = xParameterNode.attributes()[PROPERTY_TYPE];
        if (sParameterType.simplified().isEmpty())
        {
            qDebug() << "WARNING: FOUND A PARAMETER WITH AN UNDEFINED TYPE, DEFAULTING TO STRING";
            sParameterType = PROPERTY_STRING;
        }

        // Special case for table
        if (sParameterType == PROPERTY_TABLE)
        {
            parseTableParameters(xParameterNode);
        }
        else
        {
            QString sParameterName = xParameterNode.attributes()[PROPERTY_NAME];
            if (sParameterName.simplified().isEmpty())
            {
                qDebug() << "WARNING: FOUND A PARAMETER WITH AN EMPTY NAME";
            }
            QString sParameterVariable = xParameterNode.attributes()[PROPERTY_VARIABLE];
            if (sParameterVariable.simplified().isEmpty())
            {
                qDebug() << "ERROR: FOUND A PARAMETER WITH AN UNDEFINED VARIABLE";
                continue;
            }
            if (!m_hParameters.contains(sParameterVariable))
                m_hParameters[sParameterVariable] = new Parameter(sParameterName, sParameterType, sParameterVariable);
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
                        m_hParameters[sFormattedVariable] = new Parameter(sFormattedVariable.toUpper(), sParameterType, sFormattedVariable);
                    }
                }
                else
                    // Compute variable name using method2
                    if (sVariableMethod == PROPERTY_VARIABLE_METHOD2)
                    {
                        sFormattedVariable = identifyTargetVariable_method2(sTargetVariable, iRow);
                        if (!m_hParameters.contains(sFormattedVariable))
                        {
                            m_hParameters[sFormattedVariable] = new Parameter(sFormattedVariable.toUpper(), sParameterType, sFormattedVariable);
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

double ParameterMgr::evaluateAutoScript(const QString &sAutoScript, bool &bSuccess)
{
    QVector<QString> vVariableNames = extractVariableNames(sAutoScript);
    QString sMatchedScript = sAutoScript;
    bSuccess = true;
    foreach (QString sVariableName, vVariableNames)
    {
        Parameter *pParameter = getParameterByVariableName(sVariableName);
        if (pParameter == nullptr)
        {
            qDebug() << "ERROR: CAN'T EVALUATE " << sAutoScript << " SINCE VARIABLE " << sVariableName << " DOES NOT EXIST";
            bSuccess = false;
            break;
        }
        sMatchedScript = sMatchedScript.replace(sVariableName, pParameter->value());
    }
    if (bSuccess)
    {
        bSuccess = false;
        QScriptEngine expression;
        QScriptValue xResult = expression.evaluate(sMatchedScript);
        if (xResult.isNumber())
        {
            bSuccess = true;
            return xResult.toNumber();
        }
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------

bool ParameterMgr::evaluateEnabledCondition(const QString &sEnabledCondition, bool &bSuccess)
{
    QVector<QString> vVariableNames = extractVariableNames(sEnabledCondition);
    QString sMatchedScript = sEnabledCondition;
    bSuccess = true;
    foreach (QString sVariableName, vVariableNames)
    {
        Parameter *pParameter = getParameterByVariableName(sVariableName);
        if (pParameter == nullptr)
        {
            qDebug() << "ERROR: CAN'T EVALUATE " << sEnabledCondition << " SINCE VARIABLE " << sVariableName << " DOES NOT EXIST";
            bSuccess = false;
            break;
        }
        if (pParameter->type() == PROPERTY_STRING)
        {
            sMatchedScript = sMatchedScript.replace("&quot;", "\"");
            QString sQuotedString = QString("\"%1\"").arg(pParameter->value());
            sMatchedScript = sMatchedScript.replace(sVariableName, sQuotedString);
        }
        else
            sMatchedScript = sMatchedScript.replace(sVariableName, pParameter->value());
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
    qDebug() << "INFORMATION: IDENTIFIED " << m_hParameters.size() << " PARAMETERS IN MENU 1 ***";

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
    qDebug() << "INFORMATION: IDENTIFIED " << m_hParameters.size() << " PARAMETERS IN MENU 2";

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
    QString sInFile = "D:/projects/keymanager/keybuilder/data/testdata_in.txt";
    QString sOutFile = "D:/projects/keymanager/keybuilder/data/testdata_out.txt";
    ScriptMgr::generateScript(sInFile, sOutFile, m_hParameters.values());
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

Parameter *ParameterMgr::getParameterByVariableName(const QString &sVariableName) const
{
    return m_hParameters[sVariableName];
}
