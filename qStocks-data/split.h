#ifndef SPLIT_H
#define SPLIT_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT Split
{
public:
    explicit Split(int timeStamp, int productId, int numerator, int denominator);

    int id() const;
    void setId(int id);

    int timeStamp() const;
    void setTimeStamp(int timeStamp);

    int productId() const;
    void setProductId(int productId);

    int numerator() const;
    void setNumerator(int numerator);

    int denominator() const;
    void setDenominator(int denominator);

private:
    int mId;
    int mTimeStamp;
    int mProductId;
    int mNumerator;
    int mDenominator;
};

#endif // SPLIT_H
