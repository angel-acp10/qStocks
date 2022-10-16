#include "WatchItemDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "WatchItem.h"
#include "DatabaseManager.h"

using namespace std;

WatchItemDao::WatchItemDao(QSqlDatabase& db) :
    mDb(db)
{
}

void WatchItemDao::init() const
{
    if (mDb.tables().contains("WatchItems"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS WatchItems ("
        "ID             INTEGER UNIQUE,"
        "ProductID      INTEGER,"
        "WatchListID	INTEGER,"
        "FOREIGN KEY(ProductID)     REFERENCES Products(ID),"
        "FOREIGN KEY(WatchListID)   REFERENCES WatchLists(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void WatchItemDao::add(WatchItem& watchItem) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO WatchItems (ProductID, WatchListID) VALUES (:ProductID, :WatchListID)";
    query.prepare(q);
    query.bindValue(":ProductID",   watchItem.productId());
    query.bindValue(":WatchListID", watchItem.watchListId());
    query.exec();
    watchItem.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void WatchItemDao::update(const WatchItem& watchItem) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE WatchItems SET "
            "ProductID = (:ProductID), "
            "WatchListID = (:WatchListID) "
        "WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ProductID",   watchItem.productId());
    query.bindValue(":WatchListID", watchItem.watchListId());
    query.bindValue(":ID",          watchItem.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void WatchItemDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM WatchItems WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<WatchItem>>> WatchItemDao::watchItems() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM WatchItems";
    query.exec(q);

    unique_ptr<vector<unique_ptr<WatchItem>>> list(new vector<unique_ptr<WatchItem>>());
    while(query.next())
    {
        unique_ptr<WatchItem> watchItem(new WatchItem());

        watchItem->setId(           query.value("ID").toInt());
        watchItem->setProductId(    query.value("ProductID").toInt());
        watchItem->setWatchListId(  query.value("WatchListID").toInt());
        list->push_back(move(watchItem));
    }
    return list;
}

