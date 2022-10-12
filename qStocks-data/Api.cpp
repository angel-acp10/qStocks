#include "api.h"

Api::Api(const QString& name) :
    mId(-1),
    mName(name)
{
}


int Api::id() const
{
    return mId;
}

void Api::setId(int id)
{
    mId = id;
}

QString Api::name() const
{
    return mName;
}

void Api::setName(const QString& name)
{
    mName = name;
}

