#ifndef DXFORSTLFILEPICKER_H
#define DXFORSTLFILEPICKER_H

// Qt
#include <QWidget>

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
    explicit DXForSTLFilePicker(QWidget *parent=nullptr);

    //! Constructor
    explicit DXForSTLFilePicker(const QString &sDefaultValue, const QString &sSTLVariable, const QString &sDXFVariable, QWidget *parent=nullptr);

    //! Destructor
    ~DXForSTLFilePicker();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return DXF value
    QString dxfValue() const;

    //! Return STL value
    QString stlValue() const;

    //! Return DXF variable
    const QString &dxfVariable() const;

    //! Return STL variable
    const QString &stlVariable() const;

private:
    //! UI
    Ui::DXForSTLFilePicker *ui;

    //! STL variable
    QString m_sSTLVariable;

    //! DXF variable
    QString m_sDXFVariable;

public slots:
    //! Select DXF
    void onSelectDXF();

    //! Select STL
    void onSelectSTL();

signals:
    //! DXF selected
    void dxfSelected();

    //! STL selected
    void stlSelected();
};

#endif // DXFORSTLFILEPICKER_H
