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

        DataBase::transaction_t tr;
        tr.dateTime = QDateTime(date, time);
        tr.security.name = row[2];
        tr.security.ticker = "";
        tr.security.id = row[3];
        tr.qty = row[6].toInt();
        tr.locUnitPrice = row[7].toDouble();
        double locValue = -tr.qty * tr.locUnitPrice;
        tr.locCurrency = db->getCurrencyEnum(row[8]);
        tr.exchRate = (row[13].toDouble()==0.0) ? 1 : row[13].toDouble();
        tr.commissions = row[14].toDouble();
        tr.value = locValue / tr.exchRate;
        tr.total = tr.value + tr.commissions;

        tr.currency = db->getCurrencyEnum(row[12]);


        if(!db->transactionsTable_addTransaction(tr))
        {
            f.close();
            db->close();
            QString message = QString("Error adding transaction with date:%1 and product:%2").arg(
                        tr.dateTime.toString("dd-MM-yyyy HH:mm"),
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
