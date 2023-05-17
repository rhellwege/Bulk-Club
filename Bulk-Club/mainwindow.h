
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BulkClubDatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updatePermissions(QString loginStatus);
    void on_tabWidgetMain_currentChanged(int index);

private:
    // TODO: add permissions enum
    enum Permission {
        NONE = 0,
        MANAGER,
        ADMINISTRATOR
    } permission;

    Ui::MainWindow *ui;
    BulkClubDatabase db;

};

#endif // MAINWINDOW_H
