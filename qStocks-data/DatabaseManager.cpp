#include "DatabaseManager.h"

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

void DatabaseManager::debugQuery(const QSqlQuery& query)
{
    if (query.lastError().type() == QSqlError::ErrorType::NoError) {
        qDebug() << "Query OK:"  << query.lastQuery();
    } else {
       qWarning() << "Query KO:" << query.lastError().text();
       qWarning() << "Query text:" << query.lastQuery();
    }
}

DatabaseManager&DatabaseManager::instance()
{
    static DatabaseManager singleton;
    return singleton;
}

DatabaseManager::DatabaseManager(const QString& path) :
    mDatabase(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"))),
    apiDao(*mDatabase),
    brokerDao(*mDatabase),
    currencyDao(*mDatabase),
    dividendDao(*mDatabase),
    priceDao(*mDatabase),
    productDao(*mDatabase),
    productTypeDao(*mDatabase),
    splitDao(*mDatabase),
    transactionDao(*mDatabase),
    watchItemDao(*mDatabase),
    watchListDao(*mDatabase)
{
    mDatabase->setDatabaseName(path);

    bool openStatus = mDatabase->open();
    qDebug() << "Database connection: " << (openStatus ? "OK" : "Error");

    apiDao.init();
    brokerDao.init();
    currencyDao.init();
    dividendDao.init();
    priceDao.init();
    productDao.init();
    productTypeDao.init();
    splitDao.init();
    transactionDao.init();
    watchItemDao.init();
    watchListDao.init();
}

DatabaseManager::~DatabaseManager()
{
    mDatabase->close();
}

