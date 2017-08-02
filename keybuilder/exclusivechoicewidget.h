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
    explicit ExclusiveChoiceWidget(const QStringList &lLabels, const QStringList &lValues, const QString &sLabel, QWidget *parent=nullptr);

    //! Destructor
    ~ExclusiveChoiceWidget();

private:
    //! Setup
    void setup(const QStringList &vLabels, const QStringList &vValues);

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
