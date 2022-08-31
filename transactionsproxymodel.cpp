#include "transactionsproxymodel.h"

#include "columns.h"
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
        if(index.column() == COL_TR_DateTime)
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
        case COL_TR_Name:
        case COL_TR_LocCurrency:
        case COL_TR_Currency:
            return Qt::AlignLeft;
        case COL_TR_UnitPrice:
        case COL_TR_Value:
            return Qt::AlignRight;
        default:
            return Qt::AlignCenter;
        }
    }
    else if(role == Qt::BackgroundRole)
    {
        int qty;

        if(index.column() == COL_TR_Quantity)
            qty = index.data().toDouble();
        else
            qty = index.siblingAtColumn(COL_TR_Quantity).data().toDouble();

        if(qty>=0)  return QColor(143, 188, 143);  // buy - green
        else        return QColor(205, 92, 92); // sell - red
    }

    return value;
}

