
#ifndef BULKCLUBDATABASE_H
#define BULKCLUBDATABASE_H

#include <QString>
#include <QObject>

#include "inventory.h"
#include "transaction.h"
#include "member.h"

const QString MEMBER_PATH = ":/transactions/warehouse shoppers.txt";
const QString TRANSACTION_PATHS[7] = {":/transactions/day1.txt",
                                      ":/transactions/day2.txt",
                                      ":/transactions/day3.txt",
                                      ":/transactions/day4.txt",
                                      ":/transactions/day5.txt",
                                      ":/transactions/day6.txt",
                                      ":/transactions/day7.txt"};

// This class holds all the data
class BulkClubDatabase //: public QObject
{
    //Q_OBJECT
private:
    TransactionList m_transactionlist;
    MemberList m_memberlist;
    Inventory m_inventory;
signals:
    void newTransaction();
public:
    BulkClubDatabase()
    {
        qDebug() << "Loading files into database...";
        loadMemberData(MEMBER_PATH);
        qDebug() << "Memberlist count: " << m_memberlist.count();
        for (auto path : TRANSACTION_PATHS)
        {
            loadTransactionData(path);
        }
        updateAll(); // after loading everything update the information in inventory and memberlist
    }
    ~BulkClubDatabase() {}

    TransactionList* transactions() {return &m_transactionlist;}
    MemberList* members() {return &m_memberlist;}
    Inventory* inventory() {return &m_inventory;}

    void loadMemberData(QString path)
    {
        m_memberlist.appendFromFile(path);
    }

    void loadTransactionData(QString path)
    {
        m_transactionlist.appendFromFile(path);
    }

    // this function updates all the info in members and inventory based on the transactionlist
    void updateAll()
    {
        m_memberlist.processAllTransactions(m_transactionlist);
        m_inventory.processAllTransactions(m_transactionlist);
    }

    void addTransaction(Transaction& t)
    {
        m_transactionlist.append(t); // add the transaction to the transactionlist
        m_memberlist.processTransaction(t);
        m_inventory.processTransaction(t);
        emit newTransaction(); // tell everyone that there is a new transaction
    }

};

#endif // BULKCLUBDATABASE_H
