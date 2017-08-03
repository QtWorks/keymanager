// Qt
#include <QRadioButton>

// Application
#include "exclusivechoicewidget.h"
#include "ui_exclusivechoicewidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::ExclusiveChoiceWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::ExclusiveChoiceWidget), m_sDefaultValue("")
{

}

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::ExclusiveChoiceWidget(const QStringList &lLabels, const QStringList &lValues, const QString &sLabel, const QString &sDefaultValue, QWidget *parent) :
    QWidget(parent), ui(new Ui::ExclusiveChoiceWidget), m_sDefaultValue(sDefaultValue)
{
    ui->setupUi(this);
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
        connect(pRadioButton, &QRadioButton::clicked, this, &ExclusiveChoiceWidget::onRadioButtonClicked);
        ui->radioButtonArea->addWidget(pRadioButton);
        m_vRadioButtons << pRadioButton;
    }
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::applyDefaultValue()
{
    if (!m_sDefaultValue.isEmpty())
    {
        foreach (QRadioButton *pRadioButton, m_vRadioButtons)
        {
            QString sUserValue = pRadioButton->property(PROPERTY_USER_VALUE).toString();
            if (sUserValue == m_sDefaultValue)
            {
                pRadioButton->setChecked(true);
                break;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::onRadioButtonClicked(bool bClicked)
{
    QRadioButton *pSender = dynamic_cast<QRadioButton *>(sender());
    if (pSender && bClicked)
    {
        QString sUserValue = pSender->property(PROPERTY_USER_VALUE).toString();
        emit selectionChanged(sUserValue);
    }
}
