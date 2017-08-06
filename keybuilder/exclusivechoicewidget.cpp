// Qt
#include <QRadioButton>

// Application
#include "exclusivechoicewidget.h"
#include "ui_exclusivechoicewidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::ExclusiveChoiceWidget(QWidget *parent) : BaseWidget(parent),
    ui(new Ui::ExclusiveChoiceWidget)
{
    m_sDefaultValue = "";
}

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::ExclusiveChoiceWidget(const QStringList &lLabels, const QStringList &lValues, const QString &sLabel, const QString &sDefaultValue, QWidget *parent) :
    BaseWidget(parent), ui(new Ui::ExclusiveChoiceWidget)
{
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;
    setup(sLabel, lLabels, lValues);
}

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::~ExclusiveChoiceWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::setup(const QString &sLabel, const QStringList &lLabels, const QStringList &lValues)
{
    ui->labelArea->setVisible(!sLabel.simplified().isEmpty());
    ui->labelArea->setText(sLabel.simplified());

    int nItems = qMin(lLabels.size(), lValues.size());
    for (int i=0; i<nItems; i++)
    {
        QRadioButton *pRadioButton = new QRadioButton(lLabels[i], this);
        pRadioButton->setAutoExclusive(true);
        pRadioButton->setProperty(PROPERTY_USER_VALUE, lValues[i]);
        connect(pRadioButton, &QRadioButton::toggled, this, &ExclusiveChoiceWidget::onRadioButtonToggled);
        ui->radioButtonArea->addWidget(pRadioButton);
        m_vRadioButtons << pRadioButton;
    }
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::applyDefaultValue()
{
    if (!m_sDefaultValue.isEmpty())
    {
        bool bMatch = false;
        foreach (QRadioButton *pRadioButton, m_vRadioButtons)
        {
            QString sUserValue = pRadioButton->property(PROPERTY_USER_VALUE).toString();
            if (sUserValue == m_sDefaultValue)
            {
                bMatch = true;
                pRadioButton->setChecked(true);
                break;
            }
        }
        if (!bMatch && !m_vRadioButtons.isEmpty())
            m_vRadioButtons.first()->setChecked(true);
    }
    else
    if (!m_vRadioButtons.isEmpty())
        m_vRadioButtons.first()->setChecked(true);
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::onRadioButtonToggled(bool bClicked)
{
    QRadioButton *pSender = dynamic_cast<QRadioButton *>(sender());
    if (pSender && bClicked)
    {
        QString sUserValue = pSender->property(PROPERTY_USER_VALUE).toString();
        emit selectionChanged(sUserValue);
    }
}
