#ifndef DXFORSTLFILEPICKER_H
#define DXFORSTLFILEPICKER_H

// Qt
#include <QWidget>
class QLineEdit;

// Application
#include "basewidget.h"
namespace Ui {
class DXForSTLFilePicker;
}

class DXForSTLFilePicker : public BaseWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit DXForSTLFilePicker(Controller *pController, const QString &sDefaultValue, const QString &sSTLVariable, const QString &sDXFVariable, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent=nullptr);

    //! Destructor
    ~DXForSTLFilePicker();

    //! Set value
    void setValue(const QString &sVariableName, const QString &sVariableValue);

    //! Apply value
    virtual void applyValue(const QString &sValue);

    //! Apply default value
    virtual void applyDefaultValue();

    //! Return STL variable
    const QString &stlVariable() const;

    //! Return DXF variable
    const QString &dxfVariable() const;

private:
    //! UI
    Ui::DXForSTLFilePicker *ui;

    //! STL variable
    QString m_sSTLVariable;

    //! DXF variable
    QString m_sDXFVariable;

    //! Widget hash
    QHash<QString, QLineEdit *> m_hWidgetHash;

public slots:
    //! Select DXF
    void onSelectDXF();

    //! Select STL
    void onSelectSTL();
};

#endif // DXFORSTLFILEPICKER_H
