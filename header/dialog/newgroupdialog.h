#ifndef NEWGROUPDIALOG_H
#define NEWGROUPDIALOG_H
#include <QDialog>
#include "structure/group.h"
#include "controller/customTitleBar.h"
#include "customField/customComboBox.h"
#include "customField/customLineEdit.h"
namespace Ui {
class newGroupDialog;
}

class newGroupDialog : public QDialog
{
    Q_OBJECT
public:
    enum mode{
        NEWGROUP,
        EDITGROUP
    };
public:
    Ui::newGroupDialog *ui;
    //自定义控件
    CustomTitleBar *m_titleBar;
    customLineEdit* newGroupName;
    customComboBox* newGroupType;
    //辅助变量，便于不同函数间传递数据
    Group* newGroup;
    mode currentMode;
private:
    bool dataValid=false;
    QString styleSheet;
public:
    explicit newGroupDialog(QWidget *parent = nullptr);
    ~newGroupDialog();
    void setWindowTitle(const QString& title);
    void setCurrentMode(mode newCurrentMode);
    bool getDataValid() const;
    void setDataValid(bool newDataValid);

private:
    void InitDialog();
private slots:
    void onCancelClicked();
    void onConfirmClicked();
    void onJumpToGroupTypeManagerClicked();
};

#endif // NEWGROUPDIALOG_H
