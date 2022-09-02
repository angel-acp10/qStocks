#ifndef SECURITIESCONFIGDIALOG_H
#define SECURITIESCONFIGDIALOG_H

#include <QDialog>
#include "database.h"
#include "yahooapi.h"

namespace Ui {
class SecuritiesConfigDialog;
}

class SecuritiesConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecuritiesConfigDialog(QWidget *parent, DataBase *db);
    ~SecuritiesConfigDialog();

private slots:
    void onSelectionChanged_secList(const QItemSelection & selected, const QItemSelection & deselected);
    void onReceived_searchTicker(const QStringList &exchanges,
                                 const QStringList &names,
                                 const QStringList &types,
                                 const QStringList &tickers );
    void onSelectionChanged_matches();
    void onTextEdited_ticker(const QString &text);
    void onClicked_update();



private:
    Ui::SecuritiesConfigDialog *ui;

    DataBase *db;

    QSqlRelationalTableModel *model;
    YahooApi *yahoo;

    int m_selRow_secList;
    int m_selRow_matches;
};

#endif // SECURITIESCONFIGDIALOG_H
