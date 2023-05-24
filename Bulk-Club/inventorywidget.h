#ifndef INVENTORYWIDGET_H
#define INVENTORYWIDGET_H

#include <QWidget>

#include "BulkClubDatabase.h"
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class InventoryWidget;
}

// this is a model class for the table view inside the memberswidget
class InventoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    InventoryModel(QObject *parent = nullptr, BulkClubDatabase *db = nullptr) : QAbstractTableModel(parent)
    {
        this->db = db;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return db->inventory()->count();
    }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return 4;
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole && role != Qt::EditRole)
            return {};
        Item &item = (*db->inventory())[index.row()];
        switch (index.column())
        {
        case 0:
            return item.name;
        case 1:
            return item.price;
        case 2:
            return item.totalSoldQty;
        case 3:
            return item.totalRevenue;
        default:
            return {};
        };
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};
        switch (section)
        {
        case 0:
            return "Name";
        case 1:
            return "Price";
        case 2:
            return "Quantity";
        case 3:
            return "Revenue";
        default:
            return {};
        }
    }
    void reset()
    {
        beginResetModel();
        endResetModel();
    }

private:
    BulkClubDatabase *db;
};

class InventoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InventoryWidget(QWidget *parent = nullptr, BulkClubDatabase *db = nullptr);
    ~InventoryWidget();

public slots:
    void dbUpdated();

private slots:
    void on_removeItem_clicked();


    void on_addItemInv_clicked();

    void on_invSearch_textChanged(const QString &arg1);

private:
    Ui::InventoryWidget *ui;
    BulkClubDatabase *db;
    QSortFilterProxyModel *proxyInventory;
    InventoryModel *invModel;
};

#endif // INVENTORYWIDGET_H
