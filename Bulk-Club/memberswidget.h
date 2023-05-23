#ifndef MEMBERSWIDGET_H
#define MEMBERSWIDGET_H

#include <QWidget>

#include "BulkClubDatabase.h"
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

namespace Ui
{
class MembersWidget;
}

// this is a model class for the table view inside the memberswidget
class MembersModel : public QAbstractTableModel
{
    Q_OBJECT
  public:
    MembersModel(QObject *parent = nullptr, BulkClubDatabase *db = nullptr) : QAbstractTableModel(parent)
    {
        this->db = db;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return db->members()->count();
    }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return (db->permission == Permission::ADMINISTRATOR) ? 7 : 6;
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole && role != Qt::EditRole)
            return {};
        Member &member = (*db->members())[index.row()];
        switch (index.column())
        {
        case 0:
            return member.id;
        case 1:
            return member.name;
        case 2:
            return member.expiration;
        case 3:
            return member.totalSpent;
        case 4:
            return member.type;
        case 5:
            return (member.type == "Regular") ? QVariant("N/A") : QVariant(member.totalRebate);
        case 6:
            return (member.shouldConvert()) ? "Yes" : "No";
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
            return "ID";
        case 1:
            return "Name";
        case 2:
            return "Expiration";
        case 3:
            return "Total Purchased";
        case 4:
            return "Type";
        case 5:
            return "Rebate";
        case 6:
            return "Convert";
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

class NameIDFilter : public QSortFilterProxyModel
{
    Q_OBJECT

  public:
    NameIDFilter(QObject *parent = 0, BulkClubDatabase *db = 0) : QSortFilterProxyModel(parent)
    {
        this->db = db;
        enabled = false;
        text = "";
        filterType = 0;
    }
    void setFilterType(int type)
    {
        this->filterType = type;
        invalidateFilter();
    }

    void filterByName()
    {
        this->filterType = 0;
        invalidateFilter();
    }

    void filterByID()
    {
        this->filterType = 1;
        invalidateFilter();
    }

    void filterText(QString str)
    {
        text = str;
        invalidateFilter();
    }

    void enable()
    {
        enabled = true;
        invalidateFilter();
    }
    void disable()
    {
        enabled = false;
        invalidateFilter();
    }

  protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        if (!enabled)
            return true;
        switch (filterType)
        {
        case 0:
            return db->members()->at(sourceRow).name.startsWith(text, Qt::CaseInsensitive);
        default:
            return QString::number(db->members()->at(sourceRow).id).startsWith(text, Qt::CaseSensitive);
        }
    }

  private:
    int filterType; // if 0: name, if 1: id, 2: disabled
    QString text;
    bool enabled;
    BulkClubDatabase *db;
};

class MembersWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit MembersWidget(QWidget *parent = nullptr, BulkClubDatabase *db = nullptr);
    ~MembersWidget();

  public slots:
    void dbUpdated();
    void updatePermissions(Permission permission);

  private slots:
    void on_buttonAddMember_clicked();

    void on_buttonRemoveSelected_clicked();

    void on_checkBoxFilter_stateChanged(int arg1);

    void on_comboBoxFilter_currentIndexChanged(int index);

    void on_lineEditFilter_textChanged(const QString &arg1);

  private:
    Ui::MembersWidget *ui;
    MembersModel *modelmembers;
    NameIDFilter *proxymembers;
    BulkClubDatabase *db;
    void updateConversions();
};

#endif // MEMBERSWIDGET_H
