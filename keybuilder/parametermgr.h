#ifndef PARAMETERMGR_H
#define PARAMETERMGR_H

// Qt
#include <QObject>
#include <QHash>

// Application
#include "cxmlnode.h"
class Parameter;
class Controller;

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

    //! Load menu 3 parameters
    bool loadMenu3Parameters();

    //! Load settings parameters
    bool loadSettingsParameters();

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
    QString evaluateAutoScript(const QString &sAutoScript, bool &bSuccess);

    //! Evaluate enabled condtion
    bool evaluateEnabledCondition(const QString &sEnabledConsition, bool &bSuccess);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return menu1 node
    const CXMLNode &menu1Node() const;

    //! Return menu2 node
    const CXMLNode &menu2Node() const;

    //! Return menu3 node
    const CXMLNode &menu3Node() const;

    //! Return settings node
    const CXMLNode &settingsNode() const;

    //! Return parameter by variable name
    Parameter *getParameterByVariableName(const QString &sVariableName) const;

    //! Set controller
    void setController(Controller *pController);

private:
    //! Parse single block
    void parseSingleBlock(const CXMLNode &xBlock);

    //! Parse table parameters
    void parseTableParameters(const CXMLNode &xParameter);

private:
    //! Controller
    Controller *m_pController;

    //! Menu 1 node
    CXMLNode m_xMenu1Node;

    //! Menu 2 node
    CXMLNode m_xMenu2Node;

    //! Menu 3 node
    CXMLNode m_xMenu3Node;

    //! Settings node
    CXMLNode m_xSettingsNode;

    //! Parameters
    QHash<QString, Parameter *> m_hParameters;
};

#endif // PARAMETERMGR_H
