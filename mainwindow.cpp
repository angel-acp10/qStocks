#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new DataBase("C:/Qt-dev/qStocks/qStocks/db.sqlite");

    // set window title
    //------------------
    setWindowTitle("qStocks");

    // mainMenu initialisation
    //-------------------------
    typedef struct{
        QString label;
        QWidget * wdgPtr;
        const std::function<QWidget*()> memAlloc;
    } mainMenuItem_t;

    mainMenuItem_t items[] = {
        {"Transactions",        transactions_w, [this](){return new Transactions(this, db);} },
        {"Portfolio",           portfolio_w,    [this](){return new Portfolio(this, db);} },
        {"", NULL, NULL} // end of array
    };

    for(int i=0; items[i].wdgPtr!=NULL; i++)
    {
        ui->mainMenu_listWidget->addItem( QObject::tr(qPrintable(items[i].label)) );
        items[i].wdgPtr = items[i].memAlloc();
        ui->stackedWidget->addWidget( items[i].wdgPtr );
    }

    ui->mainMenu_listWidget->setCurrentRow(0);

    connect(ui->mainMenu_listWidget, &QListWidget::currentRowChanged,
            ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    //
    //---------------------------------

}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
}


void MainWindow::on_actionOptions_triggered()
{
    settings_d = new SettingsDialog(this);
    settings_d->exec();
}

