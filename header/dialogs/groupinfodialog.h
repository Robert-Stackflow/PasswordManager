#ifndef GROUPINFODIALOG_H
#define GROUPINFODIALOG_H

#include "customField/customComboBox.h"
#include "customField/customLineEdit.h"
#include "structure/group.h"
#include <QWidget>

namespace Ui {
class GroupInfoDialog;
}

class GroupInfoDialog : public QWidget
{
    Q_OBJECT

public:
    explicit GroupInfoDialog(QWidget *parent = nullptr);
    ~GroupInfoDialog();
    void setWindowCustomTitle(const QString& newWindowTitle);
    void loadGroup(Group* group);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void initWidget();
private:
    Ui::GroupInfoDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
    QString styleSheet;
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

#endif // GROUPINFODIALOG_H
