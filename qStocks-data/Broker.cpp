#include "Broker.h"

Broker::Broker(const QString& name) :
    mId(-1),
    mName(name)
{
}


int Broker::id() const
{
    return mId;
}

void Broker::setId(int id)
{
    mId = id;
}

QString Broker::name() const
{
    return mName;
}

void Broker::setName(const QString& name)
{
    mName = name;
}

