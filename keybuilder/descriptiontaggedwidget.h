#ifndef DESCRIPTIONTAGGEDWIDGET_H
#define DESCRIPTIONTAGGEDWIDGET_H

// Qt
#include <QWidget>

// Application
#include "cxmlnode.h"

// Application
class QPaintEvent;

class DescriptionTaggedWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit DescriptionTaggedWidget(QWidget *parent=nullptr);

    //! Destructor
    virtual ~DescriptionTaggedWidget();

protected:
    //! Paint event
    virtual void paintEvent(QPaintEvent *event);

private:
    //! Load description
    void loadDescription();

private:
    //! Items
    QVector<CXMLNode> m_vItems;

    //! Max text width
    int m_iMaxTextWidth;

    //! Text height
    int m_iTextHeight;

    //! Font
    QFont m_font;
};

#endif // DESCRIPTIONTAGGEDWIDGET_H
