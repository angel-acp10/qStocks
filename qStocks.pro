QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    currenciesconfigdialog.cpp \
    database.cpp \
    importcsvdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    portfolio.cpp \
    securitiesconfigdialog.cpp \
    settingsdialog.cpp \
    transactions.cpp \
    transactionsproxymodel.cpp \
    yahooapi.cpp

HEADERS += \
    columns.h \
    currenciesconfigdialog.h \
    database.h \
    importcsvdialog.h \
    mainwindow.h \
    portfolio.h \
    securitiesconfigdialog.h \
    settingsdialog.h \
    transactions.h \
    transactionsproxymodel.h \
    yahooapi.h

FORMS += \
    currenciesconfigdialog.ui \
    importcsvdialog.ui \
    mainwindow.ui \
    portfolio.ui \
    securitiesconfigdialog.ui \
    settingsdialog.ui \
    transactions.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
