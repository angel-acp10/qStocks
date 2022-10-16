#include "ApiDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Api.h"
#include "DatabaseManager.h"

using namespace std;

ApiDao::ApiDao(QSqlDatabase& db) :
    mDb(db)
{
}

void ApiDao::init() const
{
    if (mDb.tables().contains("Apis"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Apis ("
        "ID	    INTEGER UNIQUE,"
        "Name	VARCHAR(40) NOT NULL UNIQUE,"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void ApiDao::add(Api& api) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO Apis (Name) VALUES (:Name)";
    query.prepare(q);
    query.bindValue(":Name", api.name());
    query.exec();
    api.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void ApiDao::update(const Api& api) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE Apis SET Name = (:Name) WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":Name",    api.name());
    query.bindValue(":ID",      api.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void ApiDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM Apis WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Api>>> ApiDao::apis() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM Apis";
    query.exec(q);

    unique_ptr<vector<unique_ptr<Api>>> list(new vector<unique_ptr<Api>>());
    while(query.next())
    {
        unique_ptr<Api> api(new Api());

        api->setId(     query.value("ID").toInt());
        api->setName(   query.value("Name").toString());

        list->push_back(move(api));
    }
    return list;
}
