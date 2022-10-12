#include "price.h"

Price::Price(int timeStamp, int productId,
             double open, double close,
             double high, double low,
             double volume) :
    mId(-1),
    mTimeStamp(timeStamp),
    mProductId(productId),
    mOpen(open),
    mClose(close),
    mHigh(high),
    mLow(low),
    mVolume(volume)
{
}


int Price::id() const
{
    return mId;
}

void Price::setId(int id)
{
    mId = id;
}

int Price::timeStamp() const
{
    return mTimeStamp;
}

void Price::setTimeStamp(int timeStamp)
{
    mTimeStamp = timeStamp;
}

int Price::productId() const
{
    return mProductId;
}

void Price::setProductId(int productId)
{
    mProductId = productId;
}

double Price::open() const
{
    return mOpen;
}
void Price::setOpen(double open)
{
    mOpen = open;
}

double Price::close() const
{
    return mClose;
}
void Price::setClose(double close)
{
    mClose = close;
}

double Price::high() const
{
    return mHigh;
}
void Price::setHigh(double high)
{
    mHigh = high;
}

double Price::low() const
{
    return mLow;
}
void Price::setLow(double low)
{
    mLow = low;
}

double Price::volume() const
{
    return mVolume;
}
void Price::setVolume(double volume)
{
    mVolume = volume;
}
