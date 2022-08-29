#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // model
    QSqlDatabase database = QSqlDatabase::database();
    model = new QSqlTableModel(this, database);
    model->setTable("Settings");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    ui->settings_tableView->setModel(model);
    ui->settings_tableView->show();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
