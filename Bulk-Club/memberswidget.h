#ifndef MEMBERSWIDGET_H
#define MEMBERSWIDGET_H

#include <QWidget>

#include "BulkClubDatabase.h"
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class MembersWidget;
}

class MembersModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MembersModel(QObject *parent = nullptr, BulkClubDatabase* db = nullptr)
        : QAbstractTableModel(parent)
    {
        this->db = db;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {return db->members()->count();}
    int columnCount(const QModelIndex &parent = QModelIndex()) const override {return 4;}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
        Member & member = (*db->members())[index.row()];
        switch (index.column()) {
        case 0: return member.id;
        case 1: return member.name;
        case 2: return member.type;
        case 3: return member.totalSpent;
        default: return {};
        };
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
        switch (section) {
        case 0: return "ID";
        case 1: return "Name";
        case 2: return "Type";
        case 3: return "Total Purchased";
        default: return {};
        }
    }
private:
    BulkClubDatabase* db;

};


class MembersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MembersWidget(QWidget *parent = nullptr, BulkClubDatabase* db = nullptr);
    ~MembersWidget();

public slots:
    void dbUpdated();

private slots:
    void on_buttonAddMember_clicked();

    void on_buttonRemoveSelected_clicked();

private:
    Ui::MembersWidget *ui;
    MembersModel *membersmodel;
    BulkClubDatabase *db;
};

#endif // MEMBERSWIDGET_H
