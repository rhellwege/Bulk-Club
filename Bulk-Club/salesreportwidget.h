#ifndef SALESREPORTWIDGET_H
#define SALESREPORTWIDGET_H

#include <QWidget>
#include <QSet>
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

class SalesReportShoppersModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SalesReportShoppersModel(QObject *parent = nullptr, BulkClubDatabase* db = nullptr)
        : QAbstractTableModel(parent)
    {
        this->db = db;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {return uniqueIDs.count();}
    int columnCount(const QModelIndex &parent = QModelIndex()) const override {return 2;}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
        int id = this->at(index.row());

        Member* member = db->members()->findId(id);
        qDebug() << member->name;
        if (member == nullptr) return {};
        switch (index.column()) {
        case 0: return member->name;
        case 1: return member->type;
        default: return {};
        };
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
        switch (section) {
        case 0: return "Name";
        case 1: return "Type";
        default: return {};
        }
    }

    void updateUnique(QString date)
    {
        beginResetModel();
        uniqueIDs.clear();
        for (int i = 0; i < db->transactions()->count(); ++i)
        {
            Transaction& t = (*db->transactions())[i];
            if (t.date == date) uniqueIDs.insert(t.memberID);
        }
        qDebug() << "unique ids count: " << uniqueIDs.count();
        endResetModel();
    }

    int at(int idx) const
    {
        return uniqueIDs.values().at(idx);
    }

private:
    BulkClubDatabase* db;
    QSet<int> uniqueIDs;
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
    SalesReportShoppersModel* modelShoppers;
    QSortFilterProxyModel* proxyItems;
    //QSortFilterProxyModel* proxyShoppers;
    float totalRevenue;
    int regularCount;
    int executiveCount;

};

#endif // SALESREPORTWIDGET_H
