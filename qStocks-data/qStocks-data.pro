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
    Currency.cpp \
    Dividend.cpp \
    Price.cpp \
    Product.cpp \
    ProductType.cpp \
    Split.cpp \
    Transaction.cpp \
    WatchItem.cpp \
    WatchList.cpp

HEADERS += \
    Api.h \
    ApiDao.h \
    Broker.h \
    Currency.h \
    Dividend.h \
    Price.h \
    Product.h \
    ProductType.h \
    Split.h \
    Transaction.h \
    WatchItem.h \
    WatchList.h \
    qStocks-data_global.h

# Default rules for deployment.
!isEmpty(target.path): INSTALLS += target
