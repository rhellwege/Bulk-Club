
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDataStream>

class Transaction
{
public:
    QDataStream &operator<<(QDataStream &qd);
    QDataStream &operator>>(QDataStream &qd);
    Transaction();
private:
    QString date;
    int membershipNumber;
    QString item;
    float price;
    int qty;
};

#endif // TRANSACTION_H
