// Qt
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
        QString sParameterName = xParameterNode.attributes()[PROPERTY_NAME];
        if (sParameterName.simplified().isEmpty())
        {
            qDebug() << "*** FIND A PARAMETER WITH AN EMPTY NAME ***";
            continue;
        }
        QString sParameterType = xParameterNode.attributes()[PROPERTY_TYPE];
        if (sParameterType.simplified().isEmpty())
        {
            qDebug() << "*** FIND A PARAMETER WITH AN UNDEFINED TYPE ***";
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

    // Parse table type1
    QVector<CXMLNode> vParameterTableType1Nodes = xBlock.getNodesByTagName(TAG_PARAMETER_TABLE1);
    foreach (CXMLNode xParameterTableNode, vParameterTableType1Nodes)
    {
        QStringList lColumnVariable = xParameterTableNode.attributes()[PROPERTY_COLUMN_VARIABLES].split(",");
        QString sTargetRow = xParameterTableNode.attributes()[PROPERTY_TARGET_ROW];
        int nRows = xParameterTableNode.attributes()[PROPERTY_NROWS].toInt();
        QString sTargetVariable = xParameterTableNode.attributes()[PROPERTY_TARGET_VARIABLE];

        qDebug() << lColumnVariable << sTargetRow << nRows << sTargetVariable;

        for (int i=0; i<nRows; i++)
        {
            for (int j=0; j<lColumnVariable.size(); j++)
            {
                QString sRowLabel = QString::number(i+1);
                if (sRowLabel.length() < 2)
                    sRowLabel = "0"+sRowLabel;
                QString sColumnLabel = lColumnVariable[j].simplified();
                QString sVariable = QString(sTargetVariable).arg(sTargetRow).arg(sRowLabel).arg(sColumnLabel);
                qDebug() << "*** IDENTIFIED TABLE VARIABLE: " << sVariable;
            }
        }
    }

    // Parse child blocks
    QVector<CXMLNode> vChildBlocks = xBlock.getNodesByTagName(TAG_BLOCK);
    foreach (CXMLNode xChildBlock, vChildBlocks)
        parseSingleBlock(xChildBlock);
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
