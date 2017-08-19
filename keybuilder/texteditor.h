#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

// Qt
#include <QPlainTextEdit>
#include <QObject>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

// Application
class LineNumberArea;

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    TextEditor(QWidget *parent=nullptr);

    //! Destructor
    ~TextEditor();

    //! Handle line number painting
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    //! Return line number area width
    int lineNumberAreaWidth();

    //! Load
    void load(const QString &sText);

    //! Do search
    void doSearch(const QString &sTargetString);

protected:
    //! Handle resize event
    void resizeEvent(QResizeEvent *event);

private slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Update line number area width
    void updateLineNumberAreaWidth(int iNewBlockCount);

    //! Highlight current line
    void highlightCurrentLine();

    //! Update line number area
    void updateLineNumberArea(const QRect &, int);

private:
    //! Line number area
    QWidget *m_pLineNumberArea;

    //! First time?
    bool m_bIsFirstTime;
};

class LineNumberArea : public QWidget
{
public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    LineNumberArea(TextEditor *pEditor) : QWidget(pEditor)
    {
        m_pTextEditor = pEditor;
    }

    //! Destructor
    ~LineNumberArea()
    {

    }

    //! Return size hint
    QSize sizeHint() const
    {
        return QSize(m_pTextEditor->lineNumberAreaWidth(), 0);
    }

protected:
    //! Handle paint event
    void paintEvent(QPaintEvent *event)
    {
        m_pTextEditor->lineNumberAreaPaintEvent(event);
    }

private:
    //! Text editor
    TextEditor *m_pTextEditor;
};

#endif
