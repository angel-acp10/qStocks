#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QtSql>
#include <QMetaEnum>
#include <QObject>

class DataBase : public QObject
{
    Q_OBJECT
public:
    typedef struct{
        QString name;
        bool canBeNull;
        int value;
        int minValue;
        int maxValue;
        QString description;
    }setting_t;

    typedef struct{
        QString id;
        QString name;
        QString ticker;
    }security_t;

    enum CurrencyType{
        USD = 1,  EUR, JPY, GBP, AUD, CAD, CHF, CNY, HKD, NZD, SEK, KRW, SGD, NOK,
        MXN, INR, RUB, ZAR, TRY, BRL, TWD, DKK, PLN, THB, IDR, HUF, CZK, ILS, CLP,
        PHP, AED, COP, SAR, MYR, RON,
    };
    Q_ENUM(CurrencyType)
    int max_currencyType = (int)RON;

    typedef struct{
        QDateTime dateTime;
        security_t security;
        int qty;
        double locUnitPrice;
        enum CurrencyType locCurrency;
        double exchRate;
        double value;
        double commissions;
        double total;
        enum CurrencyType currency;
    }transaction_t;

    DataBase(const QString & dbFile);
    ~DataBase();

    bool open();
    void close();

    bool init();

    // settings table
    bool settingsTable_createIfNotExists();

    // currencies table
    bool currenciesTable_createIfNotExists();

    // securities table
    bool securitiesTable_createIfNotExists();
    bool securitiesTable_addSecurity(security_t &s);

    // transactions table
    bool transactionsTable_createIfNotExists();
    bool transactionsTable_addTransaction(transaction_t &t);

    // transactions view
    bool transactionsView_createIfNotExists();

    // stock list table
    /*
    bool stockListTable_exists(void);
    bool stockListTable_createIfNotExists(void);
    bool stockListTable_update(void); // call to update stocks table
    */

    // stocks table
    /*
    bool existsStockPriceTable(QString isin);
    bool createStockPriceTable(QString isin);
    bool updateStockPriceTable(QString isin);
    */

    DataBase::CurrencyType getCurrencyEnum(const QString &text);
    QString getCurrencyString(const DataBase::CurrencyType &enumValue);

private:
    QString dbFile;
};

#endif // DATABASE_H
