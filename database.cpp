#include "database.h"

#include <QtSql>

#define SHOW_ERROR(query) {qDebug()<<"File: "<<__FILE__<<" Line: "<<__LINE__<<query.lastError();}

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

    if(!settingsTable_init())      return false;
    if(!brokersTable_init())       return false;
    if(!apisTable_init())          return false;
    if(!watchGroupsTable_init())   return false;
    if(!currenciesTable_init())    return false;
    if(!securitiesTable_init())    return false;
    if(!watchListsTable_init())    return false;
    if(!transactionsTable_init())  return false;
    if(!securityPricesTable_init()) return false;
    if(!currencyPricesTable_init()) return false;

    if(!transactionsView_init())   return false;

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

bool DataBase::existsTable(QString table)
{
    QString q;
    QSqlQuery qry;
    bool exists = false;

    q = QString("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1';").arg(table);
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    while(qry.next())
    {
        exists = (bool)qry.value(0).toInt();
        if(exists)  
            return true;
    }
    qDebug()<<QString("DB: table '%1' does not exists").arg(table);
    return false;
}

bool DataBase::existsView(QString view)
{
    QString q;
    QSqlQuery qry;
    bool exists;

    q = QString("SELECT COUNT(*) FROM sqlite_master WHERE type='view' AND name='%1';").arg(view);

    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }   
    while(qry.next())
    {
        exists = (bool)qry.value(0).toInt();
        if(exists)  
            return true;
    }

    qDebug()<<QString("DB: view '%1' does not exists").arg(view);
    return false;
}

// settings table
bool DataBase::settingsTable_init()
{
    if(existsTable("Settings"))
        return true;

    setting_t defSettings[] = {
        {
            .name = "DefCurrency",
            .canBeNull = false,
            .value = -1, // not set
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
        SHOW_ERROR(qry);
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
            SHOW_ERROR(qry);
            return false;
        }
    }

    return true;
}

// brokers table
bool DataBase::brokersTable_init()
{
    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS Brokers ("
        "ID	INTEGER UNIQUE,"
        "Name	VARCHAR(40) NOT NULL UNIQUE,"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }

    q = "INSERT or REPLACE INTO Brokers (ID, Name) VALUES (?, ?);";
    for(int i=(int)BROKER_DEGIRO; i<=totalBrokers; i++)
    {
        qry.prepare(q);
        qry.addBindValue(i); // ID
        qry.addBindValue( brokerEnumToString((BrokerEnum)i) ); // name
        if(!qry.exec())
        {
            SHOW_ERROR(qry);
            return false;
        }
    }
    return true;
}

// APIs table
bool DataBase::apisTable_init()
{
    QStringList defApis;

    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS Apis ("
        "ID	INTEGER UNIQUE,"
        "Name	VARCHAR(40) NOT NULL UNIQUE,"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }

    q = "INSERT INTO Apis (ID, Name) VALUES (?, ?);";
    for(int i=(int)API_YAHOO; i<=totalApis; i++)
    {
        qry.prepare(q);
        qry.addBindValue(i); // ID
        qry.addBindValue( apiEnumToString((ApiEnum)i) );
        if(!qry.exec())
        {
            SHOW_ERROR(qry);
            return false;
        }
    }
    return true;
}

