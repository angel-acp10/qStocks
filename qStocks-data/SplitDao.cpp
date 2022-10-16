#include "SplitDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Split.h"
#include "DatabaseManager.h"

using namespace std;

SplitDao::SplitDao(QSqlDatabase& db) :
    mDb(db)
{
}

void SplitDao::init() const
{
    if (mDb.tables().contains("Splits"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Splits ("
        "ID             INTEGER UNIQUE,"
        "TimeStamp      INTEGER,"
        "ProductID      INTEGER,"
        "Numerator      INTEGER,"
        "Denominator    INTEGER,"
        "FOREIGN KEY(ProductID) REFERENCES Products(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void SplitDao::add(Split& split) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO Splits ("
            "TimeStamp,"
            "ProductID,"
            "Numerator,"
            "Denominator"
        ") VALUES ("
            ":TimeStamp,"
            ":ProductID,"
            ":Numerator,"
            ":Denominator"
        ")";
    query.prepare(q);
    query.bindValue(":TimeStamp",   split.timeStamp());
    query.bindValue(":ProductID",   split.productId());
    query.bindValue(":Numerator",   split.numerator());
    query.bindValue(":Denominator", split.denominator());
    query.exec();
    split.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void SplitDao::update(const Split& split) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE Splits SET "
            "TimeStamp = (:TimeStamp), "
            "ProductID = (:ProductID), "
            "Numerator = (:Numerator), "
            "Denominator = (:Denominator) "
            "WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":TimeStamp",   split.timeStamp());
    query.bindValue(":ProductID",   split.productId());
    query.bindValue(":Numerator",   split.numerator());
    query.bindValue(":Denominator", split.denominator());
    query.bindValue(":ID", split.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void SplitDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM Splits WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Split>>> SplitDao::splits() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM Splits";
    query.exec(q);

    unique_ptr<vector<unique_ptr<Split>>> list(new vector<unique_ptr<Split>>());
    while(query.next())
    {
        unique_ptr<Split> split(new Split());

        split->setId(           query.value("ID").toInt());
        split->setTimeStamp(    query.value("TimeStamp").toInt());
        split->setProductId(    query.value("ProductID").toInt());
        split->setNumerator(    query.value("Numerator").toInt());
        split->setDenominator(  query.value("Denominator").toInt());

        list->push_back(move(split));
    }
    return list;
}
