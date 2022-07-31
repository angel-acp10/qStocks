#include "purchsales.h"
#include "ui_purchsales.h"

PurchSales::PurchSales(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::PurchSales)
{
    ui->setupUi(this);
    this->db = db;
}

PurchSales::~PurchSales()
{
    delete ui;
}
