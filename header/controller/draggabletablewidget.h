#ifndef DRAGGABLETABLEWIDGET_H
#define DRAGGABLETABLEWIDGET_H
#include <QEvent>
#include <QTableWidget>
#include <QDropEvent>
#include <QHeaderView>
class draggableTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    draggableTableWidget(QWidget *parent = Q_NULLPTR);
protected:
    void dropEvent(QDropEvent *event);
};

#endif // DRAGGABLETABLEWIDGET_H
