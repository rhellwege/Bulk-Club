
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    permission = Permission::NONE;

    updateLoggedInStatus("Logged in as: N/A");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateLoggedInStatus(const QString &loggedInMessage)
{
    ui->statusbar->showMessage(loggedInMessage);
}



void MainWindow::on_pushButtonLogin_clicked()
{
    QString loginMessage = "Logged in as: ";
    QString permissionString = ui->comboBoxLogin->currentText();

    updateLoggedInStatus(loginMessage.append(permissionString));
    // update permission:
    if (permissionString == "Manager")
    {
        permission = Permission::MANAGER;
    }
    else if (permissionString == "Administrator")
    {
        permission = Permission::ADMINISTRATOR;
    }

}


void MainWindow::on_pushButtonLogOut_clicked()
{
    updateLoggedInStatus("Logged in as: N/A");
    permission = Permission::NONE;
}


void MainWindow::on_tabWidgetMain_currentChanged(int index)
{
    if (index == 0) return; // don't bother checking anything if the user chooses the login page
    switch (permission)
    {
    case Permission::NONE:
        ui->tabWidgetMain->setCurrentIndex(0); // force the tab to switch to the login page
        // notify the user to login
        QMessageBox::information(this, "Permissions Error",
                                 "You cannot accecss this page unless you are logged in as a manager or administrator.");
        break;
    }
}

