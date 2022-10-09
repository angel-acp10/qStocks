QT -= gui
QT += sql

TEMPLATE = lib
DEFINES += QSTOCKSDATA_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    api.cpp \
    broker.cpp \
    currency.cpp \
    dividend.cpp \
    price.cpp \
    product.cpp \
    producttype.cpp \
    split.cpp \
    transaction.cpp \
    watchitem.cpp \
    watchlist.cpp

HEADERS += \
    api.h \
    broker.h \
    dividend.h \
    price.h \
    product.h \
    producttype.h \
    qStocks-data_global.h \
    currency.h \
    split.h \
    transaction.h \
    watchitem.h \
    watchlist.h

# Default rules for deployment.
!isEmpty(target.path): INSTALLS += target
