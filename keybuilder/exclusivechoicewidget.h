#ifndef EXCLUSIVECHOICEWIDGET_H
#define EXCLUSIVECHOICEWIDGET_H

#include <QWidget>

namespace Ui {
class ExclusiveChoiceWidget;
}

class ExclusiveChoiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExclusiveChoiceWidget(QWidget *parent = 0);
    ~ExclusiveChoiceWidget();

private:
    Ui::ExclusiveChoiceWidget *ui;
};

#endif // EXCLUSIVECHOICEWIDGET_H
