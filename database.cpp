#include "database.h"

#include <QtSql>

DataBase::DataBase(const QString & dbFile)
{
    //this->dbFile = dbFile;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFile);
}

DataBase::~DataBase()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(db.isOpen())
        db.close();
    QSqlDatabase::removeDatabase(db.connectionName());
}

bool DataBase::existsTransactionsTable(void)
{
    QSqlDatabase db = QSqlDatabase::database();
    if( !db.open() )
        qDebug()<<"error opening db";

    bool res = db.tables().contains( QLatin1String("transactions"));
    db.close();
    if(res)
        return true;

    return false;
}

bool DataBase::createTransactionsTable(void)
{
    QSqlDatabase db = QSqlDatabase::database();
    if( !db.open() )
        qDebug()<<"error opening db";

    QString query = "CREATE TABLE IF NOT EXISTS transactions ("
            "DateTime DATETIME,"
            "Product VARCHAR(100),"
            "Ticker VARCHAR(10),"
            "Isin VARCHAR(20),"
            "Market VARCHAR(10),"
            "Quantity integer,"
            "LocalUnitPrice double,"
            "LocalValue double,"
            "LocalCurrency VARCHAR(5),"
            "Currency VARCHAR(5),"
            "ExchangeRate double,"
            "Value double,"
            "Comissions double,"
            "Total double,"
            "UNIQUE(DateTime, Product, Isin, Market, Quantity));";

    QSqlQuery qry;
    bool res = qry.exec(query);
    db.close();

    if(res)
        return true;

    qDebug()<<"error creating table";
    return false;
}

bool DataBase::addTransaction(transaction_t &t)
{
    QSqlDatabase db = QSqlDatabase::database();
    if( !db.open() )
        qDebug()<<"error opening db";

    QSqlQuery qry;

    t.locValue = -t.qty * t.locUnitPrice;
    if(t.exchRate == 0.0)
        t.exchRate = 1;
    t.value = t.locValue / t.exchRate;
    t.total = t.value + t.commissions;

    qry.prepare("INSERT or REPLACE INTO transactions ("
                "DateTime,"
                "Product,"
                "Ticker,"
                "Isin,"
                "Market,"
                "Quantity,"
                "LocalUnitPrice,"
                "LocalValue,"
                "LocalCurrency,"
                "Currency,"
                "ExchangeRate,"
                "Value,"
                "Comissions,"
                "Total)"
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");


    qry.addBindValue(t.dateTime);
    qry.addBindValue(t.product);
    qry.addBindValue(t.ticker);
    qry.addBindValue(t.isin);
    qry.addBindValue(t.market);
    qry.addBindValue(t.qty);
    qry.addBindValue(t.locUnitPrice);
    qry.addBindValue(t.locValue);
    qry.addBindValue(t.locCurrency);
    qry.addBindValue(t.currency);
    qry.addBindValue(t.exchRate);
    qry.addBindValue(t.value);
    qry.addBindValue(t.commissions);
    qry.addBindValue(t.total);

    bool res = qry.exec();
    db.close();

    if(res)
        return true;

    qDebug()<<"error adding transaction to db";


    return false;
}
