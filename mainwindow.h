#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "database.h"
#include "settingsdialog.h"
#include "securitiesconfigdialog.h"
#include "transactions.h"
#include "portfolio.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOptions_triggered();

    void on_actionSecurities_configuration_triggered();

private:
    Ui::MainWindow *ui;

    SettingsDialog *settings_d;
    SecuritiesConfigDialog *securitiesConfig_d;

    Transactions *transactions_w;
    Portfolio *portfolio_w;

    DataBase *db;
};
#endif // MAINWINDOW_H
