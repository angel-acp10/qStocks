#include "Transaction.h"

Transaction::Transaction(int timeStamp, int productId, int productTypeId,
    double quantity, double localUnitPrice, double value,
    double comissions, int currencyId, int brokerId) :
        mId(-1),
        mTimeStamp(timeStamp),
        mProductId(productId),
        mProductTypeId(productTypeId),
        mQuantity(quantity),
        mLocalUnitPrice(localUnitPrice),
        mValue(value),
        mComissions(comissions),
        mCurrencyId(currencyId),
        mBrokerId(brokerId)
{
}


int Transaction::id() const
{
    return mId;
}
void Transaction::setId(int id)
{
    mId = id;
}

int Transaction::timeStamp() const
{
    return mTimeStamp;
}
void Transaction::setTimeStamp(int timeStamp)
{
    mTimeStamp = timeStamp;
}

int Transaction::productId() const
{
    return mProductId;
}
void Transaction::setProductId(int productId)
{
    mProductId = productId;
}

double Transaction::quantity() const
{
    return mQuantity;
}
void Transaction::setQuantity(int quantity)
{
    mQuantity = quantity;
}

double Transaction::localUnitPrice() const
{
    return mLocalUnitPrice;
}
void Transaction::setLocalUnitPrice(double localUnitPrice)
{
    mLocalUnitPrice = localUnitPrice;
}

double Transaction::value() const
{
    return mValue;
}
void Transaction::setValue(double value)
{
    mValue = value;
}

double Transaction::comissions() const
{
    return mComissions;
}
void Transaction::setComissions(double comissions)
{
    mComissions = comissions;
}

int Transaction::currencyId() const
{
    return mCurrencyId;
}
void Transaction::setCurrencyId(int currencyId)
{
    mCurrencyId = currencyId;
}

int Transaction::brokerId() const
{
    return mBrokerId;
}
void Transaction::setBrokerId(int brokerId)
{
    mBrokerId = brokerId;
}
