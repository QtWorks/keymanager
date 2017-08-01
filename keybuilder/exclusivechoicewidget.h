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
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit ExclusiveChoiceWidget(const QVector<QString> &vLabels, const QVector<QString> &vValues, const QString &sLabel, QWidget *parent = 0);

    //! Destructor
    ~ExclusiveChoiceWidget();

private:
    //! Setup
    void setup(const QVector<QString> &vLabels, const QVector<QString> &vValues);

private:
    //! UI
    Ui::ExclusiveChoiceWidget *ui;

public slots:
    //! Radio button clicked
    void onRadioButtonClicked(bool bClicked);

signals:
    //! Selection changed
    void selectionChanged(const QString &sSelection);
};

#endif // EXCLUSIVECHOICEWIDGET_H
