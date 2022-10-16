#include "BrokerDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Broker.h"
#include "DatabaseManager.h"

using namespace std;

BrokerDao::BrokerDao(QSqlDatabase& db) :
    mDb(db)
{
}

void BrokerDao::init() const
{
    if (mDb.tables().contains("Brokers"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Brokers ("
        "ID	        INTEGER UNIQUE,"
        "Name	    VARCHAR(40) NOT NULL UNIQUE,"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void BrokerDao::add(Broker& broker) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO Brokers (Name) VALUES (:Name)";
    query.prepare(q);
    query.bindValue(":Name", broker.name());
    query.exec();
    broker.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void BrokerDao::update(const Broker& broker) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE Brokers SET Name = (:Name) WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":Name",    broker.name());
    query.bindValue(":ID",      broker.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void BrokerDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM Brokers WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Broker>>> BrokerDao::brokers() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM Brokers";
    query.exec(q);

    unique_ptr<vector<unique_ptr<Broker>>> list(new vector<unique_ptr<Broker>>());
    while(query.next())
    {
        unique_ptr<Broker> broker(new Broker());

        broker->setId(  query.value("ID").toInt());
        broker->setName(query.value("Name").toString());

        list->push_back(move(broker));
    }
    return list;
}
