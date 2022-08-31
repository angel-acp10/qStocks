#ifndef SECURITIESCONFIGDIALOG_H
#define SECURITIESCONFIGDIALOG_H

#include <QDialog>
#include "database.h"

namespace Ui {
class SecuritiesConfigDialog;
}

class SecuritiesConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecuritiesConfigDialog(QWidget *parent = nullptr);
    ~SecuritiesConfigDialog();

private:
    Ui::SecuritiesConfigDialog *ui;

    QSqlRelationalTableModel *model;
};

#endif // SECURITIESCONFIGDIALOG_H
