#include "portfolio.h"
#include "ui_portfolio.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>


Portfolio::Portfolio(QWidget *parent, DataBase *db) :
    QWidget(parent),
    ui(new Ui::Portfolio)
{
    ui->setupUi(this);
    this->db = db;

    QVector<int> secId;
    QVector<QString> ticker;
    QVector<double> quantity;
    QVector<double> invValue;
    QVector<double> comissions;
    QVector<int> currId;
    QVector<double> lastPrice;
    QVector<int> locCurrId;

    db->currentPortfolioQuery_get(secId,
                              ticker,
                              quantity,
                              invValue,
                              comissions,
                              currId,
                              lastPrice,
                              locCurrId);

    QPieSeries *series = new QPieSeries();
    series->setName("Securities");
    for(int i = 0; i<secId.size(); i++)
        series->append(ticker[i], -invValue[i]);

    for(int i=0; i<series->slices().size(); i++)
        series->slices().at(i)->setLabelVisible();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Securities");
    chart->legend()->hide();

    ui->portfolio_chartView->setChart(chart);
    ui->portfolio_chartView->setRenderHint(QPainter::Antialiasing);
}

Portfolio::~Portfolio()
{
    delete ui;
}
