#include "exclusivechoicewidget.h"
#include "ui_exclusivechoicewidget.h"

ExclusiveChoiceWidget::ExclusiveChoiceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExclusiveChoiceWidget)
{
    ui->setupUi(this);
}

ExclusiveChoiceWidget::~ExclusiveChoiceWidget()
{
    delete ui;
}
