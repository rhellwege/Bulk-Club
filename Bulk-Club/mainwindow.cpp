
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStatusBar>
#include <QMessageBox>
#include "salesreportwidget.h"
#include "memberswidget.h"

#include <iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initialization for all children:
    updatePermissions("N/A");
    connect(ui->widgetLogin, &LoginWidget::updatePermissions, this, &MainWindow::updatePermissions);
    // setup salesreport
    SalesReportWidget *widgetSalesReport = new SalesReportWidget(this, &db);
    ui->gridLayoutSalesReport->addWidget(widgetSalesReport);
    connect(&db, &BulkClubDatabase::dbUpdated, widgetSalesReport, &SalesReportWidget::dbUpdated);

    MembersWidget *widgetMembers = new MembersWidget(this, &db);
    ui->gridLayoutMembers->addWidget(widgetMembers);
    connect(&db, &BulkClubDatabase::dbUpdated, widgetMembers, &MembersWidget::dbUpdated);
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
        db.permission = Permission::MANAGER;
    }
    else if (loginStatus == "Administrator")
    {
        db.permission = Permission::ADMINISTRATOR;
    }
    else
    {
        db.permission = Permission::NONE;
    }
}

void MainWindow::on_tabWidgetMain_currentChanged(int index)
{
    if (index == 0) return; // don't bother checking anything if the user chooses the login page
    switch (db.permission)
    {
    case Permission::NONE:
        ui->tabWidgetMain->setCurrentIndex(0); // force the tab to switch to the login page
        // notify the user to login
        QMessageBox::information(this, "Permissions Error",
                                 "You cannot accecss this page unless you are logged in as a manager or administrator.");
        break;
    }
}

