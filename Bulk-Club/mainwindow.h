
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

enum Permission {
    NONE = 0,
    MANAGER,
    ADMINISTRATOR
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonLogin_clicked();

    void on_pushButtonLogOut_clicked();

    void on_tabWidgetMain_currentChanged(int index);

private:
    // TODO: add permissions enum
    Permission permission;
    Ui::MainWindow *ui;
    void updateLoggedInStatus(const QString& loggedInMessage);
};

#endif // MAINWINDOW_H
