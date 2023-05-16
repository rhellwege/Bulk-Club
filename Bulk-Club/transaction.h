
#ifndef TRANSACTION_H
#define TRANSACTION_H


class Transaction
{
public:
    QDataStreaam &operator<<(QDataStream &qd, const Transaction &m);
    QDataStreaam &operator>>(QDataStream &qd, Transaction &m);
    Transaction();
private:
    QString date;
    int membershipNumber;
    QString item;
    float price;
    int qty;
};

#endif // TRANSACTION_H
