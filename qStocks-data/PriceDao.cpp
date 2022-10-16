#include "PriceDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Price.h"
#include "DatabaseManager.h"

using namespace std;

PriceDao::PriceDao(QSqlDatabase& db) :
    mDb(db)
{
}

void PriceDao::init() const
{
    if (mDb.tables().contains("Prices"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Prices ("
        "ID             INTEGER UNIQUE,"
        "TimeStamp      INTEGER,"
        "ProductID      INTEGER,"
        "Open           DOUBLE,"
        "Close          DOUBLE,"
        "High           DOUBLE,"
        "Low            DOUBLE,"
        "Volume         DOUBLE,"
        "FOREIGN KEY(ProductID) REFERENCES Products(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void PriceDao::add(Price& price) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO Prices ( "
            "TimeStamp, "
            "ProductID, "
            "Open, "
            "Close, "
            "High, "
            "Low, "
            "Volume "
        ") VALUES ("
            ":TimeStamp, "
            ":ProductID, "
            "::Open, "
            ":Close, "
            ":High, "
            ":Low, "
            ":Volume "
        ")";
    query.prepare(q);
    query.bindValue(":TimeStamp",   price.timeStamp());
    query.bindValue(":ProductID",   price.productId());
    query.bindValue(":Open",        price.open());
    query.bindValue(":Close",       price.close());
    query.bindValue(":High",        price.high());
    query.bindValue(":Low",         price.low());
    query.bindValue(":Volume",      price.volume());
    query.exec();
    price.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void PriceDao::update(const Price& price) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE Prices SET "
            "TimeStamp = (:TimeStamp), "
            "ProductID = (:ProductID), "
            "Open = (:Open), "
            "Close = (:Close), "
            "High = (:High), "
            "Low = (:Low), "
            "Volume = (:Volume) "
        "WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":TimeStamp",   price.timeStamp());
    query.bindValue(":ProductID",   price.productId());
    query.bindValue(":Open",        price.open());
    query.bindValue(":Close",       price.close());
    query.bindValue(":High",        price.high());
    query.bindValue(":Low",         price.low());
    query.bindValue(":Volume",      price.volume());
    query.bindValue(":ID",          price.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void PriceDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM Prices WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Price>>> PriceDao::prices() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM Prices";
    query.exec(q);

    unique_ptr<vector<unique_ptr<Price>>> list(new vector<unique_ptr<Price>>());
    while(query.next())
    {
        unique_ptr<Price> price(new Price());

        price->setId(           query.value("ID").toInt());
        price->setTimeStamp(    query.value("TimeStamp").toInt());
        price->setProductId(    query.value("ProductID").toInt());
        price->setOpen(         query.value("Open").toDouble());
        price->setClose(        query.value("Close").toDouble());
        price->setHigh(         query.value("High").toDouble());
        price->setLow(          query.value("Low").toDouble());
        price->setVolume(       query.value("Volume").toDouble());

        list->push_back(move(price));
    }
    return list;
}
