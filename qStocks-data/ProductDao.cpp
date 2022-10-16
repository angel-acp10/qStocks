#include "ProductDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Product.h"
#include "DatabaseManager.h"

using namespace std;

ProductDao::ProductDao(QSqlDatabase& db) :
    mDb(db)
{
}

void ProductDao::init() const
{
    if (mDb.tables().contains("Products"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Products ("
        "ID             INTEGER     UNIQUE,"
        "ProductTypeID  INTEGER     NOT NULL"
        "Isin           VARCHAR(15) NOT NULL,"
        "Name           VARCHAR(60),"
        "Ticker         VARCHAR(15),"
        "CurrencyID     INTEGER     NOT NULL,"
        "AuxCurrencyID	INTEGER,"
        "Notes          TEXT,"
        "ApiID          INTEGER,"
        "ApiTicker      VARCHAR(15),"
        "FOREIGN KEY(ProductTypeID)     REFERENCES ProductTypes(ID),"
        "FOREIGN KEY(CurrencyID)        REFERENCES Currencies(ID),"
        "FOREIGN KEY(AuxCurrencyID)     REFERENCES Currencies(ID),"
        "FOREIGN KEY(ApiID)             REFERENCES Apis(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT),"
        "UNIQUE(Isin,CurrencyID)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void ProductDao::add(Product& product) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO Products ("
            "ProductTypeID, "
            "Isin, "
            "Name, "
            "Ticker, "
            "CurrencyID, "
            "AuxCurrencyID, "
            "Notes, "
            "ApiID, "
            "ApiTicker "
        ") VALUES ("
            ":ProductTypeID, "
            ":Isin, "
            ":Name, "
            ":Ticker, "
            ":CurrencyID, "
            ":AuxCurrencyID, "
            ":Notes, "
            ":ApiID, "
            ":ApiTicker, "
        ")";
    query.prepare(q);
    query.bindValue(":ProductTypeID",   product.typeId());
    query.bindValue(":Isin",            product.isin());
    query.bindValue(":Name",            product.name());
    query.bindValue(":Ticker",          product.ticker());
    query.bindValue(":CurrencyID",      product.currencyId());
    query.bindValue(":AuxCurrencyID",   product.auxCurrencyId());
    query.bindValue(":Notes",           product.notes());
    query.bindValue(":ApiID",           product.apiId());
    query.bindValue(":ApiTicker",       product.apiTicker());

    query.exec();
    product.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void ProductDao::update(const Product& product) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE Products SET "
        "ProductTypeID = (:ProductTypeID), "
        "Isin = (:Isin), "
        "Name = (:Name), "
        "Ticker = (:Ticker), "
        "CurrencyID = (:CurrencyID), "
        "AuxCurrencyID = (:AuxCurrencyID), "
        "Notes = (:Notes), "
        "ApiID = (:ApiID), "
        "ApiTicker = (:ApiTicker) "
        "WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ProductTypeID",   product.typeId());
    query.bindValue(":Isin",            product.isin());
    query.bindValue(":Name",            product.name());
    query.bindValue(":Ticker",          product.ticker());
    query.bindValue(":CurrencyID",      product.currencyId());
    query.bindValue(":AuxCurrencyID",   product.auxCurrencyId());
    query.bindValue(":Notes",           product.notes());
    query.bindValue(":ApiID",           product.apiId());
    query.bindValue(":ApiTicker",       product.apiTicker());
    query.bindValue(":ID",              product.id());

    query.exec();
    DatabaseManager::debugQuery(query);
}

void ProductDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM Products WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Product>>> ProductDao::products() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM Products";
    query.exec(q);

    unique_ptr<vector<unique_ptr<Product>>> list(new vector<unique_ptr<Product>>());
    while(query.next())
    {
        unique_ptr<Product> product(new Product());

        product->setId(             query.value("ID").toInt());
        product->setTypeId(         query.value("ProductTypeID").toInt());
        product->setIsin(           query.value("Isin").toString());
        product->setName(           query.value("Name").toString());
        product->setTicker(         query.value("Ticker").toString());
        product->setCurrencyId(     query.value("CurrencyID").toInt());
        product->setAuxCurrencyId(  query.value("AuxCurrencyID").toInt());
        product->setNotes(          query.value("Notes").toString());
        product->setApiId(          query.value("ApiID").toInt());
        product->setApiTicker(      query.value("ApiTicker").toString());

        list->push_back(move(product));
    }
    return list;
}
