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

bool ParameterMgr::loadMenu1Parameters()
{
    // Retrieve root node
    m_xMenu1Node = CXMLNode::loadXMLFromFile(":/data/menu1.xml");
    if (m_xMenu1Node.nodes().isEmpty())
        return false;

    // This describes the type of key to be created
    m_hParameters[PARAMETER_TYPE_OF_KEY] = new Parameter();

    // Retrieve Key nodes
    QVector<CXMLNode> vKeyNodes = m_xMenu1Node.getNodesByTagName(TAG_KEY);
    foreach (CXMLNode xKeyNode, vKeyNodes)
    {
        QVector<CXMLNode> vParameterNodes = xKeyNode.getNodesByTagName(TAG_PARAMETER);
        foreach (CXMLNode xParameterNode, vParameterNodes)
        {
            QString sParameterName = xParameterNode.attributes()[PROPERTY_NAME];
            if (sParameterName.simplified().isEmpty())
            {
                qDebug() << "--- FIND A PARAMETER WITH AN EMPTY NAME!";
                continue;
            }
            QString sParameterType = xParameterNode.attributes()[PROPERTY_TYPE];
            if (sParameterType.simplified().isEmpty())
            {
                qDebug() << "--- FIND A PARAMETER WITH AN UNDEFINED TYPE!";
                continue;
            }
            QString sParameterVariable = xParameterNode.attributes()[PROPERTY_VARIABLE];
            if (sParameterVariable.simplified().isEmpty())
            {
                qDebug() << "--- FIND A PARAMETER WITH AN UNDEFINED VARIABLE!";
                continue;
            }

            if (!m_hParameters.contains(sParameterVariable))
                m_hParameters[sParameterVariable] = new Parameter(sParameterName, sParameterType, sParameterVariable);
        }
    }

    qDebug() << "IDENTIFIED " << m_hParameters.size() << " PARAMETERS";

    return true;
}

//-------------------------------------------------------------------------------------------------

bool ParameterMgr::loadMenu2Parameters()
{
    // TO DO
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
