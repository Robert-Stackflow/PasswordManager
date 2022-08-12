#include "structure/group.h"
#include "qdebug.h"
#include "util/settinghelper.h"
#include "util/shareddatahelper.h"
#include <QDateTime>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
const QString &Group::getGroupTypeName() const
{
    return groupTypeName;
}

Group::Group(){
    this->createTime=QDateTime::currentDateTime();
    this->lastEditTime=QDateTime::currentDateTime();
}
Group::Group(const QString& groupTypeName,const QString& groupName,const QString& describe){
    this->groupTypeName=groupTypeName;
    this->groupName=groupName;
    this->createTime=QDateTime::currentDateTime();
    this->lastEditTime=QDateTime::currentDateTime();
    this->describe=describe;
}
Group::Group(const QString& groupTypeName,const QString& groupName,QDateTime createTime,QDateTime lastEditTime,const QString& describe){
    this->groupTypeName=groupTypeName;
    this->groupName=groupName;
    this->createTime=createTime;
    this->lastEditTime=lastEditTime;
    this->describe=describe;
}
int Group::count(){
    return keyItemList.count();
}
void Group::clear(){
    keyItemList.clear();
}
Group* Group::clone()
{
    Group* group=new Group(groupTypeName,groupName+"_副本",getDescribe()+"(复制自分组\""+getGroupName()+"\")");
    for(int i=0;i<count();i++)
        group->append(at(i)->clone());
    return group;
}
Group* Group::operator<<(KeyItem* newKeyItem){
    keyItemList<<newKeyItem;
    return this;
}
KeyItem* Group::operator[](int index){
    return keyItemList[index];
}
KeyItem* Group::at(int index){
    return keyItemList[index];
}
bool Group::removeAt(int index){
    if(index<0||index>=count()){
        return false;
    }
    else{
        keyItemList.removeAt(index);
        return true;
    }
}
void Group::append(KeyItem* newKeyItem){
    keyItemList.append(newKeyItem);
}

int Group::getGroupType() const
{
    SharedDataHelper& sharedData=SharedDataHelper::instance();
    return sharedData.groupTypeList.index(groupTypeName);
}
const QString &Group::getGroupName() const
{
    return groupName;
}

void Group::setGroupName(const QString &newGroupName)
{
    groupName = newGroupName;
}

const QString &Group::getDescribe() const
{
    return describe;
}

void Group::setDescribe(const QString &newDescribe)
{
    describe = newDescribe;
}

const QDateTime &Group::getCreateTime() const
{
    return createTime;
}
const QDateTime &Group::getLastEditTime() const
{
    return lastEditTime;
}

void Group::setLastEditTime()
{
    lastEditTime = QDateTime::currentDateTime();
}
QString Group::getListViewInfo()
{
    SettingHelper& settingHelper=SettingHelper::instance();
    return settingHelper.getListTipEnabled()?getGroupName()+"|分组管理|0|"+QString::number(count()):getGroupName()+"|分组管理|0|";
}
