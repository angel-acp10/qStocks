#include "Split.h"

Split::Split(int timeStamp, int productId, int numerator, int denominator) :
    mId(-1),
    mTimeStamp(timeStamp),
    mProductId(productId),
    mNumerator(numerator),
    mDenominator(denominator)
{
}


int Split::id() const
{
    return mId;
}

void Split::setId(int id)
{
    mId = id;
}

int Split::timeStamp() const
{
    return mTimeStamp;
}

void Split::setTimeStamp(int timeStamp)
{
    mTimeStamp = timeStamp;
}

int Split::productId() const
{
    return mProductId;
}

void Split::setProductId(int productId)
{
    mProductId = productId;
}

int Split::numerator() const
{
    return mNumerator;
}

void Split::setNumerator(int numerator)
{
    mNumerator = numerator;
}

int Split::denominator() const
{
    return mDenominator;
}

void Split::setDenominator(int denominator)
{
    mDenominator = denominator;
}

