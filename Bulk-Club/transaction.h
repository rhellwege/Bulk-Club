
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>

struct Transaction
{

    friend QTextStream &operator<<(QTextStream &ts, const Transaction& t)
    {
        ts << t.date << '\n' << t.memberID << '\n' << t.item << '\n' << t.price << '\n' << t.qty << '\n';
    }
    friend QTextStream &operator>>(QTextStream &ts, Transaction& t)
    {
        t.date = ts.readLine();
        t.memberID = ts.readLine().toInt();
        t.item = ts.readLine();
        t.price = ts.readLine().toFloat();
        t.qty = ts.readLine().toInt();
    }

    QString date;
    int memberID;
    QString item;
    float price;
    int qty;

    float total() // BEFORE TAX
    {
        return price * qty;
    }
};

//QTextStream &operator<<(QTextStream &ts, const Transaction &t)


//QTextStream &operator>>(QTextStream &ts, Transaction &t)



class TransactionList
{
private:
    QList<Transaction> m_data;
public:
    TransactionList() {}
    ~TransactionList() {}

    Transaction& operator[](int idx)
    {
        return m_data[idx];
    }

    Transaction& at(int idx)
    {
        return m_data[idx];
    }

    void append(Transaction& m)
    {
        m_data.append(m);
    }

    int count()
    {
        return m_data.count();
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
            Transaction t;
            stream >> t;
            append(t); // update the underlying data
        }
        file.flush();
        file.close();
    }

    void removeAt(int idx)
    {
        m_data.removeAt(idx);
    }
};


#endif // TRANSACTION_H
