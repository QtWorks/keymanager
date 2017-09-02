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
