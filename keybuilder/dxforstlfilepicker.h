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
    explicit DXForSTLFilePicker(const QString &sDefaultValue, const QString &sSTLVariable, const QString &sDXFVariable, QWidget *parent=nullptr);

    //! Destructor
    ~DXForSTLFilePicker();

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
};

#endif // DXFORSTLFILEPICKER_H