// watchGroups table
bool DataBase::watchGroupsTable_init()
{
    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS WatchGroups ("
        "ID	INTEGER UNIQUE,"
        "Name	VARCHAR(50),"
        "Color	VARCHAR(10),"
        "Description	TEXT,"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";

    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

bool DataBase::watchGroupsTable_addRecord(watchGroup_t &wg)
{
    QString q;
    QSqlQuery qry;

    q = "INSERT INTO WatchGroups (Name, Color, Description) VALUES (?, ?, ?);";

    qry.prepare(q);
    qry.addBindValue(wg.name); // name
    qry.addBindValue(wg.color.name());
    qry.addBindValue(wg.description);
    if(!qry.exec())
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}


// currencies table
bool DataBase::currenciesTable_init()
{
    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS Currencies ("
        "ID	INTEGER UNIQUE,"
        "Name	VARCHAR(10) UNIQUE,"
        "ApiID	INTEGER,"
        "ApiTicker	VARCHAR(15),"
        "FOREIGN KEY(ApiID) REFERENCES Apis(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

bool DataBase::currenciesTable_addRecord(currency_t &c)
{
    QString q;
    QSqlQuery qry;

    q = "INSERT INTO Currencies (Name, ApiID, ApiTicker) VALUES (?, ?, ?);";

    qry.prepare(q);
    qry.addBindValue(c.name); // name
    qry.addBindValue(c.apiId);
    qry.addBindValue(c.apiTicker);
    if(!qry.exec())
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

int DataBase::currenciesTable_getId(QString &name)
{
    int currencyId;
    QString q;
    QSqlQuery qry;

    q = QString("SELECT ID FROM Currencies WHERE Name = '%1';").arg(name.toUpper());   
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    qry.next();
    currencyId = qry.value(0).toInt();
    return currencyId;
}

// securities table
bool DataBase::securitiesTable_init()
{
    QSqlQuery qry;
    QString q;

    q = "CREATE TABLE IF NOT EXISTS Securities ("
        "ID	INTEGER UNIQUE,"
        "Isin	VARCHAR(15) NOT NULL,"
        "Name	VARCHAR(60),"
        "Ticker	VARCHAR(15),"
        "CurrencyID	INTEGER NOT NULL,"
        "Notes	INTEGER,"
        "ApiID	INTEGER,"
        "ApiTicker	VARCHAR(15),"
        "FOREIGN KEY(CurrencyID) REFERENCES Currencies(ID),"
        "FOREIGN KEY(ApiID) REFERENCES Apis(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT),"
        "UNIQUE(Isin,CurrencyID)"
    ");";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

bool DataBase::securitiesTable_addRecord(security_t &s)
{
    currenciesTable_addRecord(s.currency);
    s.currency.id = currenciesTable_getId(s.currency.name);

    QSqlQuery qry;
    QString q;
    q = "INSERT INTO Securities ("
        "Isin,"
        "Name,"
        "Ticker,"
        "CurrencyID,"
        "Notes,"
        "ApiID,"
        "ApiTicker)"
    "VALUES (?, ?, ?, ?, ?, ?, ?);";

    qry.prepare(q);
    qry.addBindValue(s.isin);
    qry.addBindValue(s.name);
    qry.addBindValue(s.ticker);
    qry.addBindValue(s.currency.id);
    qry.addBindValue(s.notes);
    qry.addBindValue((int)s.apiId);
    qry.addBindValue(s.apiTicker);

    if(!qry.exec())
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

int DataBase::securitiesTable_getId(QString &isin, int currencyId)
{
    int securityId;
    QString q;
    QSqlQuery qry;
    
    q = QString("SELECT ID FROM Securities WHERE Isin = '%1' AND CurrencyID = '%2';")
        .arg(isin.toUpper())
        .arg(currencyId);   
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    qry.next();
    securityId = qry.value(0).toInt();
    return securityId;
}

// watchLists table
bool DataBase::watchListsTable_init()
{
    QSqlQuery qry;
    QString q;

    q = "CREATE TABLE IF NOT EXISTS WatchLists ("
        "ID	INTEGER UNIQUE,"
        "SecurityID	INTEGER,"
        "WatchGroupID	INTEGER,"
        "PRIMARY KEY(ID AUTOINCREMENT),"
        "FOREIGN KEY(SecurityID) REFERENCES Securities(ID),"
        "FOREIGN KEY(WatchGroupID) REFERENCES WatchGroups(ID)"
    ");";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

bool DataBase::watchListsTable_addRecord(watchItem_t wi)
{
    QSqlQuery qry;
    QString q;
    q = "INSERT INTO Securities ("
        "SecurityID,"
        "WatchGroupID)"
    "VALUES (?, ?);";
    qry.prepare(q);
    qry.addBindValue(wi.security.id);
    qry.addBindValue(wi.watchGroup.id);

    if(!qry.exec())
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

// transactions table
bool DataBase::transactionsTable_init()
{
    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS Transactions ("
        "ID	INTEGER UNIQUE,"
        "TimeStamp	INTEGER NOT NULL,"
        "SecurityID	INTEGER NOT NULL,"
        "Quantity	DOUBLE NOT NULL,"
        "LocalUnitPrice	DOUBLE NOT NULL,"
        "ExchangeRate	DOUBLE NOT NULL,"
        "Value	DOUBLE NOT NULL,"
        "Comissions	DOUBLE NOT NULL,"
        "CurrencyID	INTEGER NOT NULL,"
        "BrokerID	INTEGER,"
        "FOREIGN KEY(BrokerID) REFERENCES Brokers(ID),"
        "FOREIGN KEY(SecurityID) REFERENCES Securities(ID),"
        "FOREIGN KEY(CurrencyID) REFERENCES Currencies(ID),"
        "UNIQUE(TimeStamp,SecurityID,Quantity),"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

bool DataBase::transactionsTable_addRecord(transaction_t &t)
{
    securitiesTable_addRecord(t.security);
    t.security.id = securitiesTable_getId(t.security.isin, t.security.currency.id);
    currenciesTable_addRecord(t.currency);
    t.currency.id = currenciesTable_getId(t.currency.name);

    QSqlQuery qry;
    QString q;

    q = "INSERT or REPLACE INTO Transactions ("
        "TimeStamp,"
        "SecurityID,"
        "Quantity,"
        "LocalUnitPrice,"
        "ExchangeRate,"
        "Value,"
        "Comissions,"
        "CurrencyID,"
        "BrokerID)"
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
    qry.prepare(q);

    qry.addBindValue(t.timeStamp);
    qry.addBindValue(t.security.id);
    qry.addBindValue(t.qty);
    qry.addBindValue(t.locUnitPrice);
    qry.addBindValue(t.exchRate);
    qry.addBindValue(t.value);
    qry.addBindValue(t.commissions);
    qry.addBindValue(t.currency.id);
    qry.addBindValue((int)t.brokerId);

    if(!qry.exec())
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

// securityPrices
bool DataBase::securityPricesTable_init()
{
    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS SecurityPrices ("
        "ID	INTEGER UNIQUE,"
        "SecurityID	INTEGER,"
        "TimeStamp	INTEGER NOT NULL,"
        "Open	DOUBLE,"
        "Close	DOUBLE,"
        "High	DOUBLE,"
        "Low	DOUBLE,"
        "Volume	DOUBLE,"
        "FOREIGN KEY(SecurityID) REFERENCES Securities(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

bool DataBase::securityPricesTable_addRecords(securityPrice_t &sp)
{
    QSqlQuery qry;
    QString q;

    q = "INSERT or REPLACE INTO Transactions ("
        "SecurityID,"
        "TimeStamp,"
        "Open,"
        "Close,"
        "High,"
        "Low,"
        "Volume)"
    "VALUES (?, ?, ?, ?, ?, ?, ?);";
    for(int i=0; i<sp.timeStamp.size(); i++)
    {
        qry.prepare(q);
        qry.addBindValue(sp.security.id);
        qry.addBindValue(sp.timeStamp.at(i));
        qry.addBindValue(sp.open.at(i));
        qry.addBindValue(sp.close.at(i));
        qry.addBindValue(sp.high.at(i));
        qry.addBindValue(sp.low.at(i));
        qry.addBindValue(sp.volume.at(i));
        if(!qry.exec())
        {
            SHOW_ERROR(qry);
            return false;
        }
    }
    return true;
}

// currencyPrices
bool DataBase::currencyPricesTable_init()
{  
    QString q;
    QSqlQuery qry;

    q = "CREATE TABLE IF NOT EXISTS CurrencyPrices ("
        "ID	INTEGER UNIQUE,"
        "CurrencyID	INTEGER,"
        "TimeStamp	INTEGER NOT NULL,"
        "Open	DOUBLE,"
        "Close	DOUBLE,"
        "High	DOUBLE,"
        "Low	DOUBLE,"
        "FOREIGN KEY(CurrencyID) REFERENCES Currencies(ID),"
        "PRIMARY KEY(ID AUTOINCREMENT)"
    ");";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}

bool DataBase::currencyPricesTable_addRecords(currencyPrice_t &cp)
{
    QSqlQuery qry;
    QString q;

    q = "INSERT or REPLACE INTO CurrencyPrices ("
        "CurrencyID,"
        "TimeStamp,"
        "Open,"
        "Close,"
        "High,"
        "Low)"
    "VALUES (?, ?, ?, ?, ?, ?);";
    for(int i=0; i<cp.timeStamp.size(); i++)
    {
        qry.prepare(q);
        qry.addBindValue(cp.currency.id);
        qry.addBindValue(cp.timeStamp.at(i));
        qry.addBindValue(cp.open.at(i));
        qry.addBindValue(cp.close.at(i));
        qry.addBindValue(cp.high.at(i));
        qry.addBindValue(cp.low.at(i));
        if(!qry.exec())
        {
            SHOW_ERROR(qry);
            return false;
        }
    }
    return true;
}

bool DataBase::transactionsView_init()
{
    QSqlQuery qry;
    QString q;

    q = "DROP VIEW IF EXISTS VW_TRANSACTIONS;";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }

    q = "CREATE VIEW VW_TRANSACTIONS AS "
        "SELECT Transactions.TimeStamp AS 'Date'"
            ",(SELECT Securities.Name FROM Securities WHERE Securities.ID=Transactions.SecurityID) AS 'Name'"
            ",(SELECT Securities.Ticker   FROM Securities WHERE Securities.ID=Transactions.SecurityID) AS 'Ticker'"
            ",(SELECT Securities.Isin   FROM Securities WHERE Securities.ID=Transactions.SecurityID) AS 'Isin'"
            ",Transactions.Quantity AS 'Quantity'"
            ",ROUND(Transactions.LocalUnitPrice, 2) AS 'Loc unit. price'"
            ",(SELECT Currencies.Name FROM Currencies WHERE Currencies.ID= "
                "(SELECT Securities.CurrencyID FROM Securities WHERE Securities.ID=Transactions.SecurityID) "
            " ) AS 'Loc cur.'"
            ",ROUND(Transactions.ExchangeRate, 4) AS 'Exch. Rate'"
            ",ROUND(Transactions.Value + Transactions.Comissions, 2) AS 'Value'"
            ",(SELECT Currencies.Name FROM Currencies WHERE Currencies.ID=Transactions.CurrencyID) AS 'Cur.' "
            "FROM Transactions;";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    return true;
}


DataBase::ApiEnum DataBase::stringToApiEnum(const QString &text)
{
    return QVariant(text).value<ApiEnum>();
}
QString DataBase::apiEnumToString(const DataBase::ApiEnum &enumValue)
{
    return QVariant::fromValue(enumValue).toString();
}

DataBase::BrokerEnum DataBase::stringToBrokerEnum(const QString &text)
{
    return QVariant(text).value<BrokerEnum>();
}
QString DataBase::brokerEnumToString(const DataBase::BrokerEnum &enumValue)
{
    return QVariant::fromValue(enumValue).toString();
}

