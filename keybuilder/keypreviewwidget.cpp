// Qt
#include <QPainter>

// Application
#include "keypreviewwidget.h"
#include "controller.h"

//-------------------------------------------------------------------------------------------------

KeyPreviewWidget::KeyPreviewWidget(QWidget *parent) : QWidget(parent),
    m_pKeyImage(nullptr), m_pController(nullptr)
{

}

//-------------------------------------------------------------------------------------------------

KeyPreviewWidget::~KeyPreviewWidget()
{

}

//-------------------------------------------------------------------------------------------------

void KeyPreviewWidget::setController(Controller *pController)
{
    m_pController = pController;
    m_pKeyImage = m_pController->keyPreviewImage();
}

//-------------------------------------------------------------------------------------------------

void KeyPreviewWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::red);
    QFont font;
    font.setFamily("Segoe UI");
    font.setPixelSize(24);
    QFontMetrics fm(font);
    int iTextHeight = fm.height();
    QString sLabel = tr("NO KEY SELECTED");
    int iTextWidth = fm.width(sLabel);
    int x = (rect().width()-iTextWidth)/2;
    int y = (rect().height()-iTextHeight)/2;
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(QPoint(x, y), sLabel);

    if (m_pKeyImage != nullptr)
    {
        QRect srcRect = m_pKeyImage->rect();
        QRect dstRect = rect();
        double dScaleFactor = computeScaleFactor(srcRect, dstRect);
        if (dScaleFactor > 0)
        {
            int iTargetHeight = qRound(srcRect.height()*dScaleFactor);
            int iTargetWidth = qRound(srcRect.width()*dScaleFactor);
            double x = (dstRect.width()-iTargetWidth)/2;
            double y = (dstRect.height()-iTargetHeight)/2;
            QRect targetRect(QPoint(x, y), QSize(iTargetWidth, iTargetHeight));
            painter.drawImage(targetRect, *m_pKeyImage, srcRect);
        }
    }
}

//-------------------------------------------------------------------------------------------------

double KeyPreviewWidget::computeScaleFactor(const QRect &srcRect, const QRect &dstRect)
{
    double dScale = -1;
    if ((srcRect.width() > 0) && (srcRect.height() > 0))
        dScale = qMin((double)dstRect.width()/(double)srcRect.width(), (double)dstRect.height()/(double)srcRect.height());
    return dScale;
}

//-------------------------------------------------------------------------------------------------

void KeyPreviewWidget::onUpdateKeyImage()
{
    update();
}
