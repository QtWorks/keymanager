// Qt
#include <QPainter>
#include <QPen>
#include <QDebug>

// Application
#include "hometab.h"
#include "ui_hometab.h"
#include "cxmlnode.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

HomeTab::HomeTab(QWidget *parent) : QWidget(parent),
    ui(new Ui::HomeTab), m_iMaxTextWidth(0)
{
    ui->setupUi(this);
    m_font.setFamily("Segoe UI");
    m_font.setPixelSize(16);
    loadDescription();
}

//-------------------------------------------------------------------------------------------------

HomeTab::~HomeTab()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void HomeTab::paintEvent(QPaintEvent *event)
{
    // Default paint
    QWidget::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    int nItems = m_vItems.size();
    int iHeight = (nItems+.5)*m_iTextHeight;
    qreal xRadius = 10;
    qreal yRadius = 10;

    // Setup painter path
    QPainterPath path;
    QPoint topLeft(10, 10);
    QSize size(m_iMaxTextWidth+m_font.pixelSize(), iHeight);
    int iStartX = topLeft.x()+6;
    int iStartY = topLeft.y()+m_iTextHeight;
    path.addRoundedRect(QRectF(topLeft, size), xRadius, yRadius);
    QPen pen(QColor("orange"));
    p.setPen(pen);
    p.fillPath(path, QColor("#444444"));
    p.drawPath(path);
    p.setFont(m_font);
    pen.setColor(Qt::lightGray);
    p.setPen(pen);
    int c = 0;
    foreach (CXMLNode xNode, m_vItems)
    {
        QString sLabel = xNode.attributes()[PROPERTY_VALUE];
        p.drawText(iStartX, iStartY+(m_iTextHeight*c++), sLabel);
    }
}

//-------------------------------------------------------------------------------------------------

void HomeTab::loadDescription()
{
    CXMLNode xRootNode = CXMLNode::loadXMLFromFile(":/data/about.xml");
    m_vItems = xRootNode.getNodesByTagName(TAG_ITEM);
    QFontMetrics fm(m_font);

    // Find max text width and height
    m_iTextHeight = fm.height();
    m_iMaxTextWidth = 0;
    foreach (CXMLNode xNode, m_vItems)
    {
        QString sLabel = xNode.attributes()[PROPERTY_VALUE];
        int iWidth = fm.width(sLabel);
        if (iWidth > m_iMaxTextWidth)
            m_iMaxTextWidth = iWidth;
    }

    update();
}
