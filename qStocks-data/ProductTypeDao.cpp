#include "ProductTypeDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "ProductType.h"
#include "DatabaseManager.h"

using namespace std;

ProductTypeDao::ProductTypeDao(QSqlDatabase& db) :
    mDb(db)
{
}

void ProductTypeDao::init() const
{
    if (mDb.tables().contains("ProductTypes"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS ProductTypes ("
        "ID	    INTEGER UNIQUE,"
        "Name	VARCHAR(40) NOT NULL UNIQUE,"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void ProductTypeDao::add(ProductType& productType) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO ProductTypes (Name) VALUES (:Name)";
    query.prepare(q);
    query.bindValue(":Name", productType.name());
    query.exec();
    productType.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void ProductTypeDao::update(const ProductType& productType) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE ProductTypes SET Name = (:Name) WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":Name",    productType.name());
    query.bindValue(":ID",      productType.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void ProductTypeDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM ProductTypes WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<ProductType>>> ProductTypeDao::productTypes() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM ProductTypes";
    query.exec(q);

    unique_ptr<vector<unique_ptr<ProductType>>> list(new vector<unique_ptr<ProductType>>());
    while(query.next())
    {
        unique_ptr<ProductType> productType(new ProductType());

        productType->setId(     query.value("ID").toInt());
        productType->setName(   query.value("Name").toString());

        list->push_back(move(productType));
    }
    return list;
}
