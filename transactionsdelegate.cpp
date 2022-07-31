#include "transactionsdelegate.h"

#include <QPalette>
#include <QDoubleSpinBox>
#include <QDateTime>

#include "transactionscolumns.h"

TransactionsDelegate::TransactionsDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

TransactionsDelegate::~TransactionsDelegate()
{

}

void TransactionsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int qty;
    QColor color;

    if(index.column() == COL_Quantity)
        qty = index.data().toInt();
    else
        qty = index.siblingAtColumn(COL_Quantity).data().toInt();

    if(qty>=0)  color = QColor(143, 188, 143);  // buy - green
    else        color = QColor(205, 92, 92); // sell - red

    painter->fillRect(option.rect, color);
    QStyledItemDelegate::paint(painter, option, index);
}


//QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
/*void TransactionsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}*/

/*
 * #include "memorydelegate.h"
#include <cstdint>

MemoryDelegate::MemoryDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *MemoryDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSpinBox *sb_editor;
    QDoubleSpinBox *dsb_editor;
    QDateEdit *d_editor;
    QLineEdit *l_editor;

    for(int i=0; i<MEMDATA_SIZE; i++)
    {
        int r = 8*data[i].block + data[i].str;
        if(index.row() == r)
        {
            switch(data[i].format)
            {
            case FMT_UINT8:  case FMT_INT8:
            case FMT_UINT16: case FMT_INT16:
            case FMT_UINT32: case FMT_INT32:
                sb_editor = new QSpinBox(parent);   break;
            case FMT_DATE:
                d_editor = new QDateEdit(parent);   break;
            case FMT_1CHAR: case FMT_2CHAR:
            case FMT_3CHAR: case FMT_4CHAR:
            case FMT_5CHAR: case FMT_6CHAR:
            case FMT_7CHAR: case FMT_8CHAR:
                l_editor = new QLineEdit(parent);   break;
            case FMT_TEMP:
                dsb_editor = new QDoubleSpinBox(parent);   break;
            }
            switch(data[i].format)
            {
            case FMT_UINT8:
                sb_editor->setMinimum(0);
                sb_editor->setMaximum(UINT8_MAX);
                return sb_editor;
            case FMT_INT8:
                sb_editor->setMinimum(INT8_MIN);
                sb_editor->setMaximum(INT8_MAX);
                return sb_editor;
            case FMT_UINT16:
                sb_editor->setMinimum(0);
                sb_editor->setMaximum(UINT16_MAX);
                return sb_editor;
            case FMT_INT16:
                sb_editor->setMinimum(INT16_MIN);
                sb_editor->setMaximum(INT16_MAX);
                return sb_editor;
            case FMT_UINT32:
                sb_editor->setMinimum(0);
                sb_editor->setMaximum(UINT32_MAX);
                return sb_editor;
            case FMT_INT32:
                sb_editor->setMinimum(INT32_MIN);
                sb_editor->setMaximum(INT32_MAX);
                return sb_editor;

            case FMT_DATE:
                d_editor->setDateRange( QDate(2000,1,1), QDate(10000,12,31) );
                return d_editor;

            case FMT_1CHAR: l_editor->setMaxLength(1); return l_editor;
            case FMT_2CHAR: l_editor->setMaxLength(2); return l_editor;
            case FMT_3CHAR: l_editor->setMaxLength(3); return l_editor;
            case FMT_4CHAR: l_editor->setMaxLength(4); return l_editor;
            case FMT_5CHAR: l_editor->setMaxLength(5); return l_editor;
            case FMT_6CHAR: l_editor->setMaxLength(6); return l_editor;
            case FMT_7CHAR: l_editor->setMaxLength(7); return l_editor;
            case FMT_8CHAR: l_editor->setMaxLength(8); return l_editor;

            case FMT_TEMP:
                dsb_editor->setDecimals(1);
                dsb_editor->setMaximum(100.0);
                dsb_editor->setMinimum(-100.0);
                return dsb_editor;
            }
        }
    }
}

//void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

void MemoryDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value;
    QDate date;
    QString string;
    double number;

    QSpinBox *spinbox;
    QDateEdit *dateEdit;
    QLineEdit *lineEdit;
    QDoubleSpinBox *d_spinbox;

    for(int i=0; i<MEMDATA_SIZE; i++)
    {
        int r = 8*data[i].block + data[i].str;
        if(index.row() == r)
        {
            switch(data[i].format)
            {
            case FMT_UINT8:  case FMT_INT8:
            case FMT_UINT16: case FMT_INT16:
            case FMT_UINT32: case FMT_INT32:
                value = index.model()->data(index,Qt::EditRole).toInt();
                spinbox = static_cast<QSpinBox*>(editor);
                spinbox->setValue(value);
                return;

            case FMT_DATE:
                date = index.model()->data(index,Qt::EditRole).toDate();
                dateEdit = static_cast<QDateEdit*>(editor);
                dateEdit->setDate(date);
                return;

            case FMT_1CHAR: case FMT_2CHAR:
            case FMT_3CHAR: case FMT_4CHAR:
            case FMT_5CHAR: case FMT_6CHAR:
            case FMT_7CHAR: case FMT_8CHAR:
                string = index.model()->data(index, Qt::EditRole).toString();
                lineEdit = static_cast<QLineEdit*>(editor);
                lineEdit->setText(string);
                return;

            case FMT_TEMP:
                number = index.model()->data(index,Qt::EditRole).toDouble();
                d_spinbox = static_cast<QDoubleSpinBox*>(editor);
                d_spinbox->setValue(number);
                return;
            }
        }
    }


}

void MemoryDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinbox;
    QDateEdit *dateEdit;
    QLineEdit *lineEdit;
    QDoubleSpinBox *d_spinbox;

    int value;
    QDate date;
    QString string;
    double number;


    for(int i=0; i<MEMDATA_SIZE; i++)
    {
        int r = 8*data[i].block + data[i].str;
        if(index.row() == r)
        {
            switch(data[i].format)
            {
            case FMT_UINT8:  case FMT_INT8:
            case FMT_UINT16: case FMT_INT16:
            case FMT_UINT32: case FMT_INT32:
                spinbox = static_cast<QSpinBox*>(editor);
                spinbox->interpretText();
                value = spinbox->value();
                model->setData(index,value, Qt::EditRole);
                return;

            case FMT_DATE:
                dateEdit = static_cast<QDateEdit*>(editor);
                dateEdit->interpretText();
                date = dateEdit->date();
                model->setData(index, date, Qt::EditRole);
                return;

            case FMT_1CHAR: case FMT_2CHAR:
            case FMT_3CHAR: case FMT_4CHAR:
            case FMT_5CHAR: case FMT_6CHAR:
            case FMT_7CHAR: case FMT_8CHAR:
                lineEdit = static_cast<QLineEdit*>(editor);
                string = lineEdit->text();
                model->setData(index, string, Qt::EditRole);
                return;

            case FMT_TEMP:
                d_spinbox = static_cast<QDoubleSpinBox*>(editor);
                d_spinbox->interpretText();
                number = d_spinbox->value();
                model->setData(index, number, Qt::EditRole);
                return;
            }
        }
    }


}

//QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
void MemoryDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
*/
