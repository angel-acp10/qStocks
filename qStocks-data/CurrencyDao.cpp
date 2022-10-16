#include "CurrencyDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Currency.h"
#include "DatabaseManager.h"

using namespace std;

CurrencyDao::CurrencyDao(QSqlDatabase& db) :
    mDb(db)
{
}

void CurrencyDao::init() const
{
    if (mDb.tables().contains("Currencies"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Currencies ("
        "ID	    INTEGER UNIQUE,"
        "Name	VARCHAR(40) NOT NULL UNIQUE,"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void CurrencyDao::add(Currency& currency) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO Currencies (Name) VALUES (:Name)";
    query.prepare(q);
    query.bindValue(":Name", currency.name());
    query.exec();
    currency.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void CurrencyDao::update(const Currency& currency) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE Currencies SET Name = (:Name) WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":Name",    currency.name());
    query.bindValue(":ID",      currency.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void CurrencyDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM Currencies WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Currency>>> CurrencyDao::currencies() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM Currencies";
    query.exec(q);

    unique_ptr<vector<unique_ptr<Currency>>> list(new vector<unique_ptr<Currency>>());
    while(query.next())
    {
        unique_ptr<Currency> currency(new Currency());

        currency->setId(    query.value("ID").toInt());
        currency->setName(  query.value("Name").toString());

        list->push_back(move(currency));
    }
    return list;
}
