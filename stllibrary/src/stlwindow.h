#ifndef STLWINDOW_H
#define STLWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QFileSystemWatcher>
#include "stllibrary_global.h"

class Canvas;

class STLLIBRARYSHARED_EXPORT STLWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit STLWindow(QWidget* parent=0);
    bool load_stl(const QString& filename, bool is_reload=false);
    void viewOrthographic();
    void viewPerspective();

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

public slots:
    void on_open();
    void on_saveas();
    void on_bad_stl();
    void on_empty_mesh();
    void on_missing_file();
    void on_confusing_stl();
    void set_watched(const QString& filename);

private slots:
    void on_projection(QAction* proj);
    void on_watched_change(const QString& filename);
    void on_reload();
    void on_autoreload_triggered(bool r);
    void on_load_recent(QAction* a);

private:
    QAction* const open_action;
    QAction* const saveas_action;
    QAction* const perspective_action;
    QAction* const orthogonal_action;
    QString currentSTLFileName;
    QFileSystemWatcher* watcher;
    Canvas* canvas;
};

#endif // STLWINDOW_H
