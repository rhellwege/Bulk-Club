#include "inventorywidget.h"
#include "ui_inventorywidget.h"
#include <string>
#include "customdialog.h"

InventoryWidget::InventoryWidget(QWidget *parent, BulkClubDatabase *db) :
    QWidget(parent),
    ui(new Ui::InventoryWidget)
{
    ui->setupUi(this);
    this->db = db;

    invModel = new InventoryModel(this, db);
    proxyInventory = new QSortFilterProxyModel(this);
    proxyInventory->setSourceModel(invModel);
    proxyInventory->setFilterKeyColumn(0);
    ui->tableView->setModel(proxyInventory);
}

InventoryWidget::~InventoryWidget()
{
    delete ui;
}

void InventoryWidget::on_removeItem_clicked()
{
    if (db->permission < Permission::ADMINISTRATOR)
    {
        QMessageBox::information(this, "Permissions Error",
                                 "You must be logged in as administrator to remove a member");
        return;
    }
    QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
    if (selection.count() < 1)
    {
        QMessageBox::information(this, "Selection Error", "You must select at least 1 row to delete");
        return;
    }

    db->removeItemAt(proxyInventory->mapToSource(selection.at(0)).row());
}

void InventoryWidget::dbUpdated(){
    invModel->reset();
}


void InventoryWidget::on_addItemInv_clicked()
{
        if (db->permission < Permission::ADMINISTRATOR)
        {
            QMessageBox::information(this, "Permissions Error", "You must be logged in as administrator to add a member");
            return;
        }
        CustomDialog d("Add Item", this);
        Item item;
        int typeIdx = 0;
        string name;
        string price;
        string expiration;
        d.addLineEdit("Name: ", &name);
        d.addLineEdit("Price: ", &price);
        d.exec();

        if (d.wasCancelled())
            return;
        item.name = QString::fromStdString(name);
        item.price = atoi(price.c_str());
        item.totalSoldQty = 0;
        item.totalRevenue = 0;
        //member.type = (typeIdx == 0) ? "Regular" : "Executive";
        //member.expiration = QString::fromStdString(expiration);
        db->addItem(item);
}


void InventoryWidget::on_invSearch_textChanged(const QString &arg1)
{
    proxyInventory->setFilterFixedString(arg1);
}

