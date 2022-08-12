#ifndef GROUPS_H
#define GROUPS_H
#include "group.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
class Groups{
private:
    QString groupsName="分组管理";
    QList<Group*> groupList;
public:
    Groups();
    //基本操作
    int count();
    void clear();
    bool has(const QString &groupName);
    int index(const QString &groupName);
    Group* at(int index);
    bool removeAt(int i);
    void append(Group* newGroup);
    bool removeOne(const QString &groupName);
    bool remove(const QStringList &groupNames);
    //重载运算符
    Group* operator[](int index);
    Groups* operator<<(Group* newGroup);
    //获取分组名称列表
    QStringList getGroupNames();
    //获取列表信息
    QStringList getListViewInfo();
};
#endif // GROUPS_H
