// Qt
#include <QRadioButton>

// Application
#include "exclusivechoicewidget.h"
#include "ui_exclusivechoicewidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::ExclusiveChoiceWidget(const QVector<QString> &vLabels, const QVector<QString> &vValues, const QString &sLabel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExclusiveChoiceWidget)
{
    ui->setupUi(this);
    ui->labelArea->setVisible(!sLabel.simplified().isEmpty());
    ui->labelArea->setText(sLabel.simplified());
    setup(vLabels, vValues);
}

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::~ExclusiveChoiceWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::setup(const QVector<QString> &vLabels, const QVector<QString> &vValues)
{
    int nItems = qMin(vLabels.size(), vValues.size());
    for (int i=0; i<nItems; i++)
    {
        QRadioButton *pRadioButton = new QRadioButton(vLabels[i], this);
        pRadioButton->setAutoExclusive(true);
        pRadioButton->setProperty(PROPERTY_USER_VALUE, vValues[i]);
        connect(pRadioButton, &QRadioButton::clicked, this, &ExclusiveChoiceWidget::onRadioButtonClicked);
        ui->radioButtonArea->addWidget(pRadioButton);
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
