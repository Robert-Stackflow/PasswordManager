#include "controller/floatbox.h"
#include "customField/customComboBox.h"
#include "customField/customLineEdit.h"
#include "customField/customTextEdit.h"
#include "dialogs/grouptypedialog.h"
#include "dialogs/grouptypeinfodialog.h"
#include "qsqlerror.h"
#include "ui_groupinfodialog.h"
#include "widget.h"
#include "ui_widget.h"
#include "util/databasehelper.h"
#include "util/datapathhelper.h"
#include "util/shareddatahelper.h"
#include "util/customdatabase.h"
#include "QsLog.h"
#include <QSqlQuery>
#include <QMessageBox>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
void Widget::onAddGroupTypeClicked(const QString &name)
{
    GroupTypeDialog *dialog=new GroupTypeDialog(GroupTypeDialog::NEWGROUPTYPE,this);
    dialog->show();
    connect(dialog,SIGNAL(newGroupTypeAccept(GroupType*)),this,SLOT(onNewGroupTypeAccept(GroupType*)));
}
void Widget::onGroupTypeMoreClicked(const QString &name)
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    GroupTypeInfoDialog *dialog=new GroupTypeInfoDialog(this);
    dialog->loadGroupType(sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(name)]);
    dialog->show();
    connect(dialog,&GroupTypeInfoDialog::editClicked,[=]{
        GroupTypeDialog *dialog=new GroupTypeDialog(GroupTypeDialog::EDITGROUPTYPE,this);
        dialog->loadGroupType(sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(name)]);
        dialog->show();
        connect(dialog,SIGNAL(editGroupTypeAccept(QString,GroupType*)),this,SLOT(onEditGroupTypeAccept(QString,GroupType*)));
    });
    connect(dialog,&GroupTypeInfoDialog::copyClicked,[=]{
        GroupTypeDialog *dialog=new GroupTypeDialog(GroupTypeDialog::COPYGROUPTYPE,this);
        dialog->loadGroupType(sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(name)]);
        dialog->show();
        connect(dialog,SIGNAL(newGroupTypeAccept(GroupType*)),this,SLOT(onNewGroupTypeAccept(GroupType*)));
    });
    connect(dialog,&GroupTypeInfoDialog::deleteClicked,[=]{
        if(sharedDataHelper.groupTypeList.count()==1){
            CustomMessageBox::warning(this,"删除分组类型","无法删除分组类型(至少存在一种分组类型!)");
        }else{
            int choice=CustomMessageBox::question(this,"删除分组类型","是否确认删除分组类型\""+name+"\"?\n警告:分组类型被删除后将无法撤回,且与该分组类型关联的所有分组都将被删除!");
            if(choice==CustomMessageBox::YES)
                deleteGroupType(name);
        }
    });
}
void Widget::onNewGroupTypeAccept(GroupType* groupType)
{
    if(groupType!=nullptr){
        SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
        DataPathHelper& dataPathHelper = DataPathHelper::instance();
        CustomDataBase& customDataBase = CustomDataBase::instance();
        DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
        //添加到sharedData.groupTypeList中
        sharedDataHelper.groupTypeList<<groupType;
        //添加到数据库中
        customDataBase.setDatabaseName(dataPathHelper.getCurrentAccountDataBasePath());
        QSqlQuery query(customDataBase.getDatabase());
        if(customDataBase.open()){
            QString newGroupTypeTableName=dataBaseHelper.getGroupTypeTableName(groupType->getGroupTypeName());
            if(!customDataBase.tables().contains(newGroupTypeTableName)){
                //向sharedData.groupTypeList表插入数据
                query.prepare("insert into "+dataBaseHelper.getGroupTypesTableName()+" (groupTypeName,fieldCount,createTime,lastEditTime,describe)"
                                                                                     "VALUES (:1,:2,:3,:4,:5)");
                query.bindValue(":1",groupType->getGroupTypeName());
                query.bindValue(":2",groupType->count());
                query.bindValue(":3",groupType->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
                query.bindValue(":4",groupType->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"));
                query.bindValue(":5",groupType->getDescribe());
                query.exec();
                //创建记录类型中字段信息的表
                query.exec("create table "+newGroupTypeTableName+" (groupTypeName varchar(40),fieldName varchar(200),isRequired int,fieldTypeName varchar(200),placeholderText varchar(200))");
                //添加字段信息记录
                for(int i=0;i<groupType->count();i++){
                    query.prepare("insert into "+newGroupTypeTableName+" (groupTypeName,fieldName,isRequired,fieldTypeName,placeholderText)"
                                                                       "VALUES (:1,:2,:3,:4,:5)");
                    query.bindValue(":1",groupType->getGroupTypeName());
                    query.bindValue(":2",groupType->at(i).fieldName);
                    query.bindValue(":3",groupType->at(i).isRequired);
                    query.bindValue(":4",groupType->at(i).fieldTypeName);
                    query.bindValue(":5",groupType->at(i).placeholderText);
                    query.exec();
                }
                customDataBase.close();
            }
        }
        //发射信号——类型数目改变
        emit groupTypeCountChanged();
        FloatBox::Success("新建成功",this);
        ui->listView->setCurrentIndexByData(groupType->getGroupTypeName());
    }
}
void Widget::onEditGroupTypeAccept(const QString &oldGroupTypeName, GroupType* groupType)
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    if(groupType!=nullptr&&sharedDataHelper.groupTypeList.has(oldGroupTypeName)){
        //更新groupType信息
        GroupType* oldGroupType=sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(oldGroupTypeName)];
        oldGroupType->setGroupTypeName(groupType->getGroupTypeName());
        oldGroupType->setLastEditTime();
        oldGroupType->setDescribe(groupType->getDescribe());
        //更新数据库
        customDataBase.setDatabaseName(dataPathHelper.getCurrentAccountDataBasePath());
        QSqlQuery query(customDataBase.getDatabase());
        if(customDataBase.open()){
            QString editGroupTypeTableOldName=dataBaseHelper.getGroupTypeTableName(oldGroupTypeName);
            QString editGroupTypeTableName=dataBaseHelper.getGroupTypeTableName(groupType->getGroupTypeName());
            if(customDataBase.tables().contains(editGroupTypeTableOldName)){
                //更新sharedData.groupTypeList
                QString sql=QString("update "+dataBaseHelper.getGroupTypesTableName()+" set groupTypeName='%1',fieldCount='%2',lastEditTime='%3',describe='%4' where createTime='%5' ")
                        .arg(oldGroupType->getGroupTypeName())
                        .arg(QString::number(oldGroupType->count()))
                        .arg(oldGroupType->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"))
                        .arg(oldGroupType->getDescribe())
                        .arg(oldGroupType->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
                query.exec(sql);
                //更新记录类型中字段信息的表
                query.exec("drop table "+editGroupTypeTableOldName);
                query.exec("create table "+editGroupTypeTableName+" (fieldName int,isRequired int,fieldTypeName varchar(200),placeholderText varchar(200))");
                //添加字段信息记录
                for(int i=0;i<oldGroupType->count();i++){
                    query.prepare("insert into "+editGroupTypeTableName+" (fieldName,isRequired,fieldTypeName,placeholderText)"
                                                                        "VALUES (:1,:2,:3,:4)");
                    query.bindValue(":1",oldGroupType->at(i).fieldName);
                    query.bindValue(":2",oldGroupType->at(i).isRequired);
                    query.bindValue(":3",oldGroupType->at(i).fieldTypeName);
                    query.bindValue(":4",oldGroupType->at(i).placeholderText);
                    query.exec();
                }
                customDataBase.close();
            }
        }
        FloatBox::Success("修改成功",this);
        ui->listView->setCurrentIndexByData(groupType->getGroupTypeName());
    }
}
void Widget::deleteGroupType(const QString &groupTypeName)
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    if(sharedDataHelper.groupTypeList.has(groupTypeName)){
        customDataBase.setDatabaseName(dataPathHelper.getCurrentAccountDataBasePath());
        QSqlQuery query(customDataBase.getDatabase());
        if(customDataBase.open()){
            QString groupTypeTableName=dataBaseHelper.getGroupTypeTableName(groupTypeName);
            if(customDataBase.tables().contains(groupTypeTableName)){
                query.exec("drop table "+groupTypeTableName);
                QString sql=QString("delete from "+dataBaseHelper.getGroupTypesTableName()+" where groupTypeName='%1' ").arg(groupTypeName);
                query.exec(sql);
                sharedDataHelper.groupTypeList.removeOne(groupTypeName);
                customDataBase.close();
            }
        }
        emit groupTypeCountChanged();
        FloatBox::Success("删除成功",this);
        ui->listView->setCurrentIndexByData("分组类型管理");
    }
}
