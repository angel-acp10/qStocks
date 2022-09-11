#include "database.h"

#include <QtSql>

#define SHOW_ERROR(query) {qDebug()<<"File: "<<__FILE__<<" Line: "<<__LINE__<<query.lastError();}

DataBase::DataBase(const QString & dbFile)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFile);

    yahoo = new YahooApi(NULL);

    connect(yahoo, &YahooApi::received_GetDailyPrice,
            this, &DataBase::onReceived_GetDailyPrice);
}

DataBase::~DataBase()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(db.isOpen())
        db.close();
    QSqlDatabase::removeDatabase(db.connectionName());

    delete yahoo;
}

bool DataBase::init(void)
{
    if( !open() )
        return false;

    bool ret = true;

    if(!settingsTable_init())      ret = false;
    if(!brokersTable_init())       ret = false;
    if(!apisTable_init())          ret = false;
    if(!watchGroupsTable_init())   ret = false;
    if(!currenciesTable_init())    ret = false;
    if(!securitiesTable_init())    ret = false;
    if(!watchListsTable_init())    ret = false;
    if(!transactionsTable_init())  ret = false;
    if(!securityPricesTable_init()) ret = false;
    if(!currencyPricesTable_init()) ret = false;

    if(!transactionsView_init())   ret = false;

    return ret;
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

    q = "INSERT or REPLACE INTO Apis (ID, Name) VALUES (?, ?);";
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

bool DataBase::currenciesTable_getCurrency(const int currencyId, currency_t &out_currency)
{
    QString q;
    QSqlQuery qry;

    q = QString("SELECT "
                "Currencies.ID "
                ",Currencies.Name "
                ",Currencies.ApiID "
                ",Currencies.ApiTicker "
            "FROM "
                "Currencies "
            "WHERE "
                "Currencies.ID = %1").arg(currencyId);
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    qry.next();

    out_currency.id = qry.value(0).toInt();
    out_currency.name = qry.value(1).toString();
    out_currency.apiId = (ApiEnum)qry.value(2).toInt();
    out_currency.apiTicker = qry.value(3).toString();
    return true;
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
        "CurrencyID	INTEGER NOT NULL,"
        "Ticker	VARCHAR(15),"
        "ApiID	INTEGER,"
        "ApiTicker	VARCHAR(15),"
        "Notes	INTEGER,"
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
        "CurrencyID,"
        "Ticker,"
        "ApiID,"
        "ApiTicker,"
        "Notes)"
    "VALUES (?, ?, ?, ?, ?, ?, ?);";

    qry.prepare(q);
    qry.addBindValue(s.isin);
    qry.addBindValue(s.name);
    qry.addBindValue(s.currency.id);
    qry.addBindValue(s.ticker);
    qry.addBindValue((int)s.apiId);
    qry.addBindValue(s.apiTicker);
    qry.addBindValue(s.notes);

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

bool DataBase::securitiesTable_getSecurity(const int securityId, security_t &out_security)
{
    QString q;
    QSqlQuery qry;

    q = QString("SELECT "
            "Securities.Isin "
            ",Securities.Name "
            ",Securities.Ticker "
            ",Securities.ApiID "
            ",Securities.ApiTicker "
            ",Securities.Notes "
            ",Securities.CurrencyID "
            ",Currencies.Name "
            ",Currencies.ApiID AS CurrApiID "
            ",Currencies.ApiTicker AS CurrApiTicker "
        "FROM "
            "Securities "
        "LEFT JOIN "
            "Currencies "
        "ON "
            "Securities.CurrencyID = Currencies.ID "
        "WHERE "
            "Securities.ID = %1").arg(securityId);
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    qry.next();

    out_security.id = securityId;
    out_security.isin = qry.value(0).toString();
    out_security.name = qry.value(1).toString();
    out_security.ticker = qry.value(2).toString();
    out_security.apiId = (ApiEnum)qry.value(3).toInt();
    out_security.apiTicker = qry.value(4).toString();
    out_security.notes = qry.value(5).toString();

    out_security.currency.id = qry.value(6).toInt();
    out_security.currency.name = qry.value(7).toString();
    out_security.currency.apiId = (ApiEnum)qry.value(8).toInt();
    out_security.currency.apiTicker = qry.value(9).toString();
    return true;
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
        "PRIMARY KEY(ID AUTOINCREMENT),"
        "UNIQUE(SecurityID,TimeStamp)"
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

    q = "INSERT or REPLACE INTO SecurityPrices ("
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
        qry.addBindValue(0);
        //qry.addBindValue(sp.volume.at(i));
        if(!qry.exec())
        {
            SHOW_ERROR(qry);
            return false;
        }
    }
    return true;
}


bool DataBase::currencyPricesTable_startUpdate()
{
    QString q;
    QSqlQuery qry;

    priceQuery_t priceQry;
    qint64 firstBuy_ts;
    qint64 lastPrice_ts;

    // get first buy
    q = "SELECT MIN(TimeStamp) FROM Transactions";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }
    qry.next();
    firstBuy_ts = qry.value(0).toInt();


    q = "SELECT "
            "Currencies.ID, "
            "Currencies.ApiID, "
            "Currencies.ApiTicker, "
            "cp.FirstPrice, "
            "cp.LastPrice "
        "FROM "
            "Currencies "
        "LEFT JOIN "
            "(SELECT "
                "CurrencyPrices.CurrencyID AS CurrencyID "
                ",MIN(CurrencyPrices.TimeStamp) AS FirstPrice "
                ",MAX(CurrencyPrices.TimeStamp) AS LastPrice "
            "FROM "
                "CurrencyPrices "
            "GROUP BY "
                "CurrencyPrices.CurrencyID "
            ") AS cp "
        "ON Currencies.ID = cp.CurrencyID ";
    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }

    priceQry.priceType = PRICE_CURRENCY;
    priceQry.end_ts = QDateTime::currentSecsSinceEpoch();

    while(qry.next())
    {
        priceQry.id = qry.value(0).toInt();
        priceQry.api = (ApiEnum)qry.value(1).toInt();
        priceQry.apiTicker = qry.value(2).toString();
        lastPrice_ts = qry.value(4).isNull() ? 0 : qry.value(4).toInt();
        qDebug()<<priceQry.apiTicker;

        if(lastPrice_ts == 0) // there's no new price
            priceQry.start_ts = firstBuy_ts;
        else // there is previous data
            priceQry.start_ts = lastPrice_ts;

        m_priceQueriesQueue.enqueue(priceQry);
    }
    return true;
}

