#include "WatchListDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "WatchList.h"
#include "DatabaseManager.h"

using namespace std;

WatchListDao::WatchListDao(QSqlDatabase& db) :
    mDb(db)
{
}

void WatchListDao::init() const
{
    if (mDb.tables().contains("WatchLists"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS WatchLists ("
        "ID             INTEGER UNIQUE,"
        "Name           VARCHAR(50) NOT NULL UNIQUE,"
        "Color          VARCHAR(50),"
        "Description    TEXT,"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void WatchListDao::add(WatchList& watchList) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO WatchLists (Name, Color, Description) "
        "VALUES (:Name, :Color, :Description)";
    query.prepare(q);
    query.bindValue(":Name",        watchList.name());
    query.bindValue(":Color",       watchList.color());
    query.bindValue(":Description", watchList.description());
    query.exec();
    watchList.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void WatchListDao::update(const WatchList& watchList) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE WatchLists "
        "SET Name = (:Name), "
        "Color = (:Color), "
        "Description = (:Description) "
        "WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":Name",        watchList.name());
    query.bindValue(":Color",       watchList.color());
    query.bindValue(":Description", watchList.description());
    query.bindValue(":ID",          watchList.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void WatchListDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM WatchLists WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<WatchList>>> WatchListDao::watchLists() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM WatchLists";
    query.exec(q);

    unique_ptr<vector<unique_ptr<WatchList>>> list(new vector<unique_ptr<WatchList>>());
    while(query.next())
    {
        unique_ptr<WatchList> watchList(new WatchList());

        watchList->setId(           query.value("ID").toInt());
        watchList->setName(         query.value("Name").toString());
        watchList->setColor(        query.value("Color").toString());
        watchList->setDescription(  query.value("Description").toString());

        list->push_back(move(watchList));
    }
    return list;
}
