#ifndef SALESREPORTWIDGET_H
#define SALESREPORTWIDGET_H

#include <QWidget>
#include "BulkClubDatabase.h"

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

// models for the table views:
class SalesReportItemsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SalesReportItemsModel(QObject *parent = nullptr, BulkClubDatabase* db = nullptr)
    : QAbstractTableModel(parent)
    {
        this->db = db;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {return db->transactions()->count();}
    int columnCount(const QModelIndex &parent = QModelIndex()) const override {return 3;}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
        Transaction & transaction = (*db->transactions())[index.row()];
        switch (index.column()) {
        case 0: return transaction.date;
        case 1: return transaction.item;
        case 2: return transaction.qty;
        default: return {};
        };
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
        switch (section) {
        case 0: return "Date";
        case 1: return "Item";
        case 2: return "Qty";
        default: return {};
        }
    }
private:
    BulkClubDatabase* db;
};


namespace Ui {
class SalesReportWidget;
}

class SalesReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SalesReportWidget(QWidget *parent = nullptr, BulkClubDatabase* db = nullptr);
    ~SalesReportWidget();

private slots:
    void on_dateEdit_userDateChanged(const QDate &date);

private:
    void updateTotalRevenue();
    void countShoppers();
    Ui::SalesReportWidget *ui;
    BulkClubDatabase* db;
    QSortFilterProxyModel* proxyItems;
    QSortFilterProxyModel* proxyNames;
    float totalRevenue;
    int regularCount;
    int executiveCount;
};

#endif // SALESREPORTWIDGET_H
