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

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

public slots:
    void on_open();
    void on_about();
    void on_bad_stl();
    void on_empty_mesh();
    void on_missing_file();
    void on_confusing_stl();

    void enable_open();
    void disable_open();

    void set_watched(const QString& filename);

private slots:
    void on_projection(QAction* proj);
    void on_watched_change(const QString& filename);
    void on_reload();
    void on_autoreload_triggered(bool r);
    void on_clear_recent();
    void on_load_recent(QAction* a);

private:
    void rebuild_recent_files();

    QAction* const open_action;
    QAction* const about_action;
    QAction* const quit_action;
    QAction* const perspective_action;
    QAction* const orthogonal_action;
    QAction* const reload_action;
    QAction* const autoreload_action;

    QMenu* const recent_files;
    QActionGroup* const recent_files_group;
    QAction* const recent_files_clear_action;
    const static int MAX_RECENT_FILES=8;
    const static QString RECENT_FILE_KEY;

    QFileSystemWatcher* watcher;

    Canvas* canvas;
};

#endif // STLWINDOW_H
