// Qt
#include <QPainter>
#include <QPen>
#include <QDebug>

// Application
#include "hometab.h"
#include "ui_hometab.h"
#include "cxmlnode.h"
#define APP_TITLE "title"
#define APP_DEVELOPER "developer"
#define DEVELOPER_EMAIL "email"
#define DEVELOPER_PHONE "phone"
#define NODE_VALUE "value"

//-------------------------------------------------------------------------------------------------

HomeTab::HomeTab(QWidget *parent) : QWidget(parent),
    ui(new Ui::HomeTab)
{
    ui->setupUi(this);
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
    QWidget::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(QRectF(10, 10, 216, 80), 10, 10);
    QPen pen(Qt::gray, 3);
    p.setPen(pen);
    p.fillPath(path, QColor("#444444"));
    p.drawPath(path);
    p.drawText(16, 28, m_sAppTitle);
    p.drawText(16, 44, m_sAppDeveloper);
    p.drawText(16, 60, m_sDeveloperEmail);
    p.drawText(16, 76, m_sDeveloperPhone);
}

//-------------------------------------------------------------------------------------------------

void HomeTab::loadDescription()
{
    CXMLNode xRootNode = CXMLNode::loadXMLFromFile(":/data/about.xml");
    CXMLNode xAppTitleNode = xRootNode.getNodeByTagName(APP_TITLE);
    m_sAppTitle = xAppTitleNode.attributes()[NODE_VALUE];
    CXMLNode xAppDeveloperNode = xRootNode.getNodeByTagName(APP_DEVELOPER);
    m_sAppDeveloper = xAppDeveloperNode.attributes()[NODE_VALUE];
    CXMLNode xDeveloperEmail = xRootNode.getNodeByTagName(DEVELOPER_EMAIL);
    m_sDeveloperEmail = xDeveloperEmail.attributes()[NODE_VALUE];
    CXMLNode xDeveloperPhone = xRootNode.getNodeByTagName(DEVELOPER_PHONE);
    m_sDeveloperPhone = xDeveloperPhone.attributes()[NODE_VALUE];
    update();
}
