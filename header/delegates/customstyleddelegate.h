#ifndef CUSTOMSTYLEDDELEGATE_H
#define CUSTOMSTYLEDDELEGATE_H

#include <QStyledItemDelegate>

class CustomStyledDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustomStyledDelegate(QObject *parent = nullptr);
private:
    void paint(QPainter *painter,
            const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CUSTOMSTYLEDDELEGATE_H
