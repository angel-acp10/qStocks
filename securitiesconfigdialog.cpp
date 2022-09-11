#include "securitiesconfigdialog.h"
#include "ui_securitiesconfigdialog.h"
#include "columns.h"

SecuritiesConfigDialog::SecuritiesConfigDialog(QWidget *parent, DataBase *db) :
    QDialog(parent),
    ui(new Ui::SecuritiesConfigDialog)
{
    ui->setupUi(this);
    this->db = db;

    //////////////////////
    // Securities list
    //////////////////////
    // model
    QSqlDatabase database = QSqlDatabase::database();
    model = new QSqlRelationalTableModel(this, database);
    model->setTable("Securities");
    model->setRelation(COL_SEC_CurrencyID, QSqlRelation("Currencies", "ID", "Name"));
    model->setRelation(COL_SEC_ApiID, QSqlRelation("Apis", "ID", "Name"));
    model->select();
    ui->secList_tableView->setModel(model);

    // hidden columns
    QList<securitiesColumns_t> hiddenColumns ({
        COL_SEC_ID,
        COL_SEC_Notes
    });
    for(auto col : hiddenColumns)
        ui->secList_tableView->setColumnHidden(col, true);

    // other properties
    ui->secList_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->secList_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->secList_tableView->verticalHeader()->setVisible(false);
    ui->secList_tableView->setSortingEnabled(true);
    ui->secList_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->secList_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->secList_tableView->show();

    // signals
    connect(ui->secList_tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SecuritiesConfigDialog::onSelectionChanged_secList);

    //////////////////////
    // Apis
    //////////////////////
    yahoo = new YahooApi(this);

    // signals
    connect(yahoo, &YahooApi::received_SearchTicker,
            this, &SecuritiesConfigDialog::onReceived_searchTicker);
    connect(ui->ticker_lineEdit, &QLineEdit::textEdited,
            this, &SecuritiesConfigDialog::onTextEdited_ticker);

    // fill combobox apis
    for(int i=(int)DataBase::API_YAHOO; i<=db->totalApis; i++)
    {
        QString apiName = db->apiEnumToString((DataBase::ApiEnum)i);
        ui->api_comboBox->addItem(apiName);
    }

    //////////////////////
    // Matches list
    //////////////////////
    // header
    QStringList matches_header;
    matches_header<<"Exchanges"<<"Names"<<"Types"<<"Tickers";
    ui->matches_tableWidget->setColumnCount(matches_header.size());
    ui->matches_tableWidget->setHorizontalHeaderLabels(matches_header);

    // other properties
    ui->matches_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->matches_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->matches_tableWidget->verticalHeader()->setVisible(false);
    ui->matches_tableWidget->setSortingEnabled(true);
    ui->matches_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->matches_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // signals
    connect(ui->matches_tableWidget, &QTableWidget::itemSelectionChanged,
            this, &SecuritiesConfigDialog::onSelectionChanged_matches);

    /////////////////////
    // Update Button
    /////////////////////
    connect(ui->update_pushButton, &QPushButton::clicked,
            this, &SecuritiesConfigDialog::onClicked_update);
}

SecuritiesConfigDialog::~SecuritiesConfigDialog()
{
    delete ui;
    delete model;
    delete yahoo;
}

void SecuritiesConfigDialog::onSelectionChanged_secList(const QItemSelection & selected, const QItemSelection & deselected)
{
    (void)deselected;

    ui->matches_tableWidget->clearSelection();
    ui->matches_tableWidget->setRowCount(0);
    if(selected.indexes().isEmpty())
        return;

    m_selRow_secList = selected.indexes().first().row();
    QAbstractItemModel *m = ui->secList_tableView->model();

    QString isin = m->index(m_selRow_secList, COL_SEC_Isin).data().toString();
    QString name = m->index(m_selRow_secList, COL_SEC_Name).data().toString();
    QString currency = m->index(m_selRow_secList, COL_SEC_CurrencyID).data().toString();
    QString ticker = m->index(m_selRow_secList, COL_SEC_Ticker).data().toString();
    QString api = m->index(m_selRow_secList, COL_SEC_ApiID).data().toString();
    QString apiTicker = m->index(m_selRow_secList, COL_SEC_ApiTicker).data().toString();

    ui->isin_lineEdit->setText(isin);
    ui->name_lineEdit->setText(name);
    ui->locCurr_lineEdit->setText(currency);
    ui->ticker_lineEdit->setText(ticker);
    ui->api_comboBox->setCurrentText(api);
    ui->apiTicker_lineEdit->setText(apiTicker);

    onTextEdited_ticker(ticker);
}

void SecuritiesConfigDialog::onReceived_searchTicker(const QStringList &exchanges,
                                                    const QStringList &names,
                                                    const QStringList &types,
                                                    const QStringList &tickers  )
{
    if(exchanges.isEmpty())
    {
        ui->matches_tableWidget->setRowCount(1);
        ui->matches_tableWidget->setItem(0, 0, new QTableWidgetItem(""));
        ui->matches_tableWidget->setItem(0, 1, new QTableWidgetItem("Not Found"));
        ui->matches_tableWidget->setItem(0, 2, new QTableWidgetItem(""));
        ui->matches_tableWidget->setItem(0, 3, new QTableWidgetItem(""));
        return;
    }
    
    ui->matches_tableWidget->setRowCount(exchanges.size());
    for(int i=0; i<exchanges.size(); i++)
    {
        ui->matches_tableWidget->setItem(i, 0, new QTableWidgetItem(exchanges.at(i)));
        ui->matches_tableWidget->setItem(i, 1, new QTableWidgetItem(names.at(i)));
        ui->matches_tableWidget->setItem(i, 2, new QTableWidgetItem(types.at(i)));
        ui->matches_tableWidget->setItem(i, 3, new QTableWidgetItem(tickers.at(i)));
    }
}

void SecuritiesConfigDialog::onSelectionChanged_matches()
{
    QList<QTableWidgetItem*> selected = ui->matches_tableWidget->selectedItems();
    if(selected.isEmpty())
        return;

    m_selRow_matches = selected.first()->row();
    QString apiTicker = ui->matches_tableWidget->item(m_selRow_matches, 3)->text();
    ui->apiTicker_lineEdit->setText(apiTicker);
}

void SecuritiesConfigDialog::onTextEdited_ticker(const QString &text)
{
    QAbstractItemModel *m = ui->secList_tableView->model();
    QString api = m->index(m_selRow_secList, COL_SEC_ApiID).data().toString();

    ui->matches_tableWidget->clearSelection();
    ui->ticker_lineEdit->setText(text.toUpper());
    if(text == "")
        return;

    switch(db->stringToApiEnum(api))
    {
    case DataBase::API_YAHOO:
        yahoo->searchTicker(text);
        break;

    default:
        break;
    }
}

void SecuritiesConfigDialog::onClicked_update()
{
    QAbstractItemModel *m = ui->secList_tableView->model();

    QString ticker = ui->ticker_lineEdit->text();
    QModelIndex idx_ticker = m->index(m_selRow_secList, COL_SEC_Ticker);
    m->setData(idx_ticker, ticker);

    QString api = ui->api_comboBox->currentText();
    QModelIndex idx_api = m->index(m_selRow_secList, COL_SEC_ApiID);
    m->setData(idx_api, api);

    QString apiTicker = ui->apiTicker_lineEdit->text();
    QModelIndex idx_apiTicker = m->index(m_selRow_secList, COL_SEC_ApiTicker);
    m->setData(idx_apiTicker, apiTicker);

    m->submit();

    emit m->dataChanged(idx_ticker, idx_apiTicker);
}
