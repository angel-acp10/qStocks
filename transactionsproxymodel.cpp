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
            QDateTime date;
            date.setSecsSinceEpoch(value.toInt());
            return date.toString("dd/MM/yyyy HH:mm");
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        switch(index.column())
        {
        case COL_Name:
        case COL_LocCurrency:
        case COL_Currency:
            return Qt::AlignLeft;
        case COL_UnitPrice:
        case COL_Value:
            return Qt::AlignRight;
        default:
            return Qt::AlignCenter;
        }
    }
    else if(role == Qt::BackgroundRole)
    {
        int qty;

        if(index.column() == COL_Quantity)
            qty = index.data().toDouble();
        else
            qty = index.siblingAtColumn(COL_Quantity).data().toDouble();

        if(qty>=0)  return QColor(143, 188, 143);  // buy - green
        else        return QColor(205, 92, 92); // sell - red
    }

    return value;
}

