#include "importcsvdialog.h"
#include "ui_importcsvdialog.h"

#include <QFileDialog>
#include <QErrorMessage>

ImportCsvDialog::ImportCsvDialog(QWidget *parent, DataBase *db) :
    QDialog(parent),
    ui(new Ui::ImportCsvDialog)
{
    ui->setupUi(this);
    this->db = db;

    ui->broker_comboBox->addItem("Degiro");

    connect(ui->select_pushButton, SIGNAL(clicked()), this, SLOT(onClicked_select()));
    connect(ui->import_pushButton, SIGNAL(clicked()), this, SLOT(onClicked_import()));
}

ImportCsvDialog::~ImportCsvDialog()
{
    delete ui;
}

void ImportCsvDialog::onClicked_select(void)
{
    QString file;
    file = QFileDialog::getOpenFileName(this, tr("Import file..."),  "", tr("Comma Separated Values (*.csv)") );
    ui->file_lineEdit->setText(file);
}

void ImportCsvDialog::onClicked_import(void)
{
    QString broker;
    QString file;

    broker = ui->broker_comboBox->currentText();
    file = ui->file_lineEdit->text();

    if(file == "")
    {
        QErrorMessage err = QErrorMessage(this);
        err.showMessage(tr("No file was selected."));
        return;
    }

    if(broker.toLower() == "degiro")
    {
        importCsv_degiro(file);
    }
}

void ImportCsvDialog::importCsv_degiro(QString csvFile)
{
    QErrorMessage err = QErrorMessage(this);
    QFile f = QFile(csvFile);

    if (!f.open(QIODevice::ReadOnly))
    {
        err.showMessage(tr("File could not be opened."));
        err.exec();
        return;
    }

    if(!db->open())
    {
        f.close();
        err.showMessage(tr("DataBase could not be opened."));
        err.exec();
        return;
    }

    f.readLine(); // ignores header
    while(!f.atEnd())
    {
        QList row = f.readLine().split(',');
        QDate date = QDate::fromString(row[0], "dd-MM-yyyy");
        QTime time = QTime::fromString(row[1], "HH:mm");

        // transaction_t
        DataBase::transaction_t tr;
        tr.id = -1; // autoincrement
        tr.timeStamp = QDateTime(date, time).toSecsSinceEpoch();
        tr.qty = row[6].toDouble();
        tr.locUnitPrice = row[7].toDouble();
        tr.exchRate = (row[13].toDouble()==0.0) ? 1 : row[13].toDouble();
        double locValue = -tr.qty * tr.locUnitPrice;
        tr.value = locValue / tr.exchRate;
        tr.commissions = row[14].toDouble();
        tr.brokerId = DataBase::BROKER_DEGIRO;

        // security
        tr.security.id = -1; // autoincrement
        tr.security.isin = row[3];
        tr.security.name = row[2];
        tr.security.ticker = ""; // manually set by user
        tr.security.notes = ""; // manually set by user
        tr.security.apiId = DataBase::API_YAHOO;
        tr.security.apiTicker = ""; // manually set by user

        // security - currency
        tr.security.currency.id = -1; // autoincrement
        tr.security.currency.name = row[8].toUpper();
        tr.security.currency.apiId = DataBase::API_YAHOO;
        tr.security.currency.apiTicker = ""; // manually set by user

        // currency (used by user)
        tr.currency.id = -1; // autoincrement
        tr.currency.name = row[12].toUpper();
        tr.currency.apiId = DataBase::API_YAHOO;
        tr.currency.apiTicker = ""; // manually set by user

        if(!db->transactionsTable_addRecord(tr))
        {
            f.close();
            db->close();
            QString message = QString("Error adding transaction with date:%1 and product:%2").arg(
                        QDateTime(date, time).toString("dd-MM-yyyy HH:mm"),
                        tr.security.name);
            err.showMessage(message);
            err.exec();
            return;
        }
    }
    f.close();
    db->close();
    err.exec();
    return;
}
