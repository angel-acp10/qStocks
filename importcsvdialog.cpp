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
    QFile f = QFile(csvFile);
    if (!f.open(QIODevice::ReadOnly))
    {
        QErrorMessage err = QErrorMessage(this);
        err.showMessage(tr("File could not be opened."));
        return;
    }

    f.readLine(); // ignores header
    while(!f.atEnd())
    {
        QList row = f.readLine().split(',');
        QDate date = QDate::fromString(row[0], "dd-MM-yyyy");
        QTime time = QTime::fromString(row[1], "HH:mm");

        DataBase::transaction_t transaction = {
            .dateTime = QDateTime(date, time),
            .product = row[2],
            .ticker = "",
            .isin = row[3],
            .market = row[4],
            .qty = row[6].toInt(),
            .locUnitPrice = row[7].toDouble(),
            .locCurrency = row[8],
            .currency = row[12],
            .exchRate = row[13].toDouble(),
            .commissions = row[14].toDouble(),
        };

        db->addTransaction(transaction);
    }
    return;
}
