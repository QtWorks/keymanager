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

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return menu1 node
    const CXMLNode &menu1Node() const;

    //! Return menu2 node
    const CXMLNode &menu2Node() const;

private:
    //! Menu 1 node
    CXMLNode m_xMenu1Node;

    //! Menu 2 node
    CXMLNode m_xMenu2Node;

    //! Parameters
    QHash<QString, Parameter *> m_hParameters;
};

#endif // PARAMETERMGR_H
