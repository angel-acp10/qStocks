#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QWidget>

#include "database.h"

namespace Ui {
class Portfolio;
}

class Portfolio : public QWidget
{
    Q_OBJECT

public:
    explicit Portfolio(QWidget *parent, DataBase *db);
    ~Portfolio();

private:
    Ui::Portfolio *ui;
    DataBase * db;
};

#endif // PORTFOLIO_H
