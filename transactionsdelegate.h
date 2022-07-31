#ifndef TRANSACTIONSDELEGATE_H
#define TRANSACTIONSDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class TransactionsDelegate : public QStyledItemDelegate
{
public:
    explicit TransactionsDelegate(QObject *parent = nullptr);
    ~TransactionsDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    //QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    //void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // TRANSACTIONSDELEGATE_H

/*
class MemoryDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    const memoryData_t data[MEMDATA_SIZE]={
        //block,str,end, field,         format
        {   0,  0,  0,  FMT_UINT8,  "Versión datos"},
        {   0,  1,  1,  FMT_UINT8,  "Versión de electrodo"},
        {   0,  2,  5,  FMT_4CHAR,  "Tipo de electrodo interno"},
        {   0,  6,  6,  FMT_UINT8,  "Método de encriptación"},
        {   0,  7,  7,  FMT_CSUM,  "CSUM"},

        {   1,  0,  6,  FMT_7CHAR,  "Lote/Núm. Serie [0-6]"},
        {   1,  7,  7,  FMT_CSUM,  "CSUM"},

        {   2,  0,  0,  FMT_1CHAR,  "Lote/Núm. Serie [7]"},
        {   2,  3,  4,  FMT_UINT16, "Tipo de uso (caso dual)"},
        {   2,  5,  6,  FMT_TEMP,  "Offset de temperatura"},
        {   2,  7,  7,  FMT_CSUM,  "CSUM"},

        {   3,  0,  6,  FMT_7CHAR,  "Ref. electrodo [0-6]"},
        {   3,  7,  7,  FMT_CSUM,  "CSUM"},

        {   4,  0,  0,  FMT_1CHAR,  "Ref. electrodo [7]"},
        {   4,  7,  7,  FMT_CSUM,  "CSUM"},

        {   5,  7,  7,  FMT_CSUM,  "CSUM"},

        {   6,  0,  2,  FMT_DATE,   "Fecha primer uso"},
        {   6,  7,  7,  FMT_CSUM,  "CSUM"},

        {   7,  7,  7,  FMT_CSUM,  "CSUM"},

        {   8,  0,  2,  FMT_DATE,   "Fecha último uso"},
        {   8,  3,  6,  FMT_UINT32, "Número de minutos"},
        {   8,  7,  7,  FMT_CSUM,  "CSUM"},

        {   9,  0,  3,  FMT_UINT32, "Energía acumulada"},
        {   9,  4,  5,  FMT_TEMP,  "Temperatura máxima"},
        {   9,  7,  7,  FMT_CSUM,  "CSUM"},

        {   10, 0,  1,  FMT_UINT16, "Status electrodo"},
        {   10, 7,  7,  FMT_CSUM,  "CSUM"},

        {   11, 7,  7,  FMT_CSUM,  "CSUM"},

        {   12,  0,  2,  FMT_DATE,   "Fecha último uso"},
        {   12,  3,  6,  FMT_UINT32, "Número de minutos"},
        {   12,  7,  7,  FMT_CSUM,  "CSUM"},

        {   13,  0,  3,  FMT_UINT32, "Energía acumulada"},
        {   13,  4,  5,  FMT_TEMP,  "Temperatura máxima"},
        {   13,  7,  7,  FMT_CSUM,  "CSUM"},

        {   14, 0,  1,  FMT_UINT16, "Status electrodo"},
        {   14, 7,  7,  FMT_CSUM,  "CSUM"},

        {   15, 7,  7,  FMT_CSUM,  "CSUM"},
    };


    explicit MemoryDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
signals:

public slots:

};

#endif // MEMORYDELEGATE_H

*/
