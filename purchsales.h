#ifndef PURCHSALES_H
#define PURCHSALES_H

#include <QWidget>

#include "database.h"

namespace Ui {
class PurchSales;
}

class PurchSales : public QWidget
{
    Q_OBJECT

public:
    explicit PurchSales(QWidget *parent, DataBase *db);
    ~PurchSales();

private:
    Ui::PurchSales *ui;

    DataBase *db;
};

#endif // PURCHSALES_H
