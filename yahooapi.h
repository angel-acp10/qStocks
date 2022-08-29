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
    void SearchTicker(const QString &ticker);
    void GetDailyStockPrice(const QString &ticker, const QDateTime start, const QDateTime end);

signals:
    void received_SearchTicker(const QStringList &results);
    void received_GetDailyStockPrice(const QString &symbol,
                                     const QString &currency,
                                     const QVector<qint64> &unixTime,
                                     const QVector<double> &open,
                                     const QVector<double> &close,
                                     const QVector<double> &high,
                                     const QVector<double> &low);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;

    typedef struct{
        const QString baseUrl;
        void(YahooApi::*decode)(const QByteArray answer);
    }decodeResponse_t;

    void decodeResponse_SearchTicker(const QByteArray answer);
    void decodeResponse_GetDailyStockPrice(const QByteArray answer);

    decodeResponse_t decodeArray[3] = {
        {"https://query1.finance.yahoo.com/v1/finance/search?q=", &YahooApi::decodeResponse_SearchTicker},
        {"https://query1.finance.yahoo.com/v8/finance/chart/", &YahooApi::decodeResponse_GetDailyStockPrice},
        {"", NULL}
    };

private slots:
    void onReceived_httpGet(QNetworkReply *reply);

};

#endif // YAHOOAPI_H
