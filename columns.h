#ifndef COLUMNS_H
#define COLUMNS_H

typedef enum{
    COL_TR_DateTime=0,
    COL_TR_Name,
    COL_TR_Ticker,
    COL_TR_Isin,
    COL_TR_Quantity,
    COL_TR_UnitPrice,
    COL_TR_LocCurrency,
    COL_TR_ExchangeRate,
    COL_TR_Value,
    COL_TR_Currency,
}transactionsColumns_t;

typedef enum{
    COL_SEC_ID=0,
    COL_SEC_Isin,
    COL_SEC_Name,
    COL_SEC_CurrencyID,
    COL_SEC_Ticker,
    COL_SEC_ApiID,
    COL_SEC_ApiTicker,
    COL_SEC_Notes,
}securitiesColumns_t;

#endif // COLUMNS_H
