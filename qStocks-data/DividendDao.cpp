#include "DividendDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Dividend.h"
#include "DatabaseManager.h"

using namespace std;

DividendDao::DividendDao(QSqlDatabase& db) :
    mDb(db)
{
}

void DividendDao::init() const
{
    if (mDb.tables().contains("Dividends"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Dividends ("
        "ID             INTEGER UNIQUE,"
        "TimeStamp      INTEGER,"
        "ProductID      INTEGER,"
        "Amount         DOUBLE,"
        "FOREIGN KEY(ProductID) REFERENCES Products(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void DividendDao::add(Dividend& dividend) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO Dividends ("
            "TimeStamp,"
            "ProductID,"
            "Amount,"
        ") VALUES ("
            ":TimeStamp,"
            ":ProductID,"
            ":Amount,"
        ")";
    query.prepare(q);
    query.bindValue(":TimeStamp",   dividend.timeStamp());
    query.bindValue(":ProductID",   dividend.productId());
    query.bindValue(":Amount",      dividend.amount());
    query.exec();
    dividend.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void DividendDao::update(const Dividend& dividend) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE Dividends SET "
            "TimeStamp = (:TimeStamp), "
            "ProductID = (:ProductID), "
            "Numerator = (:Numerator), "
            "Denominator = (:Denominator) "
            "WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":TimeStamp",   dividend.timeStamp());
    query.bindValue(":ProductID",   dividend.productId());
    query.bindValue(":Amount",      dividend.amount());
    query.bindValue(":ID",          dividend.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void DividendDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM Dividends WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Dividend>>> DividendDao::dividends() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM Dividends";
    query.exec(q);

    unique_ptr<vector<unique_ptr<Dividend>>> list(new vector<unique_ptr<Dividend>>());
    while(query.next())
    {
        unique_ptr<Dividend> dividend(new Dividend());

        dividend->setId(            query.value("ID").toInt());
        dividend->setTimeStamp(     query.value("TimeStamp").toInt());
        dividend->setProductId(     query.value("ProductID").toInt());
        dividend->setAmount(        query.value("Amount").toDouble());

        list->push_back(move(dividend));
    }
    return list;
}
