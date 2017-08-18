#ifndef HOMETAB_H
#define HOMETAB_H

// Qt
#include <QWidget>
#include <QFont>

// Application
#include "cxmlnode.h"

namespace Ui {
class HomeTab;
}

class HomeTab : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit HomeTab(QWidget *parent = 0);

    //! Destructor
    ~HomeTab();

protected:
    //! Paint event
    virtual void paintEvent(QPaintEvent *event);

private:
    //! Load description
    void loadDescription();

private:
    //! UI
    Ui::HomeTab *ui;

    //! Items
    QVector<CXMLNode> m_vItems;

    //! Max text width
    int m_iMaxTextWidth;

    //! Text height
    int m_iTextHeight;

    //! Font
    QFont m_font;
};

#endif // HOMETAB_H
