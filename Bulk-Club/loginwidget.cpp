#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent), ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_pushButtonLogOut_clicked()
{
    emit updatePermissions(Permission::NONE);
}

void LoginWidget::on_pushButtonLogin_clicked()
{
    switch (ui->comboBoxLogin->currentIndex())
    {
    case 0:
        emit updatePermissions(Permission::MANAGER);
        break;
    case 1:
        emit updatePermissions(Permission::ADMINISTRATOR);
        break;
    default:
        emit updatePermissions(Permission::NONE);
    }
}
