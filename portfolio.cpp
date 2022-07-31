#include "portfolio.h"
#include "ui_portfolio.h"

Portfolio::Portfolio(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::Portfolio)
{
    ui->setupUi(this);
    this->db = db;
}

Portfolio::~Portfolio()
{
    delete ui;
}
