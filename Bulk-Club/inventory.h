
#ifndef INVENTORY_H
#define INVENTORY_H

#include "transaction.h"
#include <QList>
#include <QMap>
#include <QString>

struct Item
{
    QString name;
    float price;

    int totalSoldQty;
    float totalRevenue;
};

class Inventory
{
  private:
    QList<Item> m_data;

  public:
    Inventory()
    {
    }
    int count()
    {
        return m_data.count();
    }
    Item &operator[](int idx)
    {
        return m_data[idx];
    }
    Item &at(int idx)
    {
        return m_data[idx];
    }

    void removeAt(int idx)
    {
        m_data.removeAt(idx);
    }

    void append(Item &i){
        m_data.append(i);
    }

    Item *findName(QString name)
    {
        for (auto item : m_data)
        {
            if (item.name == name)
                return &item;
        }
        return nullptr;
    }

    void processTransaction(Transaction &t)
    {
        Item *item = findName(t.item);
        if (item == nullptr) // insert a new entry for this item
        {
            Item i;
            i.name = t.item;
            i.price = t.price;
            i.totalRevenue = t.total();
            i.totalSoldQty = t.qty;
            m_data.append(i);
            return;
        }
        // entry already exists, so update the info:
        item->totalRevenue += t.total();
        item->totalSoldQty += t.qty;
    }

    void processAllTransactions(TransactionList &t)
    {
        for (int i = 0; i < t.count(); ++i)
        {
            processTransaction(t[i]);
        }
    }
};

#endif // INVENTORY_H
