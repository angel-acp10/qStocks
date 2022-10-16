#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT Transaction
{
public:
    explicit Transaction(int timeStamp = -1, int productId = -1, int productTypeId = -1,
                         double quantity = 0, double localUnitPrice = 0, double value = 0,
                         double comissions = 0, int currencyId = -1, int brokerId = -1);

    int id() const;
    void setId(int id);

    int timeStamp() const;
    void setTimeStamp(int timeStamp);

    int productId() const;
    void setProductId(int productId);

    int productTypeId() const;
    void setProductTypeId(int productTypeId);

    double quantity() const;
    void setQuantity(int quantity);

    double localUnitPrice() const;
    void setLocalUnitPrice(double localUnitPrice);

    double value() const;
    void setValue(double value);

    double comissions() const;
    void setComissions(double comissions);

    int currencyId() const;
    void setCurrencyId(int currencyId);

    int brokerId() const;
    void setBrokerId(int brokerId);

private:
    int mId;
    int mTimeStamp;
    int mProductId;
    int mProductTypeId;
    double mQuantity;
    double mLocalUnitPrice;
    double mValue;
    double mComissions;
    int mCurrencyId;
    int mBrokerId;
};

#endif // TRANSACTION_H
