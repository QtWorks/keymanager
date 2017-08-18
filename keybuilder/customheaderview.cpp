// Qt
#include <QHeaderView>

// Application
#include "customheaderview.h"
#include "headerobject.h"

//-------------------------------------------------------------------------------------------------

CustomHeaderView::CustomHeaderView(const QVector<QString> &vColumnLabels, QWidget *parent) : QHeaderView(Qt::Horizontal, parent)
{
    int iColumnCount = vColumnLabels.size();
    for (int i=0; i<iColumnCount; i++)
    {
        HeaderObject *pHeaderObject = new HeaderObject(i, vColumnLabels[i], this);
        connect(pHeaderObject, &HeaderObject::clearClicked, this, &CustomHeaderView::clearClicked, Qt::UniqueConnection);
        m_vHeaderSections.insert(i, pHeaderObject);
        m_vHeaderSections[i]->hide();
    }

    setMinimumSectionSize(m_vHeaderSections[0]->minimumSizeHint().width());
    setDefaultSectionSize(m_vHeaderSections[0]->minimumSizeHint().width());
}

//-------------------------------------------------------------------------------------------------

CustomHeaderView::~CustomHeaderView()
{

}

//-------------------------------------------------------------------------------------------------

void CustomHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    Q_UNUSED(painter);
    if (rect.isValid())
    {
        m_vHeaderSections[logicalIndex]->setGeometry(rect);
        m_vHeaderSections[logicalIndex]->show();
    }
}

//-------------------------------------------------------------------------------------------------

QSize CustomHeaderView::sizeHint() const
{
    QSize s = size();
    s.setHeight(m_vHeaderSections[0]->minimumSizeHint().height());
    return s;
}
