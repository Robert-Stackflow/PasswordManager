#include "customField/customTextEdit.h"
#include "customField/customLineEdit.h"
#include "customField/customComboBox.h"
#include "customField/fieldfactory.h"
#include "qsqlerror.h"
#include "util/settinghelper.h"
#include "widget.h"
#include "ui_widget.h"
#include "QsLog.h"
#include "util/databasehelper.h"
#include "util/datapathhelper.h"
#include "util/customdatabase.h"
#include "util/shareddatahelper.h"
#include <QSqlQuery>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
void Widget::loadUserData()
{
    loadGroupTypes();
    loadGroups();
    loadAutofillInfo();
}
void Widget::loadGroups()
{
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    //连接数据库
    QString databaseName=dataPathHelper.getCurrentAccountDataBasePath();
    customDataBase.setDatabaseName(databaseName);
    QSqlQuery query(customDataBase.getDatabase());
    if(customDataBase.open()){
        QStringList tableNames=customDataBase.tables();
        if(!tableNames.contains(dataBaseHelper.getGroupsTableName()))
        {//如果"groups"表不存在,则新建"默认分组"
            //开启事务
            QString transactionBeginSql="begin";
            query.exec(transactionBeginSql);
            //原子任务——创建"groups"表
            QString createSql="create table "+dataBaseHelper.getGroupsTableName()
                    +" (groupTypeName varchar(40),groupName varchar(40),KeyItemCount int,createTime varchar(200),"
                     "lastEditTime varchar(200),describe text)";
            bool createSqlFlag=query.exec(createSql);
            //原子任务——插入"默认分组"记录
            Group* newGroup=new Group("默认类型","默认分组",QDateTime::currentDateTime(),QDateTime::currentDateTime(),"账户初始化时创建的默认分组");
            sharedDataHelper.groupList<<newGroup;
            QString insertSql="insert into "+dataBaseHelper.getGroupsTableName()
                    +" (groupTypeName,groupName,KeyItemCount,createTime,lastEditTime,describe) "
                     "VALUES(:1,:2,:3,:4,:5,:6)";
            query.prepare(insertSql);
            query.bindValue(":1",newGroup->getGroupTypeName());
            query.bindValue(":2",newGroup->getGroupName());
            query.bindValue(":3",newGroup->count());
            query.bindValue(":4",newGroup->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.bindValue(":5",newGroup->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.bindValue(":6",newGroup->getDescribe());
            bool insertSqlFlag=query.exec();
            //原子任务——创建"默认分组"表
            QString createDefaultGroupSql=sharedDataHelper.groupTypeList[newGroup->getGroupType()]->getCreateSql(dataBaseHelper.getGroupTableName(newGroup->getGroupName()));
            bool createDefaultGroupFlag=query.exec(createDefaultGroupSql);
            if(createSqlFlag&&insertSqlFlag&&createDefaultGroupFlag){
                QString transactionCommitSql="commit";
                query.exec(transactionCommitSql);
            }else{
                QString transactionRollbackSql="rollback";
                query.exec(transactionRollbackSql);
            }
        }else{
            //加载groups表中的分组列表
            //新建各个分组
            QString curTableName=dataBaseHelper.getGroupsTableName();
            query.exec("select * from "+curTableName);
            for(int i = 0;query.next(); i++){
                Group* newGroup=new Group(query.value(0).toString(),query.value(1).toString(),query.value(3).toDateTime(),query.value(4).toDateTime(),query.value(5).toString());
                sharedDataHelper.groupList<<newGroup;
            }
            //加载每个分组的密码条目
            for(int i=0;i<sharedDataHelper.groupList.count();i++){
                curTableName=dataBaseHelper.getGroupTableName(sharedDataHelper.groupList[i]->getGroupName());
                query.exec("select * from "+curTableName);
                for(int k=0;query.next();k++){
                    QStringList fieldValueList;
                    int j=0;
                    for(j=1;j<sharedDataHelper.groupTypeList[sharedDataHelper.groupList[i]->getGroupType()]->count();j++)
                        fieldValueList<<query.value(j).toString();
                    KeyItem* newKeyItem=new KeyItem(sharedDataHelper.groupList[i]->getGroupName(),query.value(j+1).toDateTime(),query.value(j+2).toDateTime(),fieldValueList);
                    sharedDataHelper.groupList[i]->append(newKeyItem);
                }
            }
            customDataBase.close();
        }
    }
    //发射信号——分组数目改变
    emit groupCountChanged();
}
void Widget::loadGroupTypes()
{
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    //连接数据库
    QString currentUsername=sharedDataHelper.accountList.getActiveAccount()->getUsername();
    QString databaseName=dataPathHelper.getCurrentAccountDataBasePath();
    customDataBase.setDatabaseName(databaseName);
    QSqlQuery query(customDataBase.getDatabase());
    if(customDataBase.open()){
        QStringList tableNames=customDataBase.tables();
        if(!tableNames.contains(dataBaseHelper.getGroupTypesTableName()))
        {//如果不包含"groupTypes"表,则新建"默认分组类型"
            //开启事务
            QString transactionBeginSql="begin";
            query.exec(transactionBeginSql);
            //原子任务——创建"groupTypes"表
            QString createGroupTypesTableSql="create table "+dataBaseHelper.getGroupTypesTableName()
                    +" (groupTypeName varchar(200),fieldCount int,"
                     "createTime varchar(200),lastEditTime varchar(200),describe text)";
            bool createGroupTypesTableFlag=query.exec(createGroupTypesTableSql);
            //原子任务——插入"默认分组类型"记录
            QList<FieldData> customFieldList;
            customFieldList<<FieldData("名称",sharedDataHelper.fieldTypeList["单行文本"],FieldData::REQUIRED,"");
            customFieldList<<FieldData("用户名",sharedDataHelper.fieldTypeList["单行文本"],FieldData::REQUIRED,"");
            customFieldList<<FieldData("密码",sharedDataHelper.fieldTypeList["密码"],FieldData::REQUIRED,"");;
            customFieldList<<FieldData("邮箱",sharedDataHelper.fieldTypeList["自动填充-邮箱"],FieldData::NOTREQUIRED,"");
            customFieldList<<FieldData("电话",sharedDataHelper.fieldTypeList["自动填充-电话"],FieldData::NOTREQUIRED,"");
            customFieldList<<FieldData("网址",sharedDataHelper.fieldTypeList["自动填充-网址"],FieldData::NOTREQUIRED,"");
            customFieldList<<FieldData("备注",sharedDataHelper.fieldTypeList["多行文本"],FieldData::NOTREQUIRED,"");
            GroupType* loadGroupType=new GroupType("默认类型","账户初始化时创建的默认分组类型",customFieldList);
            sharedDataHelper.groupTypeList<<loadGroupType;
            QString insertIntoGroupTypesTableSql="insert into "+dataBaseHelper.getGroupTypesTableName()
                    +" (groupTypeName,fieldCount,createTime,lastEditTime,describe) "
                     "VALUES(:1,:2,:3,:4,:5)";
            query.prepare(insertIntoGroupTypesTableSql);
            query.bindValue(":1",loadGroupType->getGroupTypeName());
            query.bindValue(":2",loadGroupType->count());
            query.bindValue(":3",loadGroupType->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.bindValue(":4",loadGroupType->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.bindValue(":5",loadGroupType->getDescribe());
            bool insertIntoGroupTypesTableFlag=query.exec();
            //原子任务——创建"默认分组类型"表
            QString createDefaultGroupTypeTableSql="create table "+dataBaseHelper.getGroupTypeTableName(loadGroupType->getGroupTypeName())
                    +" (groupTypeName varchar(40),fieldName varchar(200),isRequired int,fieldTypeName varchar(200),placeholderText text)";
            bool createDefaultGroupTypeTableFlag=query.exec(createDefaultGroupTypeTableSql);
            //原子任务——插入"默认分组类型"的类型字段
            bool insertFieldFlag=true;
            for(int i=0;i<loadGroupType->count();i++){
                QString insertFieldSql="insert into "+dataBaseHelper.getGroupTypeTableName(loadGroupType->getGroupTypeName())
                        +" (groupTypeName,fieldName,isRequired,fieldTypeName,placeholderText)VALUES(:1,:2,:3,:4,:5)";
                query.prepare(insertFieldSql);
                query.bindValue(":1",loadGroupType->getGroupTypeName());
                query.bindValue(":2",loadGroupType->at(i).fieldName);
                query.bindValue(":3",int(loadGroupType->at(i).isRequired));
                query.bindValue(":4",loadGroupType->at(i).fieldTypeName);
                query.bindValue(":5",loadGroupType->at(i).placeholderText);
                bool temp=query.exec();
                insertFieldFlag=insertFieldFlag&&temp;
            }
            if(createGroupTypesTableFlag&&insertIntoGroupTypesTableFlag&&createDefaultGroupTypeTableFlag&&insertFieldFlag){
                QString transactionCommitSql="commit";
                query.exec(transactionCommitSql);
            }else{
                QString transactionRollbackSql="rollback";
                query.exec(transactionRollbackSql);
            }
        }
        else
        {//如果包含"groupTypes"表,则加载分组类型
            QString curTableName=dataBaseHelper.getGroupTypesTableName();
            QString selectSql="select * from "+curTableName;
            query.exec(selectSql);
            //新建各个分组类型
            for(int i = 0;query.next(); i++){
                GroupType* newGroupType=new GroupType(query.value(0).toString(),query.value(2).toDateTime(),query.value(3).toDateTime(),query.value(4).toString());
                sharedDataHelper.groupTypeList<<newGroupType;
            }
            //加载各个分组类型的字段条目
            for(int i=0;i<sharedDataHelper.groupTypeList.count();i++){
                curTableName=dataBaseHelper.getGroupTypeTableName(sharedDataHelper.groupTypeList[i]->getGroupTypeName());
                QString selectSql="select * from "+curTableName;
                query.exec(selectSql);
                QList<FieldData> fieldList;
                for(int i = 0;query.next(); i++){
                    QString fieldName=query.value(1).toString();
                    QString fieldTypeName=query.value(3).toString();
                    QString fieldPlaceholderText=query.value(4).toString();
                    FieldData::isRequiredChoices isRequired=FieldData::isRequiredChoices(query.value(2).toInt());
                    fieldList<<FieldData(fieldName,sharedDataHelper.fieldTypeList[fieldTypeName],isRequired,fieldPlaceholderText);
                }
                sharedDataHelper.groupTypeList[i]->setCustomFieldList(fieldList);
            }
        }
        customDataBase.close();
    }
    //发射信号——分组类型数目改变
    emit groupTypeCountChanged();
}
void Widget::loadAutofillInfo()
{
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    //连接数据库
    QString databaseName=dataPathHelper.getCurrentAccountDataBasePath();
    customDataBase.setDatabaseName(databaseName);
    QSqlQuery query(customDataBase.getDatabase());
    if(customDataBase.open()){
        QStringList tableNames=customDataBase.tables();
        if(!tableNames.contains(dataBaseHelper.getAutofillInfoTableName())){
            QString createSql="create table "+dataBaseHelper.getAutofillInfoTableName()+" (type int,content varchar(100),remark varchar(100))";
            query.exec(createSql);
        }
        //加载个人信息
        QString curTableName=dataBaseHelper.getAutofillInfoTableName();
        QString selectSql="select * from "+curTableName;
        query.exec(selectSql);
        for(int i = 0;query.next(); i++){
            //个人信息类型:1-邮箱,2-电话,3-网址
            int type=query.value(0).toInt();
            switch(type)
            {
            case 0:
                sharedDataHelper.autofillInfo.addMail(query.value(1).toString(),query.value(2).toString());
                break;
            case 1:
                sharedDataHelper.autofillInfo.addMobile(query.value(1).toString());
                break;
            case 2:
                sharedDataHelper.autofillInfo.addWebsite(query.value(1).toString());
                break;
            }
        }
        customDataBase.close();
    }
}
void Widget::updateAutofillInfo()
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    customDataBase.setDatabaseName(dataPathHelper.getCurrentAccountDataBasePath());
    //        customDataBase.setPassword("123456");
    //        customDataBase.setConnectOptions("QSQLITE_CREATE_ITEM");
    //        customDataBase.setConnectOptions("QSQLITE_REMOVE_ITEM");
    QSqlQuery query(customDataBase.getDatabase());
    if(customDataBase.open()){
        QString curTableName=dataBaseHelper.getAutofillInfoTableName();
        if(customDataBase.tables().contains(curTableName)){
            query.exec("drop table "+curTableName);
        }
        query.exec("create table "+curTableName+" (type int,content varchar(100),remark varchar(100))");
        //添加mail
        for(int i=0;i<sharedDataHelper.autofillInfo.getMails().count();i++){
            query.prepare("insert into "+curTableName+" (type,content,remark)"
                                                      "VALUES (:1,:2,:3)");
            query.bindValue(":1",0);
            query.bindValue(":2",sharedDataHelper.autofillInfo.getMails()[i]);
            query.bindValue(":3",sharedDataHelper.autofillInfo.getMailAliases()[i]);
            query.exec();
        }
        //添加mobile
        for(int i=0;i<sharedDataHelper.autofillInfo.getMobiles().size();i++){
            query.prepare("insert into "+curTableName+" (type,content,remark)"
                                                      "VALUES (:1,:2,:3)");
            query.bindValue(":1",1);
            query.bindValue(":2",sharedDataHelper.autofillInfo.getMobiles()[i]);
            query.bindValue(":3","");
            query.exec();
        }
        //添加website
        for(int i=0;i<sharedDataHelper.autofillInfo.getWebsites().size();i++){
            query.prepare("insert into "+curTableName+" (type,content,remark)"
                                                      "VALUES (:1,:2,:3)");
            query.bindValue(":1",2);
            query.bindValue(":2",sharedDataHelper.autofillInfo.getWebsites()[i]);
            query.bindValue(":3","");
            query.exec();
        }
        customDataBase.close();
    }
}

void Widget::loadSettings()
{
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    SettingHelper& settingHelper = SettingHelper::instance();
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    //连接数据库
    QString databaseName=dataPathHelper.getCurrentAccountDataBasePath();
    customDataBase.setDatabaseName(databaseName);
    QSqlQuery query(customDataBase.getDatabase());
    if(customDataBase.open()){
        QStringList tableNames=customDataBase.tables();
        if(!tableNames.contains(dataBaseHelper.getSettingsTableName())){
            QString createSql="create table "+dataBaseHelper.getSettingsTableName()+" (itemName varchar(200),itemContent varchar(200))";
            query.exec(createSql);
        }
    }
}
