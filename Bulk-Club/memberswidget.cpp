#include "memberswidget.h"
#include "ui_memberswidget.h"

#include <QMessageBox>

MembersWidget::MembersWidget(QWidget *parent, BulkClubDatabase* db) :
    QWidget(parent),
    ui(new Ui::MembersWidget)
{
    ui->setupUi(this);
    this->db = db;

    // create the model:
    membersmodel = new MembersModel(this, db);
    ui->tableViewMembers->setModel(membersmodel);
}

MembersWidget::~MembersWidget()
{
    delete ui;
}

void MembersWidget::on_buttonAddMember_clicked()
{
    if (db->permission < Permission::ADMINISTRATOR)
    {
        QMessageBox::information(this, "Permissions Error",
         "You must be logged in as administrator to add a member");
        return;
    }
    // dialogue that creates a member, then call db->addMember()
}

void MembersWidget::on_buttonRemoveSelected_clicked()
{
    if (db->permission < Permission::ADMINISTRATOR)
    {
        QMessageBox::information(this, "Permissions Error",
                                 "You must be logged in as administrator to remove a member");
        return;
    }
}

void MembersWidget::dbUpdated()
{

}




