#include "securitiesconfigdialog.h"
#include "ui_securitiesconfigdialog.h"
#include "columns.h"

SecuritiesConfigDialog::SecuritiesConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecuritiesConfigDialog)
{
    ui->setupUi(this);

    // model
    QSqlDatabase database = QSqlDatabase::database();
    model = new QSqlRelationalTableModel(this, database);
    model->setTable("Securities");
    model->setRelation(COL_SEC_CurrencyID, QSqlRelation("Currencies", "ID", "Name"));
    model->setRelation(COL_SEC_ApiID, QSqlRelation("Apis", "ID", "Name"));
    //model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate());

    // hidden columns
    QList<securitiesColumns_t> hiddenColumns ({
        COL_SEC_ID,
        COL_SEC_Notes
    });
    for(auto col : hiddenColumns)
        ui->tableView->setColumnHidden(col, true);

    // other properties
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView->show();
}

SecuritiesConfigDialog::~SecuritiesConfigDialog()
{
    delete ui;
    delete model;
}
