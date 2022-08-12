#include "structure/groupTypes.h"
#include "qdebug.h"
#include "util/settinghelper.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
int GroupTypes::index(const QString& GroupTypeName){
    for(int i=0;i<groupTypeList.count();i++)
        if(groupTypeList[i]!=nullptr&&groupTypeList[i]->getGroupTypeName()==GroupTypeName)
            return i;
    return -1;
}
bool GroupTypes::has(const QString& GroupTypeName){
    if(index(GroupTypeName)==-1)
        return false;
    else
        return true;
}
GroupType* GroupTypes::at(int index){
    if(index<0||index>=count())
        return nullptr;
    return groupTypeList[index];
}
void GroupTypes::append(GroupType* newGroupType){
    groupTypeList<<newGroupType;
}
bool GroupTypes::removeOne(const QString& GroupTypeName){
    int i=index(GroupTypeName);
    qDebug()<<i;
    if(i==-1){
        return false;
    }else{
        groupTypeList.removeAt(i);
        return true;
    }
}
bool GroupTypes::remove(const QStringList& GroupTypeNames){
    bool flag=true;
    for(int i=0;i<GroupTypeNames.count();i++)
        if(!removeOne(GroupTypeNames[i]))
            flag=false;
    return flag;
}
int GroupTypes::count(){
    return groupTypeList.count();
}
GroupType *GroupTypes::operator [](int index){
    if(index<0||index>=count())
        return nullptr;
    return groupTypeList[index];
}
GroupTypes* GroupTypes::operator<<(GroupType* newGroupType){
    groupTypeList<<newGroupType;
    return this;
}
QStringList GroupTypes::getGroupTypeNames(){
    QStringList list;
    for(int i=0;i<groupTypeList.count();i++)
        list<<groupTypeList[i]->getGroupTypeName();
    return list;
}
QStringList GroupTypes::getListViewInfo()
{
    QStringList list;
    SettingHelper& settingHelper=SettingHelper::instance();
    list<<(settingHelper.getListTipEnabled()?groupTypesName+"||0|"+QString::number(count()):groupTypesName+"||0|");
    for(int i=0;i<count();i++)
        list<<groupTypeList[i]->getListViewInfo();
    return list;
}
