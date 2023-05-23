#include "memberswidget.h"
#include "ui_memberswidget.h"

#include "customdialog.h"

#include <QMessageBox>
#include <QDialog>
#include <QSortFilterProxyModel>

MembersWidget::MembersWidget(QWidget *parent, BulkClubDatabase* db) :
    QWidget(parent),
    ui(new Ui::MembersWidget)
{
    ui->setupUi(this);
    this->db = db;

    // create the model:
    modelmembers = new MembersModel(this, db);
    proxymembers = new QSortFilterProxyModel(this);
    proxymembers->setSourceModel(modelmembers);
    proxymembers->sort(0); // sort by id
    ui->tableViewMembers->setModel(proxymembers);
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
    CustomDialog d("Add Member", this);
    Member member;
    int typeIdx = 0;
    string name;
    string id;
    string expiration;
    d.addLineEdit("ID: ", &id);
    d.addLineEdit("Name: ", &name, "Full name");
    d.addComboBox("Type: ", "Regular|Executive", &typeIdx);
    d.addLineEdit("Expiration: ", &expiration);
    d.exec();

    if (d.wasCancelled()) return;
    member.name = QString::fromStdString(name);
    member.id = atoi(id.c_str());
    member.type = (typeIdx == 0) ? "Regular" : "Executive";
    member.expiration = QString::fromStdString(expiration);
    db->addMember(member);

}

void MembersWidget::on_buttonRemoveSelected_clicked()
{
    if (db->permission < Permission::ADMINISTRATOR)
    {
        QMessageBox::information(this, "Permissions Error",
                                 "You must be logged in as administrator to remove a member");
        return;
    }
    QModelIndexList selection = ui->tableViewMembers->selectionModel()->selectedIndexes();
    //proxymembers->mapToSource()
    if (selection.count() < 1)
    {
        QMessageBox::information(this, "Selection Error",
                                 "You must select at least 1 row to delete");
        return;
    }

    db->removeMemberAt(proxymembers->mapToSource(selection.at(0)).row());


}

void MembersWidget::dbUpdated()
{
    qDebug() << "dbUpdated in members";
    modelmembers->reset();
}




