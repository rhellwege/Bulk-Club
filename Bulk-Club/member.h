
#ifndef MEMBER_H
#define MEMBER_H

#include <QString>
#include <QTextStream>
#include <QList>
#include <QMessageBox>
#include "transaction.h"

const float TAX_RATE = 0.0775;
const float REBATE_RATE = 0.02;
const float REGULAR_DUES = 65.0;
const float EXECUTIVE_DUES = 120.0;

struct Member
{
    Member()
    {
        totalSpent = 0;
        totalRebate = 0;
    }
    friend QTextStream &operator<<(QTextStream &ts, const Member &m)
    {
        ts << m.name << '\n' << m.id << '\n' << m.type << '\n' << m.expiration << '\n' << m.totalSpent << '\n';
    }
    friend QTextStream &operator>>(QTextStream &ts, Member &m)
    {
        m.name = ts.readLine();
        m.id = ts.readLine().toInt();
        m.type = ts.readLine();
        m.expiration = ts.readLine();
    }

    QString name;
    int id;
    QString type;
    QString expiration;

    float totalSpent;
    float totalRebate;

    bool shouldConvert()
    {
        if (type == "Executive")
            return REGULAR_DUES < (EXECUTIVE_DUES-totalRebate);
        else
            return ((totalSpent-(totalSpent*TAX_RATE))*REBATE_RATE) > (EXECUTIVE_DUES-REGULAR_DUES);
        return false;
    }
};


class MemberList
{
private:
    QList<Member> m_data;
public:
    MemberList() {}
    ~MemberList() {}

    bool processTransaction(Transaction& t)
    {
        //qDebug() << "Processing member id: " << t.memberID;
        Member* m = findId(t.memberID);
        if (m == nullptr) return false;
        //qDebug() << "Name: " << m->name;
        if (m->type == "Executive")
        {
            m->totalRebate += t.total() * REBATE_RATE; // rebates are before tax
        }
        m->totalSpent += (t.total() + t.total() * TAX_RATE);
    }

    void processAllTransactions(TransactionList& t)
    {
        for (int i = 0; i < t.count(); ++i)
        {
            processTransaction(t[i]);
        }
    }

    int count()
    {
        return m_data.count();
    }

    Member* findId(int id)
    {
        for (Member& m : m_data)
        {
            if (m.id == id) return &m;
        }
        return nullptr;
    }

    Member& operator[](int idx)
    {
        return m_data[idx];
    }

    void append(Member& m)
    {
        m_data.append(m);
    }

    Member& at(int idx)
    {
        return m_data[idx];
    }

    void appendFromFile(QString path)
    {
        QFile file(path);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString message = "Could not open ";
            message.append(path);
            QMessageBox::information(nullptr, "File Error", message, false);
            return;
        }
        QTextStream stream(&file);
        // read every line and append the list widget with every line.
        while (!stream.atEnd())
        {
            Member m;
            stream >> m;
            append(m); // update the underlying data
        }
        file.flush();
        file.close();
    }

    void removeAt(int idx)
    {
        m_data.removeAt(idx);
    }
};

#endif // MEMBER_H
