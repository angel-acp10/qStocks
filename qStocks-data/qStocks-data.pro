QT -= gui
QT += sql

TEMPLATE = lib
DEFINES += QSTOCKSDATA_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Api.cpp \
    ApiDao.cpp \
    Broker.cpp \
    BrokerDao.cpp \
    Currency.cpp \
    CurrencyDao.cpp \
    DatabaseManager.cpp \
    Dividend.cpp \
    DividendDao.cpp \
    Price.cpp \
    PriceDao.cpp \
    Product.cpp \
    ProductDao.cpp \
    ProductType.cpp \
    ProductTypeDao.cpp \
    Split.cpp \
    SplitDao.cpp \
    Transaction.cpp \
    TransactionDao.cpp \
    WatchItem.cpp \
    WatchItemDao.cpp \
    WatchList.cpp \
    WatchListDao.cpp

HEADERS += \
    Api.h \
    ApiDao.h \
    Broker.h \
    BrokerDao.h \
    Currency.h \
    CurrencyDao.h \
    DatabaseManager.h \
    Dividend.h \
    DividendDao.h \
    Price.h \
    PriceDao.h \
    Product.h \
    ProductDao.h \
    ProductType.h \
    ProductTypeDao.h \
    Split.h \
    SplitDao.h \
    Transaction.h \
    TransactionDao.h \
    WatchItem.h \
    WatchItemDao.h \
    WatchList.h \
    WatchListDao.h \
    qStocks-data_global.h

# Default rules for deployment.
!isEmpty(target.path): INSTALLS += target
