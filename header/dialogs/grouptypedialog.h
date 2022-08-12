#ifndef GROUPTYPEDIALOG_H
#define GROUPTYPEDIALOG_H

#include "customField/customLineEdit.h"
#include "structure/groupType.h"
#include <QWidget>

namespace Ui {
class GroupTypeDialog;
}

class GroupTypeDialog : public QWidget
{
    Q_OBJECT
public:
    enum mode{
        NEWGROUPTYPE,
        EDITGROUPTYPE,
        COPYGROUPTYPE
    };
    GroupType* newGroupType;
    mode currentMode;
    QString oldGroupTypeName;
public:
    explicit GroupTypeDialog(mode currentMode,QWidget *parent = nullptr);
    void setWindowCustomTitle(const QString& newWindowTitle);
    ~GroupTypeDialog();
    void loadGroupType(GroupType* groupType);
    void setCurrentMode(mode newCurrentMode);
    bool getDataValid() const;
protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    bool nativeEvent(const QByteArray &, void *message, long *);
private:
    void initWidget();
private:
    Ui::GroupTypeDialog *ui;
    bool mousePressed;
    QPoint mousePoint;
    customLineEdit* groupTypeName;
    QString styleSheet;
private slots:
    void onCancelClicked();
    void onConfirmClicked();
    void onCloseClicked();
    void onMinimizeClicked();
    void onNcActived(bool active);
signals:
    void ncActive(bool);
    void newGroupTypeAccept(GroupType* groupType);
    void editGroupTypeAccept(const QString& oldGroupTypeName,GroupType* groupType);
};

#endif // GROUPTYPEDIALOG_H
