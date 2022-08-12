#include "delegates/customstyleddelegate.h"

CustomStyledDelegate::CustomStyledDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

void CustomStyledDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, viewOption, index);
}
