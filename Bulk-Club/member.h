
#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QDataStream>

class Member
{
public:
    Member();
    QDataStream &operator<<(QDataStream &qd);
    QDataStream &operator>>(QDataStream &qd);
private:
    QString name;
    int number;
    QString exiration;
    float totalSpent;

};


#endif // MEMBER_H
