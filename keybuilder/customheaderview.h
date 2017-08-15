#ifndef CUSTOMHEADERVIEW_H
#define CUSTOMHEADERVIEW_H

// Qt
#include <QHeaderView>

// Application
class HeaderObject;

class CustomHeaderView : public QHeaderView
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CustomHeaderView(const QVector<QString> &vColumnLabels, QWidget *parent=nullptr);

    //! Destructor
    ~CustomHeaderView();

protected:
    //! Paint section
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

    //! Size hint
    QSize sizeHint() const;

private:
    //! Header sections
    QVector<HeaderObject *> m_vHeaderSections;

signals:
    //! Button clicked
    void clearClicked(int iColumnIndex);
};

#endif // CUSTOMHEADERVIEW_H
