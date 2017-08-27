// Qt
#include <QPainter>

// Application
#include "keypreviewwidget.h"
#include "controller.h"
#include "parametermgr.h"
#include "constants.h"
#include "collapsibleblock.h"

//-------------------------------------------------------------------------------------------------

KeyPreviewWidget::KeyPreviewWidget(QWidget *parent) : QWidget(parent),
    m_pKeyImage(nullptr), m_pController(nullptr), m_bNoCutSelected(true)
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
    connect(m_pController, &Controller::blockSelectionStatusChanged, this, &KeyPreviewWidget::onBlockSelectionStatusChanged);
}

//-------------------------------------------------------------------------------------------------

void KeyPreviewWidget::setNoCutSelected(bool bNoCutSelected)
{
    m_bNoCutSelected = bNoCutSelected;
    update();
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

    QString sLabel = tr("NO KEY SELECTED");
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, sLabel);

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
            if (m_bNoCutSelected)
            {
                pen.setColor("orange");
                painter.setPen(pen);
                painter.drawText(targetRect,  Qt::AlignCenter, QString("NO CUT SELECTED"));
            }
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

//-------------------------------------------------------------------------------------------------

void KeyPreviewWidget::onBlockSelectionStatusChanged(CollapsibleBlock *pBlock)
{
    Q_UNUSED(pBlock);
    QVector<QString> vWatchedParameters;
    vWatchedParameters << "qt_regular_cut_use_qt"
        << "qt_round_pump_cut_use_qt"
        << "qt_lever_lock_cut_use_qt"
        << "qt_abloy_classic_cut_use_qt"
        << "qt_abloy_disklock_cut_use_qt"
        << "qt_abloy_protec_cut_use_qt"
        << "qt_abus_plus_cut_qt"
        << "qt_axi_home_cut_use_qt"
        << "qt_fichet_787_cut_use_qt"
        << "qt_extra_cut_use_qt";
    bool bNoCutSelected = true;
    foreach (QString sParameterVariable, vWatchedParameters)
    {
        QString sValue = m_pController->parameterMgr()->getParameterValue(sParameterVariable);
        if (sValue == VALUE_YES)
        {
            bNoCutSelected = false;
            break;
        }
    }
    setNoCutSelected(bNoCutSelected);
}
