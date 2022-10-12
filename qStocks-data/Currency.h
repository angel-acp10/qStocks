#ifndef CURRENCY_H
#define CURRENCY_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT Currency
{
public:
    explicit Currency(const QString& name);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);


private:
    int mId;
    QString mName;
};

#endif // CURRENCY_H
