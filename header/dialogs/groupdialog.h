#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H
#include "customField/customComboBox.h"
#include "customField/customLineEdit.h"
#include "structure/group.h"
#include <QWidget>

namespace Ui {
class GroupDialog;
}

class GroupDialog : public QWidget
{
    Q_OBJECT

public:
    enum mode{
        NEWGROUP,
        EDITGROUP,
        COPYGROUP
    };
    Group* newGroup;
    mode currentMode;
    QString oldGroupName;
public:
    explicit GroupDialog(mode currentMode,QWidget *parent = nullptr);
    void setWindowCustomTitle(const QString& newWindowTitle);
    ~GroupDialog();
    void loadGroup(Group* group);
    void setCurrentMode(mode newCurrentMode);
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void initWidget();
private:
    Ui::GroupDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
    customComboBox* groupType;
    customLineEdit* groupName;
    QString styleSheet;
private slots:
    void onCancelClicked();
    void onConfirmClicked();
    void onCloseClicked();
    void onMinimizeClicked();
    void onNcActived(bool active);
signals:
    void ncActive(bool);
    void newGroupAccept(Group* group);
    void editGroupAccept(const QString& oldGroupName,Group* group);
};

#endif // GROUPDIALOG_H
