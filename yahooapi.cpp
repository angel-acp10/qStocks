#include "yahooapi.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#include <QDebug>

YahooApi::YahooApi(QWidget *parent) : QWidget(parent)
{
    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished,
            this, &YahooApi::onReceived_httpGet);
}

YahooApi::~YahooApi()
{
    delete manager;
}

void YahooApi::searchTicker(const QString &ticker)
{
    QString url = "https://query1.finance.yahoo.com/v1/finance/search?q=" + ticker;
    request.setUrl(QUrl(url));
    manager->get(request);
}

void YahooApi::getDailyPrice(const int id, const int priceType, const QString &ticker, const qint64 startEpoch, const qint64 endEpoch)
{
    m_id = id;
    m_priceType = priceType;
    QString url = QString("https://query1.finance.yahoo.com/v8/finance/chart/%1?"
                          "&interval=1d"
                          "&period1=%2"
                          "&period2=%3")
                            .arg(ticker)
                            .arg(startEpoch)
                            .arg(endEpoch);
    qDebug()<<"YahooApi::getDailyPrice - "<<url;
    request.setUrl(QUrl(url));
    manager->get(request);
}


void YahooApi::onReceived_httpGet(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QUrl url = reply->url();
    QByteArray answer = reply->readAll();

    for(int i=0; decodeArray[i].baseUrl!=""; i++)
    {
        if( url.toString().startsWith( decodeArray[i].baseUrl ) )
            (this->*decodeArray[i].decode)(answer);
    }
}

void YahooApi::decodeResponse_SearchTicker(const QByteArray answer)
{
    qDebug() << "decodeResponse_SearchTicker";

    QJsonDocument jsonDoc = QJsonDocument::fromJson(answer);
    if(!jsonDoc.isObject())
        return;

    QStringList exchanges, names, types, tickers;

    // reads quotes
    QJsonArray quotes = jsonDoc.object().take("quotes").toArray();
    QJsonArray::const_iterator quotes_iterator = quotes.constBegin();
    while(quotes_iterator != quotes.constEnd())
    {
        QJsonObject quote = quotes_iterator->toObject();

        QString exchange = quote.take("exchange").toString();
        QString name = quote.take("shortname").toString();
        QString type = quote.take("quoteType").toString();
        QString ticker = quote.take("symbol").toString();
        exchanges.append(exchange);
        tickers.append(ticker);
        names.append(name);
        types.append(type);
        quotes_iterator++;
    }

    emit received_SearchTicker(exchanges, names, types, tickers);
}


void YahooApi::decodeResponse_GetDailyStockPrice(const QByteArray answer)
{
    qDebug() << "decodeResponse_GetDailyStockPrice";

    QJsonDocument jsonDoc = QJsonDocument::fromJson(answer);
    if(!jsonDoc.isObject())
        return;

    // reads metadata
    QJsonObject rMeta = jsonDoc.object().take("chart").toObject()
                                    .take("result").toArray()
                                    .at(0).toObject()
                                    .take("meta").toObject();
    QString currency = rMeta.take("currency").toString();
    QString symbol = rMeta.take("symbol").toString();

    // reads timestamps
    QVector <qint64> unixTime;
    QJsonArray rTime = jsonDoc.object().take("chart").toObject()
                                        .take("result").toArray()
                                        .at(0).toObject()
                                        .take("timestamp").toArray();
    for(auto i = rTime.begin(); i != rTime.end(); i++)
        unixTime.append( (*i).toInteger() );

    // pointer to quote
    QJsonObject rQuote = jsonDoc.object().take("chart").toObject()
                                            .take("result").toArray()
                                            .at(0).toObject()
                                            .take("indicators").toObject()
                                            .take("quote").toArray()
                                            .at(0).toObject();

    // reads open
    QVector <double> open;
    QJsonArray rOpen = rQuote.take("open").toArray();
    for(auto i = rOpen.begin(); i != rOpen.end(); i++)
        open.append( (*i).toDouble() );

    // reads close
    QVector <double> close;
    QJsonArray rClose = rQuote.take("close").toArray();
    for(auto i = rClose.begin(); i != rClose.end(); i++)
        close.append( (*i).toDouble() );

    // reads high
    QVector <double> high;
    QJsonArray rHigh = rQuote.take("high").toArray();
    for(auto i = rHigh.begin(); i != rHigh.end(); i++)
        high.append( (*i).toDouble() );

    // reads low
    QVector <double> low;
    QJsonArray rLow = rQuote.take("low").toArray();
    for(auto i = rLow.begin(); i != rLow.end(); i++)
        low.append( (*i).toDouble() );

    emit received_GetDailyPrice(m_id, m_priceType, symbol, currency, unixTime, open, close, high, low);
}
