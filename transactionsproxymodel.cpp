#include "transactionsproxymodel.h"

#include "transactionscolumns.h"
#include <QDateTime>
#include <QColor>

TransactionsProxyModel::TransactionsProxyModel()
{
}

QVariant TransactionsProxyModel::data(const QModelIndex &index, int role) const
{
    const QVariant value =  QIdentityProxyModel::data(index, role);

    if(role == Qt::DisplayRole)
    {
        if(index.column() == COL_DateTime)
        {
            QDateTime date = QDateTime(value.toDateTime());
            return date.toString("dd/MM/yyyy HH:mm");
        }
        else if (index.column() == COL_LocalUnitPrice)
        {
            const double locUtPrice = value.toDouble();
            const QString locCurrency = index.siblingAtColumn(COL_LocalCurrency).data(role).toString();
            QString formated = QString("%1  %2  ").arg(locUtPrice).arg(locCurrency);
            return formated;
        }
        else if (index.column() == COL_Total) {
            const double total = value.toDouble();
            const QString currency = index.siblingAtColumn(COL_Currency).data(role).toString();
            QString formated = QString("%1  %2  ").arg(total).arg(currency);
            return formated;
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        switch(index.column())
        {
        case COL_Product:
        case COL_Isin:
            return Qt::AlignLeft;
        case COL_LocalUnitPrice:
        case COL_Total:
            return Qt::AlignRight;
        default:
            return Qt::AlignCenter;
        }
    }
    else if(role == Qt::BackgroundRole)
    {
        int qty;

        if(index.column() == COL_Quantity)
            qty = index.data().toInt();
        else
            qty = index.siblingAtColumn(COL_Quantity).data().toInt();

        if(qty>=0)  return QColor(143, 188, 143);  // buy - green
        else        return QColor(205, 92, 92); // sell - red
    }

    return value;
}

