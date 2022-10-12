#include "ProductType.h"

ProductType::ProductType(const QString& name) :
    mId(-1),
    mName(name)
{
}

int ProductType::id() const
{
    return mId;
}

void ProductType::setId(int id)
{
    mId = id;
}

QString ProductType::name() const
{
    return mName;
}

void ProductType::setName(const QString& name)
{
    mName = name;
}
