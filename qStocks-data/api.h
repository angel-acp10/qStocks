#ifndef API_H
#define API_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT Api
{
public:
    explicit Api(const QString& name);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);


private:
    int mId;
    QString mName;
};

#endif // API_H
