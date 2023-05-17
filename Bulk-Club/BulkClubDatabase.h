
#ifndef BULKCLUBDATABASE_H
#define BULKCLUBDATABASE_H

#include <QString>

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
class BulkClubDatabase
{
private:
    TransactionList m_transactionlist;
    MemberList m_memberlist;
    Inventory m_inventory;
public:
    BulkClubDatabase()
    {
        loadMemberData(MEMBER_PATH);
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
    }

};

#endif // BULKCLUBDATABASE_H
