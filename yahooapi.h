#ifndef YAHOOAPI_H
#define YAHOOAPI_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWidget>


class YahooApi : public QWidget
{
    Q_OBJECT

public:
    YahooApi(QWidget *parent);
    ~YahooApi();

public slots:
    void searchTicker(const QString &ticker);
    void getDailyPrice(const int id,
                       const int priceType,
                       const QString &ticker,
                       const qint64 startEpoch,
                       const qint64 endEpoch);

signals:
    void received_SearchTicker(const QStringList &exchanges,
                                const QStringList &names,
                                const QStringList &types,
                                const QStringList &tickers );
    void received_GetDailyPrice(const int id,
                                 const int priceType,
                                 const QString &symbol,
                                 const QString &currency,
                                 const QVector<qint64> &unixTime,
                                 const QVector<double> &open,
                                 const QVector<double> &close,
                                 const QVector<double> &high,
                                 const QVector<double> &low);

private:
    int m_id;
    int m_priceType;
    QString m_ticker;
    qint64 m_startEpoch;
    qint64 m_endEpoch;

    // decode response
    typedef struct{
        const QString baseUrl;
        void(YahooApi::*decode)(const QByteArray answer);
    }decodeResponse_t;

    void decodeResponse_SearchTicker(const QByteArray answer);
    void decodeResponse_GetDailyStockPrice(const QByteArray answer);

    QNetworkAccessManager *manager;
    QNetworkRequest request;

    decodeResponse_t decodeArray[3] = {
        {"https://query1.finance.yahoo.com/v1/finance/search?q=", &YahooApi::decodeResponse_SearchTicker},
        {"https://query1.finance.yahoo.com/v8/finance/chart/", &YahooApi::decodeResponse_GetDailyStockPrice},
        {"", NULL}
    };

private slots:
    void onReceived_httpGet(QNetworkReply *reply);

};

#endif // YAHOOAPI_H
