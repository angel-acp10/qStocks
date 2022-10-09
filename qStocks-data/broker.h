#ifndef BROKER_H
#define BROKER_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT Broker
{
public:
    explicit Broker(const QString& name);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);


private:
    int mId;
    QString mName;
};

#endif // BROKER_H
