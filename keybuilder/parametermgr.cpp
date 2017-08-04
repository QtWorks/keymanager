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
            qDebug() << "*** FIND A PARAMETER WITH AN UNDEFINED TYPE ***";
            continue;
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
                qDebug() << "*** FIND A PARAMETER WITH AN EMPTY NAME ***";
                continue;
            }
            QString sParameterVariable = xParameterNode.attributes()[PROPERTY_VARIABLE];
            if (sParameterVariable.simplified().isEmpty())
            {
                qDebug() << "*** FIND A PARAMETER WITH AN UNDEFINED VARIABLE ***";
                continue;
            }

            qDebug() << "*** IDENTIFIED ***" << sParameterVariable;
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
                        qDebug() << "*** IDENTIFIED TABLE PARAMETER ***" << sFormattedVariable;
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
                            qDebug() << "*** IDENTIFIED TABLE PARAMETER ***" << sFormattedVariable;
                        }
                    }
            }
        }
    }
    else qDebug() << "*** CAN'T PARSE PARAMETER TABLE ***";
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
        if (match.hasMatch())
            vVariableNames << match.captured(0);
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
            qDebug() << "*** CAN'T EVALUATE " << sAutoScript << " SINCE VARIABLE " << sVariableName << " DOES NOT EXIST";
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
            qDebug() << "*** CAN'T EVALUATE " << sEnabledCondition << " SINCE VARIABLE " << sVariableName << " DOES NOT EXIST";
            bSuccess = false;
            break;
        }
        if (pParameter->type() == PROPERTY_STRING)
        {
            QString sReplace
            sMatchedScript = sMatchedScript.replace(sVariableName, pParameter->value());
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

    // This describes the type of key to be created
    m_hParameters[PARAMETER_TYPE_OF_KEY] = new Parameter(PROPERTY_TYPE_OF_KEY, PROPERTY_STRING, PARAMETER_TYPE_OF_KEY);

    // Retrieve Key nodes
    QVector<CXMLNode> vBlocks = m_xMenu1Node.getNodesByTagName(TAG_BLOCK);
    foreach (CXMLNode xBlock, vBlocks)
        parseSingleBlock(xBlock);

    qDebug() << "*** IDENTIFIED " << m_hParameters.size() << " PARAMETERS IN MENU 1 ***";

    return true;
}

//-------------------------------------------------------------------------------------------------

bool ParameterMgr::loadMenu2Parameters()
{
    // Retrieve root node
    m_xMenu2Node = CXMLNode::loadXMLFromFile(":/data/menu2.xml");
    if (m_xMenu2Node.nodes().isEmpty())
        return false;

    // Retrieve Key nodes
    QVector<CXMLNode> vBlocks = m_xMenu2Node.getNodesByTagName(TAG_BLOCK);
    foreach (CXMLNode xBlock, vBlocks)
        parseSingleBlock(xBlock);

    qDebug() << "*** IDENTIFIED " << m_hParameters.size() << " PARAMETERS IN MENU 2 ***";

    return true;
}

//-------------------------------------------------------------------------------------------------

void ParameterMgr::setParameterValue(const QString &sParameterName, const QString &sParameterValue)
{
    Parameter *pParameter = m_hParameters[sParameterName];
    if (pParameter != nullptr)
        pParameter->setValue(sParameterValue);
    else qDebug() << "*** CAN'T FIND PARAMETER " << sParameterName << " ***";
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
