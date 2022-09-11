#include "currenciesconfigdialog.h"
#include "ui_currenciesconfigdialog.h"
#include "columns.h"

CurrenciesConfigDialog::CurrenciesConfigDialog(QWidget *parent, DataBase *db) :
    QDialog(parent),
    ui(new Ui::CurrenciesConfigDialog)
{
    ui->setupUi(this);
    this->db = db;

    //////////////////////
    // Securities list
    //////////////////////
    // model
    QSqlDatabase database = QSqlDatabase::database();
    model = new QSqlRelationalTableModel(this, database);
    model->setTable("Currencies");
    model->setRelation(COL_CUR_ApiID, QSqlRelation("Apis", "ID", "Name"));
    model->select();
    ui->curList_tableView->setModel(model);

    // hidden columns
    QList<currenciesColumns_t> hiddenColumns ({
        COL_CUR_ID,
    });
    for(auto col : hiddenColumns)
        ui->curList_tableView->setColumnHidden(col, true);

    // other properties
    ui->curList_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->curList_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->curList_tableView->verticalHeader()->setVisible(false);
    ui->curList_tableView->setSortingEnabled(true);
    ui->curList_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->curList_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->curList_tableView->show();

    // signals
    connect(ui->curList_tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &CurrenciesConfigDialog::onSelectionChanged_secList);

    //////////////////////
    // Apis
    //////////////////////
    yahoo = new YahooApi(this);

    // signals
    connect(yahoo, &YahooApi::received_SearchTicker,
            this, &CurrenciesConfigDialog::onReceived_searchTicker);
    connect(ui->search_lineEdit, &QLineEdit::textEdited,
            this, &CurrenciesConfigDialog::onTextEdited_search);

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
            this, &CurrenciesConfigDialog::onSelectionChanged_matches);

    /////////////////////
    // Update Button
    /////////////////////
    connect(ui->update_pushButton, &QPushButton::clicked,
            this, &CurrenciesConfigDialog::onClicked_update);
}

CurrenciesConfigDialog::~CurrenciesConfigDialog()
{
    delete ui;
    delete model;
    delete yahoo;
}


void CurrenciesConfigDialog::onSelectionChanged_secList(const QItemSelection & selected, const QItemSelection & deselected)
{
    (void)deselected;

    ui->matches_tableWidget->clearSelection();
    ui->matches_tableWidget->setRowCount(0);
    if(selected.indexes().isEmpty())
        return;

    m_selRow_secList = selected.indexes().first().row();
    QAbstractItemModel *m = ui->curList_tableView->model();

    QString name = m->index(m_selRow_secList, COL_CUR_Name).data().toString();
    QString api = m->index(m_selRow_secList, COL_CUR_ApiID).data().toString();
    QString apiTicker = m->index(m_selRow_secList, COL_CUR_ApiTicker).data().toString();

    ui->name_lineEdit->setText(name);
    ui->search_lineEdit->setText("");
    ui->api_comboBox->setCurrentText(api);
    ui->apiTicker_lineEdit->setText(apiTicker);

    onTextEdited_search("");
}

void CurrenciesConfigDialog::onReceived_searchTicker(const QStringList &exchanges,
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

void CurrenciesConfigDialog::onSelectionChanged_matches()
{
    QList<QTableWidgetItem*> selected = ui->matches_tableWidget->selectedItems();
    if(selected.isEmpty())
        return;

    m_selRow_matches = selected.first()->row();
    QString apiTicker = ui->matches_tableWidget->item(m_selRow_matches, 3)->text();
    ui->apiTicker_lineEdit->setText(apiTicker);
}

void CurrenciesConfigDialog::onTextEdited_search(const QString &text)
{
    QAbstractItemModel *m = ui->curList_tableView->model();
    QString api = m->index(m_selRow_secList, COL_CUR_ApiID).data().toString();

    ui->matches_tableWidget->clearSelection();
    ui->search_lineEdit->setText(text.toUpper());
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

void CurrenciesConfigDialog::onClicked_update()
{
    QAbstractItemModel *m = ui->curList_tableView->model();

    QString api = ui->api_comboBox->currentText();
    QModelIndex idx_api = m->index(m_selRow_secList, COL_CUR_ApiID);
    m->setData(idx_api, api);

    QString apiTicker = ui->apiTicker_lineEdit->text();
    QModelIndex idx_apiTicker = m->index(m_selRow_secList, COL_CUR_ApiTicker);
    m->setData(idx_apiTicker, apiTicker);

    m->submit();

    emit m->dataChanged(idx_api, idx_apiTicker);
}
