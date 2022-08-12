#ifndef GROUPTYPEINFODIALOG_H
#define GROUPTYPEINFODIALOG_H

#include "structure/groupType.h"
#include <QWidget>

namespace Ui {
class GroupTypeInfoDialog;
}

class GroupTypeInfoDialog : public QWidget
{
    Q_OBJECT

public:
    explicit GroupTypeInfoDialog(QWidget *parent = nullptr);
    ~GroupTypeInfoDialog();
    void setWindowCustomTitle(const QString& newWindowTitle);
    void loadGroupType(GroupType* groupType);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void initWidget();
private:
    Ui::GroupTypeInfoDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
private slots:
    void onButtonsClicked();
    void onCloseClicked();
    void onMinimizeClicked();
    void onNcActived(bool active);
signals:
    void ncActive(bool);
    void copyClicked();
    void editClicked();
    void deleteClicked();
};

#endif // GROUPTYPEINFODIALOG_H
