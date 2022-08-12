#ifndef WIDGET_H
#define WIDGET_H

#include "controller/custommessagebox.h"
#include "structure/group.h"
#include "structure/groupType.h"
#include "widgets/framelesswidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public FramelessWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    bool getIsLogin() const;

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event)Q_DECL_OVERRIDE;
private:
    //加载用户数据
    void loadSettings();
    void loadGroups();
    void loadUserData();
    void loadGroupTypes();
    void loadAutofillInfo();
    void updateAutofillInfo();
    //界面刷新相关
    void initWidget();
    void refreshListView();
    void loadAccountPortrait();
    void loadWidget(QWidget* widget);
    //分组/分组类型相关
    void deleteGroup(const QString &groupName);
    void deleteGroupType(const QString &groupTypeName);
public slots:
    //登录登出等系统界面相关
    void openLocker();
    void loginReceive();
    void onLockClicked();
    void onCloseClicked();
    void onLogoutClicked();
    void onAboutClicked();
    void onSettingClicked();
    void onStickTopClicked();
    void onMinimizeClicked();
    void onMaximizeClicked();
    void closeLockerReceived();
    void onListViewClicked(const QModelIndex &index);
    //分组/分组类型相关
    void onGroupMoreClicked(const QString &name);
    void onAddGroupClicked(const QString &name);
    void onGroupTypeMoreClicked(const QString &name);
    void onAddGroupTypeClicked(const QString &name);
    void onNewGroupAccept(Group* group);
    void onEditGroupAccept(const QString &oldGroupName, Group* group);
    void onNewGroupTypeAccept(GroupType* groupType);
    void onEditGroupTypeAccept(const QString &oldGroupTypeName, GroupType* groupType);
    //条目/字段相关
    void onAddFieldClicked(const QString& name);
    void onAddKeyItemClicked(const QString& name);
    void onNewKeyItemAccept(const QString& groupName,KeyItem* newKeyItem);
    void onNewFieldAccept(const QString& groupTypeName, const FieldData &fieldData);
    void onRemoveKeyItemClicked();
    //信号处理相关
    void onSideButtonClicked();
    void onGroupCountChanged();
    void onGroupTypeCountChanged();
    void onGroupContentChanged();
    void onGroupTypeContentChanged();
    void onSettingsChanged();
signals:
    void groupCountChanged();
    void groupTypeCountChanged();
    void groupContentChanged();
    void groupTypeContentChanged();
private:
    Ui::Widget *ui;
    bool isLogin=false;
};
#endif // WIDGET_H
