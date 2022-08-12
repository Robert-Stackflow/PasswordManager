#ifndef GROUP_H
#define GROUP_H
#include "keyItem.h"
class Group{
private:
    QString groupTypeName;
    QString groupName;
    QString describe;
    QDateTime createTime;
    QDateTime lastEditTime;
    QList<KeyItem*> keyItemList;
public:
    Group();
    Group(const QString& groupTypeName,const QString& groupName,const QString& describe);
    Group(const QString& groupTypeName,const QString& groupName,QDateTime createTime,QDateTime lastEditTime,const QString& describe);
    //Getter与Setter
    int getGroupType() const;
    const QString &getGroupName() const;
    void setGroupName(const QString &newGroupName);
    const QString &getDescribe() const;
    void setDescribe(const QString &newDescribe);
    const QDateTime &getCreateTime() const;
    const QDateTime &getLastEditTime() const;
    void setLastEditTime();
    //基本操作
    int count();
    void clear();
    Group* clone();
    bool removeAt(int i);
    KeyItem* at(int index);
    void append(KeyItem* newKeyItem);
    //重载运算符
    Group* operator<<(KeyItem* newKeyItem);
    KeyItem* operator[](int index);
    const QString &getGroupTypeName() const;
    //获取列表信息
    QString getListViewInfo();
};
#endif // GROUP_H
