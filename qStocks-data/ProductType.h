#ifndef PRODUCTTYPE_H
#define PRODUCTTYPE_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT ProductType
{
public:
    explicit ProductType(const QString& name);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

private:
    int mId;
    QString mName;
};

#endif // PRODUCTTYPE_H
