#include "structure/groupType.h"
#include "util/settinghelper.h"
#include <QDebug>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
void GroupType::setCustomFieldList(const QList<FieldData> &newCustomFieldList)
{
    customFieldList = newCustomFieldList;
}

GroupType::GroupType(QString groupTypeName,QString describe,QList<FieldData> customFieldList){
    this->groupTypeName=groupTypeName;
    this->describe=describe;
    this->customFieldList=customFieldList;
    this->createTime=QDateTime::currentDateTime();
    this->lastEditTime=QDateTime::currentDateTime();
}
GroupType::GroupType(QString groupTypeName,QDateTime createTime,QDateTime lastEditTime,QString describe){
    this->groupTypeName=groupTypeName;
    this->describe=describe;
    this->createTime=createTime;
    this->lastEditTime=lastEditTime;
}
bool GroupType::has(const QString &name)
{
    if(index(name)==-1)
        return false;
    return true;
}
int GroupType::count()
{
    return customFieldList.count();
}
int GroupType::index(const QString &name){
    for(int i=0;i<customFieldList.count();i++)
        if(customFieldList[i].fieldName==name)
            return i;
    return -1;
}
void GroupType::clear(){
    customFieldList.clear();
}
bool GroupType::removeOne(const QString &name){
    int i=index(name);
    if(i==-1){
        return false;
    }else{
        customFieldList.removeAt(i);
        return true;
    }
}
bool GroupType::removeAt(int index){
    if(index<0&&index>=count()){
        return false;
    }else{
        customFieldList.removeAt(index);
        return true;
    }
}
bool GroupType::remove(const QStringList &names){
    bool flag=true;
    for(int i=0;i<names.count();i++)
        if(!removeAt(i))
            flag=false;
    return flag;
}
GroupType GroupType::operator<<(const FieldData &newField)
{
    customFieldList<<newField;
    return *this;
}
FieldData GroupType::operator[](int index){
    return customFieldList.at(index);
}
void GroupType::append(const FieldData& newItem){
    customFieldList<<newItem;
    setLastEditTime();
}
FieldData GroupType::at(int index){
    return customFieldList.at(index);
}
void GroupType::setLastEditTime(){
    this->lastEditTime=QDateTime::currentDateTime();
}
const QString &GroupType::getDescribe() const
{
    return describe;
}

void GroupType::setDescribe(const QString &newDescribe)
{
    describe = newDescribe;
    setLastEditTime();
}

const QDateTime &GroupType::getCreateTime() const
{
    return createTime;
}

const QDateTime &GroupType::getLastEditTime() const
{
    return lastEditTime;
}

const QString &GroupType::getGroupTypeName() const
{
    return groupTypeName;
}
QStringList GroupType::getFieldNames(){
    QStringList list;
    for(int i=0;i<count();i++)
        list<<customFieldList[i].fieldName;
    return list;
}
QString groupTypeName;
QString describe;
QDateTime createTime;
QDateTime lastEditTime;
QList<AbstractCustomField*> customFieldList;
QString GroupType::getCreateSql(const QString &tableName){
    QString sql="create table "+tableName+" (groupName varchar(40),";
    for(int i=0;i<count();i++){
        sql+="'";
        sql+=customFieldList[i].fieldName;
        sql+="'";
        sql+=" text,";
    }
    sql+="createTime varchar(200),lastEditTime varchar(200))";
    return sql;
}
QString GroupType::getUpdateSql(const QString &tableName){
    QString sql="update "+tableName+" set ";
    for(int i=0;i<count();i++){
        sql+="'";
        sql+=customFieldList[i].fieldName;
        sql+="'";
        sql+="='%"+QString::number(i+1)+"',";
    }
    sql+="lastEditTime='%"+QString::number(count()+1)+"' where createTime='%"+QString::number(count()+2)+"'";
    return sql;
}
QString GroupType::getInsertSql(const QString &tableName){
    QString sql="insert into "+tableName+" (groupName,";
    for(int i=0;i<count();i++){
        sql+="'";
        sql+=customFieldList[i].fieldName;
        sql+="'";
        sql+=",";
    }
    sql+="createTime,lastEditTime)VALUES(";
    for(int i=0;i<count()+3;i++){
        sql+=":";
        sql+=QString::number(i+1);
        if(i<count()+2)
            sql+=",";
    }
    sql+=")";
    return sql;
}
void GroupType::setGroupTypeName(const QString &newGroupTypeName)
{
    groupTypeName = newGroupTypeName;
    setLastEditTime();
}
GroupType* GroupType::clone(){
    QList<FieldData> copyFieldList;
    for(int i=0;i<count();i++){
        copyFieldList<<customFieldList[i];
    }
    GroupType* copy=new GroupType(groupTypeName,describe,copyFieldList);
    return copy;
}
QString GroupType::getListViewInfo()
{
    SettingHelper& settingHelper=SettingHelper::instance();
    return (settingHelper.getListTipEnabled()?getGroupTypeName()+"|分组类型管理|0|"+QString::number(count()):getGroupTypeName()+"|分组类型管理|0|");
}
