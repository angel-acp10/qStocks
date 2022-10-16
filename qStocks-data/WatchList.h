#ifndef WATCHLIST_H
#define WATCHLIST_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT WatchList
{
public:
    explicit WatchList(const QString& name = "");

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    QString color() const;
    void setColor(const QString& color);

    QString description() const;
    void setDescription(const QString& description);

private:
    int mId;
    QString mName;
    QString mColor;
    QString mDescription;
};

#endif // WATCHLIST_H
