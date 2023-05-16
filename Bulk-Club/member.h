
#ifndef MEMBER_H
#define MEMBER_H


class Member
{
public:
    Member();
    QDataStreaam &operator<<(QDataStream &qd, const Member &m);
    QDataStreaam &operator>>(QDataStream &qd, Member &m);
private:
    QString name;
    int number;
    QString exiration;
    float totalSpent;

};


#endif // MEMBER_H
