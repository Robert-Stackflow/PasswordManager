#ifndef GROUPTYPES_H
#define GROUPTYPES_H
#include "groupType.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
class GroupTypes{
private:
    QString groupTypesName="分组类型管理";
    QList<GroupType*> groupTypeList;
public:
    //基本属性
    int count();
    bool has(const QString& groupTypeName);
    int index(const QString& groupTypeName);
    GroupType* at(int index);
    void append(GroupType* newGroupType);
    bool removeOne(const QString& groupTypeName);
    bool remove(const QStringList& groupTypeNames);
    //重载运算符
    GroupType* operator[](int index);
    GroupTypes* operator<<(GroupType* newGroupType);
    //获取分组类型名称列表
    QStringList getGroupTypeNames();
    //获取列表信息
    QStringList getListViewInfo();
};
#endif // GROUPTYPES_H
