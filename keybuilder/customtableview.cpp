// Qt
#include <QHeaderView>

// Application
#include "customtableview.h"

//-------------------------------------------------------------------------------------------------

CustomTableView::CustomTableView(QWidget *parent) : QTableView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
}

//-------------------------------------------------------------------------------------------------

CustomTableView::~CustomTableView()
{

}

//-------------------------------------------------------------------------------------------------

void CustomTableView::onRowCountChanged()
{
    QSize size(QTableView::sizeHint());
    if (model())
    {
        int iHeight = horizontalHeader()->height();
        int iRowCount = (model() != nullptr) ? model()->rowCount() : 0;
        for (int i=0; i<iRowCount; i++)
            iHeight += rowHeight(i);
        size.setHeight(iHeight);
    }
    setFixedHeight(size.height());
}
