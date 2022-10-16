#ifndef DIVIDEND_H
#define DIVIDEND_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT Dividend
{
public:
    explicit Dividend(int timeStamp = -1, int productId = -1, double amount = -1);

    int id() const;
    void setId(int id);

    int timeStamp() const;
    void setTimeStamp(int timeStamp);

    int productId() const;
    void setProductId(int productId);

    double amount() const;
    void setAmount(double amount);

private:
    int mId;
    int mTimeStamp;
    int mProductId;
    double mAmount;
};

#endif // DIVIDEND_H
