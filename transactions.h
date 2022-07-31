#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QWidget>

#include "database.h"
#include "importcsvdialog.h"
#include "transactionsproxymodel.h"


namespace Ui {
class Transactions;
}

class Transactions : public QWidget
{
    Q_OBJECT

public:
    explicit Transactions(QWidget *parent, DataBase *db);
    ~Transactions();

private:
    Ui::Transactions *ui;
    ImportCsvDialog *import_d;

    DataBase * db;
    QSqlTableModel *model;
    TransactionsProxyModel *trProxyModel;

private slots:
    void onClicked_importCsv(void);
};

#endif // TRANSACTIONS_H
