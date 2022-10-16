#ifndef PRODUCT_H
#define PRODUCT_H

#include "qStocks-data_global.h"
#include <QString>

class QSTOCKSDATA_EXPORT Product
{
public:
    explicit Product(const int typeId = -1,
                      const QString& name = "",
                      int currencyId = -1);

    int id() const;
    void setId(int id);

    int typeId() const;
    void setTypeId(int typeId);

    QString isin() const;
    void setIsin(const QString& isin);

    QString name() const;
    void setName(const QString& name);

    QString ticker() const;
    void setTicker(const QString& ticker);

    int currencyId() const;
    void setCurrencyId(int currencyId);

    int auxCurrencyId() const;
    void setAuxCurrencyId(int auxCurrencyId);

    QString notes() const;
    void setNotes(const QString& notes);

    int apiId() const;
    void setApiId(int apiId);

    QString apiTicker() const;
    void setApiTicker(const QString& apiTicker);

private:
    int mId;
    int mTypeId;
    QString mIsin;
    QString mName;
    QString mTicker;
    int mCurrencyId;
    int mAuxCurrencyId;
    QString mNotes;
    int mApiId;
    QString mApiTicker;
};

#endif // PRODUCT_H
