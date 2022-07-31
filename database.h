#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QtSql>

class DataBase
{
public:
    typedef struct{
        QDateTime dateTime;
        QString product;
        QString ticker;
        QString isin;
        QString market;
        int qty;
        double locUnitPrice;
        double locValue;
        QString locCurrency;
        QString currency;
        double exchRate;
        double value;
        double commissions;
        double total;
    }transaction_t;

    DataBase(const QString & dbFile);
    ~DataBase();
    bool existsTransactionsTable(void);
    bool createTransactionsTable(void);
    bool addTransaction(transaction_t &t);

private:
    QString dbFile;
};

#endif // DATABASE_H
