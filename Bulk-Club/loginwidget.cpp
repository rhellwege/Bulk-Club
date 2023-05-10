#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_pushButtonLogOut_clicked()
{
    emit updatePermissions("N/A");
}


void LoginWidget::on_pushButtonLogin_clicked()
{
    emit updatePermissions(ui->comboBoxLogin->currentText());
}

