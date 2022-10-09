#include "watchlist.h"

WatchList::WatchList(const QString& name) :
    mId(-1),
    mName(name),
    mColor(""),
    mDescription("")
{
}


int WatchList::id() const
{
    return mId;
}

void WatchList::setId(int id)
{
    mId = id;
}

QString WatchList::name() const
{
    return mName;
}

void WatchList::setName(const QString& name)
{
    mName = name;
}

QString WatchList::color() const
{
    return mColor;
}

void WatchList::setColor(const QString& color)
{
    mColor = color;
}

QString WatchList::description() const
{
    return mDescription;
}

void WatchList::setDescription(const QString& description)
{
    mDescription = description;
}
