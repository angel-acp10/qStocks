#include "watchitem.h"

WatchItem::WatchItem(int productId, int watchListId) :
    mId(-1),
    mProductId(productId),
    mWatchListId(watchListId)
{
}

int WatchItem::id() const
{
    return mId;
}

void WatchItem::setId(int id)
{
    mId = id;
}

int WatchItem::productId() const
{
    return mProductId;
}

void WatchItem::setProductId(int productId)
{
    mProductId = productId;
}

int WatchItem::watchListId() const
{
    return mWatchListId;
}

void WatchItem::setWatchListId(int watchListId)
{
    mWatchListId = watchListId;
}


