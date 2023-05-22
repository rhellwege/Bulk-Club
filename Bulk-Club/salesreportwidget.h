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
    int columnCount(const QModelIndex &parent = QModelIndex()) const override {return 4;}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
        Transaction & transaction = (*db->transactions())[index.row()];
        switch (index.column()) {
        case 0: return transaction.date;
        case 1: return transaction.item;
        case 2: return transaction.qty;
        case 3: return transaction.total() + (transaction.total()*TAX_RATE);
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
        case 3: return "$ (after tax)";
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
        //qDebug() << member->name;
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

class FilterItemsProxy: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FilterItemsProxy(QObject *parent = 0, BulkClubDatabase* db = 0) : QSortFilterProxyModel(parent) {this->db = db;}
    void setFilterDate(QString date) {
        this->date = date;
        invalidateFilter();
    }
    void setFilterMemberType(QString str) {
        memberType = str;
        invalidateFilter(); // update the filter
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        Transaction& t = (*db->transactions())[sourceRow];
        Member *member = db->members()->findId(t.memberID);
        if (member == nullptr) return false;
        return (member->type == this->memberType || this->memberType == "Any") && (t.date == this->date);
        //return false;
    }

private:
    QString memberType;
    QString date;
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

public slots:
    void dbUpdated();

private slots:
    void on_dateEdit_userDateChanged(const QDate &date);

    void on_comboBoxFilter_currentIndexChanged(int index);

private:
    void updateTotalRevenue();
    void countShoppers();
    Ui::SalesReportWidget *ui;
    BulkClubDatabase* db;
    SalesReportShoppersModel* modelShoppers;
    FilterItemsProxy* proxyItems;
    QSortFilterProxyModel* proxyShoppers;
    float totalRevenue;
    int regularCount;
    int executiveCount;

};

#endif // SALESREPORTWIDGET_H
