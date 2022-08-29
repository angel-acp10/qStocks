#include "database.h"

#include <QtSql>

DataBase::DataBase(const QString & dbFile)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFile);
}

DataBase::~DataBase()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(db.isOpen())
        db.close();
    QSqlDatabase::removeDatabase(db.connectionName());
}

bool DataBase::init(void)
{
    if( !open() )
        return false;

    if(!currenciesTable_createIfNotExists())    return false;
    if(!securitiesTable_createIfNotExists())    return false;
    if(!settingsTable_createIfNotExists())      return false;
    if(!transactionsTable_createIfNotExists())  return false;
    if(!transactionsView_createIfNotExists())   return false;

    return true;
}

bool DataBase::open(void)
{
   QSqlDatabase db = QSqlDatabase::database();
   if( !db.isOpen() )
   {
       if(!db.open() )
       {
           qDebug()<<"DB error: unable to open";
           return false;
       }
   }
   return true;
}

void DataBase::close(void)
{
    QSqlDatabase db = QSqlDatabase::database();
    if( db.isOpen() )
        db.close();
}

bool DataBase::settingsTable_createIfNotExists()
{
    setting_t defSettings[] = {
        {
            .name = "DefCurrency",
            .canBeNull = false,
            .value = (int)USD,
            .minValue = 0,
            .maxValue = 0,
            .description = "Default currency to be shown on graphs and tables, except on Transactions."
        },
        {
            .name = "PrevMonths",
            .canBeNull = false,
            .value = 3,
            .minValue = 0,
            .maxValue = 12*3,
            .description = "Months of equity price prior to first purchase."
        },
        { .name = "", .canBeNull = false, .value = 0, .minValue = 0, .maxValue = 0,.description = "" }, // end of table indicator
    };

    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS Settings ("
        "Name	VARCHAR(20) NOT NULL UNIQUE,"
        "CanBeNull	INTEGER NOT NULL,"
        "Value	INTEGER,"
        "MinValue	INTEGER,"
        "MaxValue	INTEGER,"
        "Description	VARCHAR(100),"
        "PRIMARY KEY(Name)"
    ");";

     if(!qry.exec(q))
     {
         qDebug()<<"DB error: creating 'Settings' table";
         return false;
     }

     q = "INSERT or REPLACE INTO Settings ("
         "Name,"
         "CanBeNull,"
         "Value,"
         "MinValue,"
         "MaxValue,"
         "Description)"
     "VALUES (?, ?, ?, ?, ?, ?);";

     for(int i=0; defSettings[i].name != ""; i++)
     {
         qry.prepare(q);
         qry.addBindValue( defSettings[i].name ); // name
         if(defSettings[i].canBeNull)   qry.addBindValue( 1 ); // canBeNull
         else                           qry.addBindValue( 0 ); // canBeNull
         qry.addBindValue( defSettings[i].value ); // value
         qry.addBindValue( defSettings[i].minValue ); // minValue
         qry.addBindValue( defSettings[i].maxValue ); // maxValue
         qry.addBindValue( defSettings[i].description ); // description
         if(!qry.exec())
         {
             qDebug()<<"DB error: filling 'Settings' table";
             return false;
         }
     }

     return true;
}

bool DataBase::currenciesTable_createIfNotExists()
{
    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS Currencies ("
         "ID            INTEGER UNIQUE,"
         "Name       	VARCHAR(10) UNIQUE,"
         "PRIMARY KEY(ID AUTOINCREMENT)"
     ");";
     if(!qry.exec(q))
     {
         qDebug()<<"DB error: creating 'Currencies' table";
         return false;
     }

     q = "INSERT or REPLACE INTO Currencies ("
         "ID,"
         "Name)"
     "VALUES (?, ?);";
     for(int i=(int)USD; i<=max_currencyType; i++)
     {
         qry.prepare(q);
         qry.addBindValue(i); // ID
         qry.addBindValue( getCurrencyString((CurrencyType)i) ); // Name, eg. "USD"
         if(!qry.exec())
         {
             qDebug()<<"DB error: filling 'Currencies' table";
             return false;
         }
     }
     return true;
}

bool DataBase::securitiesTable_createIfNotExists()
{
    QSqlQuery qry;
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Securities ("
        "ID         VARCHAR(15) NOT NULL UNIQUE,"
        "Name   	VARCHAR(60) NOT NULL,"
        "Ticker     VARCHAR(10),"
        "PRIMARY KEY(ID)"
    ");";
    if(!qry.exec(q))
    {
        qDebug()<<"DB error: creating 'Securities' table";
        return false;
    }
    return true;
}

bool DataBase::securitiesTable_addSecurity(security_t &s)
{
    QSqlQuery qry;
    QString q;
    q = "INSERT or REPLACE INTO Securities ("
        "ID,"
        "Name,"
        "Ticker)"
    "VALUES (?, ?, ?);";

    qry.prepare(q);
    qry.addBindValue(s.id);
    qry.addBindValue(s.name);
    qry.addBindValue(s.ticker);

    if(!qry.exec())
    {
        qDebug()<<QString("DB error: unable to add '%1'").arg(s.name);
        return false;
    }
    return true;
}


bool DataBase::transactionsTable_createIfNotExists()
{
    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS Transactions ("
        "ID                 INTEGER,"
        "DateTime           DATETIME NOT NULL,"
        "SecurityID         VARCHAR(15) NOT NULL,"
        "Quantity           INTEGER NOT NULL,"
        "LocalUnitPrice     DOUBLE NOT NULL,"
        "LocalCurrencyID	INTEGER NOT NULL,"
        "ExchangeRate       DOUBLE NOT NULL,"
        "Value              DOUBLE NOT NULL,"
        "Comissions         DOUBLE NOT NULL,"
        "TotalValue         DOUBLE NOT NULL,"
        "CurrencyID         INTEGER NOT NULL,"
        "FOREIGN KEY(CurrencyID) REFERENCES Currencies(ID),"
        "FOREIGN KEY(SecurityID) REFERENCES Securities(ID),"
        "FOREIGN KEY(LocalCurrencyID) REFERENCES Currencies(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT),"
        "UNIQUE(DateTime, SecurityID, Quantity)"
    ");";
    if(!qry.exec(q))
    {
        qDebug()<<"DB error: creating 'Transactions' table";
        return false;
    }
    return true;
}

bool DataBase::transactionsTable_addTransaction(transaction_t &t)
{
    QSqlQuery qry;
    QString q;

    securitiesTable_addSecurity(t.security);

    q = "INSERT or REPLACE INTO Transactions ("
                    "DateTime,"
                    "SecurityID,"
                    "Quantity,"
                    "LocalUnitPrice,"
                    "LocalCurrencyID,"
                    "ExchangeRate,"
                    "Value,"
                    "Comissions,"
                    "TotalValue,"
                    "CurrencyID)"
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    qry.prepare(q);

    qry.addBindValue(t.dateTime);
    qry.addBindValue(t.security.id);
    qry.addBindValue(t.qty);
    qry.addBindValue(t.locUnitPrice);
    qry.addBindValue((int)t.locCurrency);
    qry.addBindValue(t.exchRate);
    qry.addBindValue(t.value);
    qry.addBindValue(t.commissions);
    qry.addBindValue(t.total);
    qry.addBindValue((int)t.currency);

    if(!qry.exec())
    {
        qDebug()<<"error adding transaction to db";
        return false;
    }
    return true;
}

bool DataBase::transactionsView_createIfNotExists()
{
    QSqlQuery qry;
    QString q;

    q = "DROP VIEW IF EXISTS VW_TRANSACTIONS;";
    if(!qry.exec(q))
    {
        qDebug()<<"DB error: removing previous 'VW_TRANSACTIONS' view";
        return false;
    }

    q = "CREATE VIEW VW_TRANSACTIONS AS "
    "SELECT Transactions.DateTime AS 'Date'"
          ",(SELECT Securities.Name FROM Securities WHERE Securities.ID=Transactions.SecurityID) AS 'Name'"
          ",(SELECT Securities.Ticker   FROM Securities WHERE Securities.ID=Transactions.SecurityID) AS 'Ticker'"
          ",Transactions.SecurityID AS 'Isin'"
          ",Transactions.Quantity AS 'Quantity'"
          ",ROUND(Transactions.LocalUnitPrice, 2) ||' '|| (SELECT Currencies.Name FROM Currencies WHERE Currencies.ID=Transactions.LocalCurrencyID) AS 'Unit. Price'"
          ",ROUND(Transactions.ExchangeRate, 4) AS 'Exch. Rate'"
          ",ROUND(Transactions.TotalValue, 2) ||' '|| (SELECT Currencies.Name FROM Currencies WHERE Currencies.ID=Transactions.CurrencyID) AS 'Value' "
    "FROM Transactions";
    if(!qry.exec(q))
    {
        //qDebug()<<qry.lastError();
        qDebug()<<"DB error: creating 'VW_TRANSACTIONS' view";
        return false;
    }
    return true;
}


DataBase::CurrencyType DataBase::getCurrencyEnum(const QString &text)
{
    return QVariant(text).value<CurrencyType>();
}

QString DataBase::getCurrencyString(const DataBase::CurrencyType &enumValue)
{
    return QVariant::fromValue(enumValue).toString();
}

