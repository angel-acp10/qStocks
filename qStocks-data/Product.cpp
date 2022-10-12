#include "Product.h"

Product::Product(const int typeId,
                   const QString& name,
                   int currencyId) :
    mId(-1),
    mTypeId(typeId),
    mIsin(""),
    mName(name),
    mTicker(""),
    mCurrencyId(currencyId),
    mAuxCurrencyId(-1),
    mNotes(""),
    mApiId(-1),
    mApiTicker("")
{
}

int Product::id() const
{
    return mId;
}

void Product::setId(int id)
{
    mId = id;
}

int Product::typeId() const
{
    return mId;
}

void Product::setTypeId(int typeId)
{
    mTypeId = typeId;
}


QString Product::isin() const
{
    return mIsin;
}

void Product::setIsin(const QString& isin)
{
    mIsin = isin;
}

QString Product::name() const
{
    return mName;
}

void Product::setName(const QString& name)
{
    mName = name;
}

QString Product::ticker() const
{
    return mTicker;
}

void Product::setTicker(const QString& ticker)
{
    mTicker = ticker;
}

int Product::currencyId() const
{
    return mCurrencyId;
}

void Product::setCurrencyId(int currencyId)
{
    mCurrencyId = currencyId;
}

int Product::auxCurrencyId() const
{
    return mAuxCurrencyId;
}

void Product::setAuxCurrencyId(int auxCurrencyId)
{
    mAuxCurrencyId = auxCurrencyId;
}

QString Product::notes() const
{
    return mNotes;
}

void Product::setNotes(const QString& notes)
{
    mNotes = notes;
}

int Product::apiId() const
{
    return mApiId;
}

void Product::setApiId(int apiId)
{
    mApiId = apiId;
}

QString Product::apiTicker() const
{
    return mApiTicker;
}

void Product::setApiTicker(const QString& apiTicker)
{
    mApiTicker = apiTicker;
}
