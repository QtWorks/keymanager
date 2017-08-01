#ifndef LINEEDITTRIPLET_H
#define LINEEDITTRIPLET_H

#include <QWidget>

namespace Ui {
class LineEditTriplet;
}

class LineEditTriplet : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit LineEditTriplet(QWidget *parent = 0);

    //! Destructor
    ~LineEditTriplet();

    //! Return value
    QString value() const;

private:
    //! UI
    Ui::LineEditTriplet *ui;

signals:
    //! Value changed
    void valueChanged();
};

#endif // LINEEDITTRIPLET_H
