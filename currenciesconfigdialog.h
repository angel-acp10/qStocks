#ifndef CURRENCIESCONFIGDIALOG_H
#define CURRENCIESCONFIGDIALOG_H

#include <QDialog>
#include "database.h"
#include "yahooapi.h"

namespace Ui {
class CurrenciesConfigDialog;
}

class CurrenciesConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CurrenciesConfigDialog(QWidget *parent, DataBase *db);
    ~CurrenciesConfigDialog();

private slots:
    void onSelectionChanged_secList(const QItemSelection & selected, const QItemSelection & deselected);
    void onReceived_searchTicker(const QStringList &exchanges,
                                 const QStringList &names,
                                 const QStringList &types,
                                 const QStringList &tickers );
    void onSelectionChanged_matches();
    void onTextEdited_search(const QString &text);
    void onClicked_update();



private:
    Ui::CurrenciesConfigDialog *ui;

    DataBase *db;

    QSqlRelationalTableModel *model;
    YahooApi *yahoo;

    int m_selRow_secList;
    int m_selRow_matches;
};

#endif // CURRENCIESCONFIGDIALOG_H
