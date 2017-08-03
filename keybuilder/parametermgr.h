#ifndef PARAMETERMGR_H
#define PARAMETERMGR_H

// Qt
#include <QObject>
#include <QHash>

// Application
#include "cxmlnode.h"
class Parameter;

class ParameterMgr : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit ParameterMgr(QObject *parent=nullptr);

    //! Destructor
    ~ParameterMgr();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Load menu 1 parameters
    bool loadMenu1Parameters();

    //! Load menu 2 parameters
    bool loadMenu2Parameters();

    //! Set parameter value
    void setParameterValue(const QString &sParameterName, const QString &sParameterValue);

    //! Generate script
    void generateScript();

    //! Identify target variable (method1)
    static QString identifyTargetVariable_method1(const QString &sTargetVariable, const QStringList &lColumnVariables, const QString &sTargetRow, int iColumn, int iRow);

    //! Identify target variable (method2)
    static QString identifyTargetVariable_method2(const QString &sTargetVariable, int iRow);

    //! Extract variable names
    static QVector<QString> extractVariableNames(const QString &sInputString);

    //! Evaluate auto script
    double evaluateAutoScript(const QString &sAutoScript, bool &bSuccess);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return menu1 node
    const CXMLNode &menu1Node() const;

    //! Return menu2 node
    const CXMLNode &menu2Node() const;

    //! Return parameter by variable name
    Parameter *getParameterByVariableName(const QString &sVariableName) const;

private:
    //! Parse single block
    void parseSingleBlock(const CXMLNode &xBlock);

    //! Parse table parameters
    void parseTableParameters(const CXMLNode &xParameter);

private:
    //! Menu 1 node
    CXMLNode m_xMenu1Node;

    //! Menu 2 node
    CXMLNode m_xMenu2Node;

    //! Parameters
    QHash<QString, Parameter *> m_hParameters;
};

#endif // PARAMETERMGR_H
