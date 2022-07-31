#ifndef TRANSACTIONSPROXYMODEL_H
#define TRANSACTIONSPROXYMODEL_H

#include <QObject>
#include <QIdentityProxyModel>

class TransactionsProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    TransactionsProxyModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // TRANSACTIONSPROXYMODEL_H
