
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QMessageBox>
#include "salesreportwidget.h"

#include <iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initialization for all children:
    updatePermissions("N/A");
    // setup salesreport
    SalesReportWidget *widgetSalesReport = new SalesReportWidget(this, &db);
    ui->gridLayoutSalesReport->addWidget(widgetSalesReport);

    // connect all signals
    connect(ui->widgetLogin, &LoginWidget::updatePermissions, this, &MainWindow::updatePermissions);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePermissions(QString loginStatus)
{
    //cout << "called updatePermissions" << endl;
    QString fmtLoggedin = "Logged in as: ";
    ui->statusbar->showMessage(fmtLoggedin.append(loginStatus));
    if (loginStatus == "Manager")
    {
        permission = Permission::MANAGER;
    }
    else if (loginStatus == "Administrator")
    {
        permission = Permission::ADMINISTRATOR;
    }
    else
    {
        permission = Permission::NONE;
    }
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