bool DataBase::currencyPricesTable_get(const int currencyId,
                             const qint64 minTimeStamp,
                             const qint64 maxTimeStamp,
                             currencyPrice_t &out_cp)
{
    currency_t curr;
    if(!currenciesTable_getCurrency(currencyId, curr))
        return false;

    QString q;
    QSqlQuery qry;
    currencyPrice_t cp;

    q = QString("SELECT "
            "TimeStamp "
            ",Open "
            ",Close "
            ",High "
            ",Low "
        "FROM "
            "CurrencyPrices "
        "WHERE "
            "CurrencyID=%1 "
            "AND TimeStamp >= %2 "
            "AND TimeStamp <= %3 "
        "ORDER BY TimeStamp")
            .arg(currencyId)
            .arg(minTimeStamp)
            .arg(maxTimeStamp);

    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }

    int rows = qry.size();
    out_cp.timeStamp.resize(rows);
    out_cp.open.resize(rows);
    out_cp.close.resize(rows);
    out_cp.high.resize(rows);
    out_cp.low.resize(rows);

    out_cp.currency = curr;
    int i=0;
    while(qry.next())
    {
        out_cp.timeStamp[i] = qry.value(0).toInt();
        out_cp.open[i] = qry.value(1).toDouble();
        out_cp.close[i] = qry.value(2).toDouble();
        out_cp.high[i] = qry.value(3).toDouble();
        out_cp.low[i] = qry.value(4).toDouble();
        i++;
    }
    return true;
}

bool DataBase::pricesTable_continueUpdate()
{
    priceQuery_t priceQry;

    // dequeues until a not null apiTicker is found
    while(m_priceQueriesQueue.size() > 0)
    {
        priceQry = m_priceQueriesQueue.dequeue();
        if(priceQry.apiTicker != "")
        {
            emit pricesTable_status(QString("updating %1").arg(priceQry.apiTicker));
            switch(priceQry.api)
            {
            case API_YAHOO:
                yahoo->getDailyPrice(priceQry.id,
                                     priceQry.priceType,
                                       priceQry.apiTicker,
                                       priceQry.start_ts,
                                       priceQry.end_ts);
                break;
            default:
                break;
            }
            return true;
        }
        else
        {
            emit pricesTable_status("ERROR: missing ApiTicker");
        }
    }
    // no more items are available
    emit pricesTable_status("updated");
    return false;
}

