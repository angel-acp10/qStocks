#include "currency.h"

Currency::Currency(const QString& name) :
    mId(-1),
    mName(name)
{
}

int Currency::id() const
{
    return mId;
}

void Currency::setId(int id)
{
    mId = id;
}

QString Currency::name() const
{
    return mName;
}

void Currency::setName(const QString& name)
{
    mName = name;
}

