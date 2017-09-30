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

HomeTab::HomeTab(QWidget *parent) : DescriptionTaggedWidget(parent),
    ui(new Ui::HomeTab)
{
    ui->setupUi(this);
}

//-------------------------------------------------------------------------------------------------

HomeTab::~HomeTab()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void HomeTab::paintEvent(QPaintEvent *event)
{
    DescriptionTaggedWidget::paintEvent(event);
    QPainter painter(this);
    QPixmap pixmap(":/images/marque-3000-px.jpg");
    int iSrcWidth = pixmap.width();
    int iSrcHeight = pixmap.height();
    int iTargetWidth = 200;
    int iTargetHeight = 200;
    int iMargin = 8;
    painter.drawPixmap(width()-(iTargetWidth+iMargin), height()-(iTargetHeight+iMargin), iTargetWidth, iTargetHeight, pixmap, 0, 0, iSrcWidth, iSrcHeight);

}
