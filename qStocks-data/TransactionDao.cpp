#include "TransactionDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "Transaction.h"
#include "DatabaseManager.h"

using namespace std;

TransactionDao::TransactionDao(QSqlDatabase& db) :
    mDb(db)
{
}

void TransactionDao::init() const
{
    if (mDb.tables().contains("Transactions"))
        return;

    QSqlQuery query(mDb);
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Transactions ("
        "ID	                INTEGER UNIQUE,"
        "TimeStamp	        INTEGER NOT NULL,"
        "ProductID	        INTEGER NOT NULL,"
        "ProductTypeID	    INTEGER NOT NULL,"
        "Quantity	        DOUBLE NOT NULL,"
        "LocalUnitPrice	    DOUBLE NOT NULL,"
        "Value	            DOUBLE NOT NULL,"
        "Comissions	        DOUBLE NOT NULL,"
        "CurrencyID	        INTEGER NOT NULL,"
        "BrokerID	        INTEGER,"
        "FOREIGN KEY(ProductID)     REFERENCES Products(ID),"
        "FOREIGN KEY(ProductTypeID) REFERENCES ProductTypes(ID),"
        "FOREIGN KEY(CurrencyID)    REFERENCES Currencies(ID),"
        "FOREIGN KEY(BrokerID)      REFERENCES Brokers(ID),"
        "UNIQUE(TimeStamp,SecurityID,Quantity),"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    query.exec(q);
    DatabaseManager::debugQuery(query);
}

void TransactionDao::add(Transaction& transaction) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "INSERT INTO Transactions ( "
            "TimeStamp, "
            "ProductID, "
            "ProductTypeID, "
            "Quantity, "
            "LocalUnitPrice, "
            "Value, "
            "Comissions, "
            "CurrencyID, "
            "BrokerID "
        ") VALUES (:TimeStamp, :ProductID, :ProductTypeID, :Quantity, :LocalUnitPrice, "
                    ":Value, :Comissions, :CurrencyID, :BrokerID )";
    query.prepare(q);
    query.bindValue(":TimeStamp",       transaction.timeStamp());
    query.bindValue(":ProductID",       transaction.productId());
    query.bindValue(":ProductTypeID",   transaction.productTypeId());
    query.bindValue(":Quantity",        transaction.quantity());
    query.bindValue(":LocalUnitPrice",  transaction.localUnitPrice());
    query.bindValue(":Value",           transaction.value());
    query.bindValue(":Comissions",      transaction.comissions());
    query.bindValue(":CurrencyID",      transaction.currencyId());
    query.bindValue(":BrokerID",        transaction.brokerId());
    query.exec();
    transaction.setId(query.lastInsertId().toInt());
    DatabaseManager::debugQuery(query);
}

void TransactionDao::update(const Transaction& transaction) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "UPDATE Transactions "
        "SET "
        "TimeStamp = (:TimeStamp), "
        "ProductID = (:ProductID), "
        "ProductTypeID = (:ProductTypeID), "
        "Quantity = (:Quantity), "
        "LocalUnitPrice = (:LocalUnitPrice), "
        "Value = (:Value), "
        "Comissions = (:Comissions), "
        "CurrencyID = (:CurrencyID), "
        "BrokerID = (:BrokerID) "
        "WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":TimeStamp",       transaction.timeStamp());
    query.bindValue(":ProductID",       transaction.productId());
    query.bindValue(":ProductTypeID",   transaction.productTypeId());
    query.bindValue(":Quantity",        transaction.quantity());
    query.bindValue(":LocalUnitPrice",  transaction.localUnitPrice());
    query.bindValue(":Value",           transaction.value());
    query.bindValue(":Comissions",      transaction.comissions());
    query.bindValue(":CurrencyID",      transaction.currencyId());
    query.bindValue(":BrokerID",        transaction.brokerId());
    query.bindValue(":ID",              transaction.id());
    query.exec();
    DatabaseManager::debugQuery(query);
}

void TransactionDao::remove(int id) const
{
    QSqlQuery query(mDb);
    QString q;

    q = "DELETE FROM Transactions WHERE ID = (:ID)";
    query.prepare(q);
    query.bindValue(":ID", id);
    query.exec();
    DatabaseManager::debugQuery(query);
}

unique_ptr<vector<unique_ptr<Transaction>>> TransactionDao::transactions() const
{
    QSqlQuery query(mDb);
    QString q;

    q = "SELECT * FROM Transactions";
    query.exec(q);

    unique_ptr<vector<unique_ptr<Transaction>>> list(new vector<unique_ptr<Transaction>>());
    while(query.next())
    {
        unique_ptr<Transaction> transaction(new Transaction());

        transaction->setId(             query.value("ID").toInt());
        transaction->setTimeStamp(      query.value("TimeStamp").toInt());
        transaction->setProductId(      query.value("ProductID").toInt());
        transaction->setProductTypeId(  query.value("ProductTypeID").toInt());
        transaction->setQuantity(       query.value("Quantity").toDouble());
        transaction->setLocalUnitPrice( query.value("LocalUnitPrice").toDouble());
        transaction->setValue(          query.value("Value").toDouble());
        transaction->setComissions(     query.value("Comissions").toDouble());
        transaction->setCurrencyId(     query.value("CurrencyID").toInt());
        transaction->setBrokerId(       query.value("BrokerID").toInt());

        list->push_back(move(transaction));
    }
    return list;
}

