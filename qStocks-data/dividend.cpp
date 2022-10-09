#include "dividend.h"

Dividend::Dividend(int timeStamp, int productId, double amount) :
    mId(-1),
    mTimeStamp(timeStamp),
    mProductId(productId),
    mAmount(amount)
{
}


int Dividend::id() const
{
    return mId;
}

void Dividend::setId(int id)
{
    mId = id;
}

int Dividend::timeStamp() const
{
    return mTimeStamp;
}

void Dividend::setTimeStamp(int timeStamp)
{
    mTimeStamp = timeStamp;
}

int Dividend::productId() const
{
    return mProductId;
}

void Dividend::setProductId(int productId)
{
    mProductId = productId;
}

double Dividend::amount() const
{
    return mAmount;
}

void Dividend::setAmount(double amount)
{
    mAmount = amount;
}
