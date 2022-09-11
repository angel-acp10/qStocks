#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QtSql>
#include <QMetaEnum>
#include <QObject>
#include <QQueue>

#include "yahooapi.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    ///////////
    // Types
    ///////////
    typedef struct{
        QString name;
        bool canBeNull;
        int value;
        int minValue;
        int maxValue;
        QString description;
    }setting_t;

    enum BrokerEnum{
        BROKER_DEGIRO = 1,
    };
    Q_ENUM(BrokerEnum)
    const int totalBrokers = 1;

    enum ApiEnum{
        API_YAHOO = 1,
    };
    Q_ENUM(ApiEnum)
    const int totalApis = 1;

    enum PriceEnum{
        PRICE_SECURITY = 1,
        PRICE_CURRENCY,
    };

    typedef struct{
        int id;
        QString name;
        QColor color;
        QString description;
    }watchGroup_t;


    typedef struct{
        int id;
        QString name;
        enum ApiEnum apiId;
        QString apiTicker;
    }currency_t;

    typedef struct{
        int id;
        QString isin;
        QString name;
        QString ticker;
        currency_t currency;
        QString notes;
        enum ApiEnum apiId;
        QString apiTicker;
    }security_t;

    typedef struct{
        int id;
        security_t security;
        watchGroup_t watchGroup;
    }watchItem_t;

    typedef struct{
        int id;
        int timeStamp;
        security_t security;
        double qty;
        double locUnitPrice;
        double exchRate;
        double value;
        double commissions;
        currency_t currency;
        enum BrokerEnum brokerId;
    }transaction_t;

    typedef struct{
        security_t security;
        QVector<qint64> timeStamp;
        QVector<double> open;
        QVector<double> close;
        QVector<double> high;
        QVector<double> low;
        QVector<double> volume;
    }securityPrice_t;

    typedef struct{
        currency_t currency;
        QVector<qint64> timeStamp;
        QVector<double> open;
        QVector<double> close;
        QVector<double> high;
        QVector<double> low;
    }currencyPrice_t;

    DataBase(const QString & dbFile);
    ~DataBase();

    bool open();
    void close();

    bool init();

    bool existsTable(QString table);
    bool existsView(QString view);

    // settings table
    bool settingsTable_init();

    // brokers table
    bool brokersTable_init();

    // APIs table
    bool apisTable_init();

    // watchGroups table
    bool watchGroupsTable_init();
    bool watchGroupsTable_addRecord(watchGroup_t &wg);

    // currencies table
    bool currenciesTable_init();
    bool currenciesTable_addRecord(currency_t &c);
    int currenciesTable_getId(QString &name);
    bool currenciesTable_getCurrency(const int currencyId, currency_t &out_currency);

    // securities table
    bool securitiesTable_init();
    bool securitiesTable_addRecord(security_t &s);
    int securitiesTable_getId(QString &isin, int currencyId);
    bool securitiesTable_getSecurity(const int securityId, security_t &out_security);

    // watchLists table
    bool watchListsTable_init();
    bool watchListsTable_addRecord(watchItem_t wi);

    // transactions table
    bool transactionsTable_init();
    bool transactionsTable_addRecord(transaction_t &t);

    // securityPrices
    bool securityPricesTable_init();
    bool securityPricesTable_addRecords(securityPrice_t &sp);
    bool securityPricesTable_startUpdate();
    bool securityPricesTable_get(const int securityId,
                                 const qint64 minTimeStamp,
                                 const qint64 maxTimeStamp,
                                 securityPrice_t &out_sp);

    // currencyPrices
    bool currencyPricesTable_init();
    bool currencyPricesTable_addRecords(currencyPrice_t &cp);
    bool currencyPricesTable_startUpdate();
    bool currencyPricesTable_get(const int currencyId,
                                 const qint64 minTimeStamp,
                                 const qint64 maxTimeStamp,
                                 currencyPrice_t &out_cp);

    // prices table - common functions for all prices
    bool pricesTable_continueUpdate();

    // transactions view
    bool transactionsView_init();

    DataBase::ApiEnum stringToApiEnum(const QString &text);
    QString apiEnumToString(const DataBase::ApiEnum &enumValue);

    DataBase::BrokerEnum stringToBrokerEnum(const QString &text);
    QString brokerEnumToString(const DataBase::BrokerEnum &enumValue);

signals:
    void pricesTable_status(const QString status);

private:
    QString dbFile;
    YahooApi *yahoo;

    typedef struct{
        enum PriceEnum priceType;
        int id;
        enum ApiEnum api;
        QString apiTicker;
        qint64 start_ts;
        qint64 end_ts;
    }priceQuery_t;

    QQueue<priceQuery_t> m_priceQueriesQueue;

private slots:
    bool pricesTable_startUpdate();
    void onReceived_GetDailyPrice(const int id,
                                     const int priceType,
                                     const QString &symbol,
                                     const QString &currency,
                                     const QVector<qint64> &unixTime,
                                     const QVector<double> &open,
                                     const QVector<double> &close,
                                     const QVector<double> &high,
                                     const QVector<double> &low);
};

#endif // DATABASE_H
