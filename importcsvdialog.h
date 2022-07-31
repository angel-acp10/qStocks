#ifndef IMPORTCSVDIALOG_H
#define IMPORTCSVDIALOG_H

#include <QDialog>
#include "database.h"

namespace Ui {
class ImportCsvDialog;
}

class ImportCsvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportCsvDialog(QWidget *parent, DataBase *db);
    ~ImportCsvDialog();

private:
    Ui::ImportCsvDialog *ui;

    DataBase *db;

    void importCsv_degiro(QString csvFile);

private slots:
    void onClicked_select(void);
    void onClicked_import(void);
};

#endif // IMPORTCSVDIALOG_H
