#include "transactions.h"
#include "ui_transactions.h"
#include "columns.h"

Transactions::Transactions(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::Transactions)
{
    ui->setupUi(this);
    this->db = db;
    this->db->init();

    // model
    QSqlDatabase database = QSqlDatabase::database();
    model = new QSqlTableModel(this, database);
    model->setTable("VW_TRANSACTIONS");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    // proxy model
    trProxyModel = new TransactionsProxyModel();
    trProxyModel->setSourceModel(model);
    ui->transactions_tableView->setModel(trProxyModel);

    // hidden columns
    QList<transactionsColumns_t> hiddenColumns ({
        COL_TR_Isin,
    });
    for(auto col : hiddenColumns)
        ui->transactions_tableView->setColumnHidden(col, true);

    // other properties
    ui->transactions_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->transactions_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->transactions_tableView->verticalHeader()->setVisible(false);
    ui->transactions_tableView->setSortingEnabled(true);
    ui->transactions_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->transactions_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->transactions_tableView->show();

    // signals
    connect(ui->importCsv_pushButton, SIGNAL(clicked()), this, SLOT(onClicked_importCsv()));
    connect(ui->update_pushButton, SIGNAL(clicked()), db, SLOT(currencyPricesTable_startUpdate()));
}

Transactions::~Transactions()
{
    delete ui;
    delete import_d;
}

void Transactions::onClicked_importCsv(void)
{
    import_d = new ImportCsvDialog(this, db);
    import_d->open();
}


