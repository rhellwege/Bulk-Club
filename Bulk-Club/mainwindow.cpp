
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "memberswidget.h"
#include "salesreportwidget.h"
#include <QMessageBox>
#include <QStatusBar>

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidgetMain->setCurrentIndex(0);

    // initialization for all children:
    updatePermissions(Permission::NONE);
    connect(ui->widgetLogin, &LoginWidget::updatePermissions, this, &MainWindow::updatePermissions);
    // setup salesreport
    SalesReportWidget *widgetSalesReport = new SalesReportWidget(this, &db);
    ui->gridLayoutSalesReport->addWidget(widgetSalesReport);
    connect(&db, &BulkClubDatabase::dbUpdated, widgetSalesReport, &SalesReportWidget::dbUpdated);

    MembersWidget *widgetMembers = new MembersWidget(this, &db);
    ui->gridLayoutMembers->addWidget(widgetMembers);
    connect(&db, &BulkClubDatabase::dbUpdated, widgetMembers, &MembersWidget::dbUpdated);
    connect(ui->widgetLogin, &LoginWidget::updatePermissions, widgetMembers,
            &MembersWidget::updatePermissions); // handle update login event inside the members widget
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePermissions(Permission permission)
{
    // cout << "called updatePermissions" << endl;
    QString fmtLoggedin = "Logged in as: ";
    QString loginStatus;
    switch (permission)
    {
    case Permission::MANAGER:
        loginStatus = "Manager";
        break;
    case Permission::ADMINISTRATOR:
        loginStatus = "Administrator";
        break;
    default:
        loginStatus = "N/A";
    }

    ui->statusbar->showMessage(fmtLoggedin.append(loginStatus));
    db.permission = permission;
}

void MainWindow::on_tabWidgetMain_currentChanged(int index)
{
    if (index == 0)
        return; // don't bother checking anything if the user chooses the login page
    switch (db.permission)
    {
    case Permission::NONE:
        ui->tabWidgetMain->setCurrentIndex(0); // force the tab to switch to the login page
        // notify the user to login
        QMessageBox::information(
            this, "Permissions Error",
            "You cannot accecss this page unless you are logged in as a manager or administrator.");
        break;
    }
}
