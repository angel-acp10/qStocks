#ifndef PRICE_H
#define PRICE_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT Price
{
public:
    explicit Price(int timeStamp = -1, int productId = -1,
                   double open = -1, double close = -1,
                   double high = -1, double low = -1,
                   double volume = -1);

    int id() const;
    void setId(int id);

    int timeStamp() const;
    void setTimeStamp(int timeStamp);

    int productId() const;
    void setProductId(int productId);

    double open() const;
    void setOpen(double open);

    double close() const;
    void setClose(double close);

    double high() const;
    void setHigh(double high);

    double low() const;
    void setLow(double low);

    double volume() const;
    void setVolume(double volume);

private:
    int mId;
    int mTimeStamp;
    int mProductId;
    double mOpen;
    double mClose;
    double mHigh;
    double mLow;
    double mVolume;
};

#endif // PRICE_H
