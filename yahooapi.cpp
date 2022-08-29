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

void YahooApi::SearchTicker(const QString &ticker)
{
    QString url = "https://query1.finance.yahoo.com/v1/finance/search?q=" + ticker;
    request.setUrl(QUrl(url));
    manager->get(request);
}

void YahooApi::GetDailyStockPrice(const QString &ticker, const QDateTime start, const QDateTime end)
{
    QString url = QString("https://query1.finance.yahoo.com/v8/finance/chart/%1?"
                          "&interval=1d"
                          "&period1=%2"
                          "&period2=%3")
                            .arg(ticker)
                            .arg(start.toSecsSinceEpoch())
                            .arg(end.toSecsSinceEpoch());
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

    QStringList results;

    // reads quotes
    QJsonArray quotes = jsonDoc.object().take("quotes").toArray();
    QJsonArray::const_iterator quotes_iterator = quotes.constBegin();
    while(quotes_iterator != quotes.constEnd())
    {
        QJsonObject quote = quotes_iterator->toObject();
        QString ticker = quote.take("symbol").toString();
        results.append(ticker);
        quotes_iterator++;
    }
    emit received_SearchTicker(results);
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

    emit received_GetDailyStockPrice(symbol, currency, unixTime, open, close, high, low);
}