void DataBase::onReceived_GetDailyPrice(const int id,
                                         const int priceType,
                                         const QString &symbol,
                                         const QString &currency,
                                         const QVector<qint64> &unixTime,
                                         const QVector<double> &open,
                                         const QVector<double> &close,
                                         const QVector<double> &high,
                                         const QVector<double> &low)
{
    (void)symbol;
    (void)currency;

    securityPrice_t sp;
    currencyPrice_t cp;

    switch((PriceEnum)priceType)
    {
    case PRICE_SECURITY:
        sp.security.id = id;
        sp.timeStamp = unixTime;
        sp.open = open;
        sp.close = close;
        sp.high = high;
        sp.low = low;

        securityPricesTable_addRecords(sp);
        pricesTable_continueUpdate();
        break;

    case PRICE_CURRENCY:
        cp.currency.id = id;
        cp.timeStamp = unixTime;
        cp.open = open;
        cp.close = close;
        cp.high = high;
        cp.low = low;

        currencyPricesTable_addRecords(cp);
        pricesTable_continueUpdate();
        break;

    default:
        break;
    }
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
        "PRIMARY KEY(ID AUTOINCREMENT),"
        "UNIQUE(CurrencyID,TimeStamp)"
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


bool DataBase::securityPricesTable_startUpdate()
{
    QString q;
    QSqlQuery qry;

    priceQuery_t priceQry;
    qint64 firstBuy_ts;
    qint64 lastPrice_ts;

    q = "SELECT "
            "p.SecurityID "
            ",p.FirstBuy "
            ",sp.FirstPrice "
            ",sp.LastPrice "
            ",s.ApiID "
            ",s.ApiTicker "
        "FROM "
            "(SELECT "
                "Transactions.SecurityID AS SecurityID "
                ",MIN(Transactions.TimeStamp) AS FirstBuy "
            "FROM "
                "Transactions "
            "GROUP BY "
                "Transactions.SecurityID "
            ") AS p "
        "LEFT JOIN "
            "(SELECT "
                "SecurityPrices.SecurityID AS SecurityID "
                ",MIN(SecurityPrices.TimeStamp) AS FirstPrice "
                ",MAX(SecurityPrices.TimeStamp) AS LastPrice "
            "FROM "
                "SecurityPrices "
            "GROUP BY "
                "SecurityPrices.SecurityID "
            ") AS sp "
        "ON p.SecurityID = sp.SecurityID "
        "LEFT JOIN Securities as s ON p.SecurityID = s.ID";

    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }

    priceQry.priceType = PRICE_SECURITY;
    priceQry.end_ts = QDateTime::currentSecsSinceEpoch();

    while(qry.next())
    {
        priceQry.id = qry.value(0).toInt();
        firstBuy_ts = qry.value(1).isNull() ? 0 : qry.value(1).toInt();
        lastPrice_ts = qry.value(3).isNull() ? 0 : qry.value(3).toInt();
        priceQry.api = (ApiEnum)qry.value(4).toInt();
        priceQry.apiTicker = qry.value(5).toString();
        qDebug()<<priceQry.apiTicker;

        if(lastPrice_ts == 0) // there's no new price
            priceQry.start_ts = firstBuy_ts;
        else // there is previous data
            priceQry.start_ts = lastPrice_ts;

        m_priceQueriesQueue.enqueue(priceQry);
    }
    return true;
}

bool DataBase::securityPricesTable_get(const int securityId,
                             const qint64 minTimeStamp,
                             const qint64 maxTimeStamp,
                             securityPrice_t &out_sp)
{
    security_t sec;
    if(!securitiesTable_getSecurity(securityId, sec))
        return false;

    QString q;
    QSqlQuery qry;

    q = QString("SELECT "
            "TimeStamp "
            ",Open "
            ",Close "
            ",High "
            ",Low "
            ",Volume "
        "FROM "
            "SecurityPrices "
        "WHERE "
            "SecurityID=%1 "
            "AND TimeStamp >= %2 "
            "AND TimeStamp <= %3 "
        "ORDER BY TimeStamp")
            .arg(securityId)
            .arg(minTimeStamp)
            .arg(maxTimeStamp);

    if(!qry.exec(q))
    {
        SHOW_ERROR(qry);
        return false;
    }

    int rows = qry.size();
    out_sp.timeStamp.resize(rows);
    out_sp.open.resize(rows);
    out_sp.close.resize(rows);
    out_sp.high.resize(rows);
    out_sp.low.resize(rows);
    out_sp.volume.resize(rows);

    out_sp.security = sec;
    int i=0;
    while(qry.next())
    {
        out_sp.timeStamp[i] = qry.value(0).toInt();
        out_sp.open[i] = qry.value(1).toDouble();
        out_sp.close[i] = qry.value(2).toDouble();
        out_sp.high[i] = qry.value(3).toDouble();
        out_sp.low[i] = qry.value(4).toDouble();
        out_sp.volume[i] = qry.value(5).toDouble();
        i++;
    }
    return true;
}

bool DataBase::pricesTable_startUpdate()
{
    bool ret = true;

    if(!securityPricesTable_startUpdate())  ret = false;
    if(!currencyPricesTable_startUpdate())  ret = false;
    pricesTable_continueUpdate();

    return ret;
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

