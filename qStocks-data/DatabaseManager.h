#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <memory>

#include <QString>

#include "ApiDao.h"
#include "BrokerDao.h"
#include "CurrencyDao.h"
#include "DividendDao.h"
#include "PriceDao.h"
#include "ProductDao.h"
#include "ProductTypeDao.h"
#include "SplitDao.h"
#include "TransactionDao.h"
#include "WatchItemDao.h"
#include "WatchListDao.h"

class QSqlQuery;
class QSqlDatabase;

const QString DATABASE_FILENAME = "qStocks_db.db";

class DatabaseManager
{
public:
    static void debugQuery(const QSqlQuery& query);

    static DatabaseManager& instance();
    ~DatabaseManager();

protected:
    DatabaseManager(const QString& path = DATABASE_FILENAME);
    DatabaseManager& operator=(const DatabaseManager& rhs);

private:
    std::unique_ptr<QSqlDatabase> mDatabase;

public:
    const ApiDao apiDao;
    const BrokerDao brokerDao;
    const CurrencyDao currencyDao;
    const DividendDao dividendDao;
    const PriceDao priceDao;
    const ProductDao productDao;
    const ProductTypeDao productTypeDao;
    const SplitDao splitDao;
    const TransactionDao transactionDao;
    const WatchItemDao watchItemDao;
    const WatchListDao watchListDao;

};

#endif // DATABASEMANAGER_H
