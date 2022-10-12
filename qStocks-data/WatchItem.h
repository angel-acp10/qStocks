#ifndef WATCHITEM_H
#define WATCHITEM_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT WatchItem
{
public:
    explicit WatchItem(int productId, int watchListId);

    int id() const;
    void setId(int id);

    int productId() const;
    void setProductId(int productId);

    int watchListId() const;
    void setWatchListId(int watchListId);

private:
    int mId;
    int mProductId;
    int mWatchListId;
};

#endif // WATCHITEM_H
