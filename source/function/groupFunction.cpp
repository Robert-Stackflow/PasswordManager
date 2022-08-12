#include "controller/floatbox.h"
#include "customField/customComboBox.h"
#include "customField/customLineEdit.h"
#include "customField/customTextEdit.h"
#include "dialogs/groupdialog.h"
#include "dialogs/groupinfodialog.h"
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
void Widget::onGroupMoreClicked(const QString& name)
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    GroupInfoDialog *dialog=new GroupInfoDialog(this);
    dialog->loadGroup(sharedDataHelper.groupList[sharedDataHelper.groupList.index(name)]);
    dialog->show();
    connect(dialog,&GroupInfoDialog::editClicked,[=]{
        GroupDialog *dialog=new GroupDialog(GroupDialog::EDITGROUP,this);
        dialog->loadGroup(sharedDataHelper.groupList[sharedDataHelper.groupList.index(name)]);
        dialog->show();
        connect(dialog,SIGNAL(editGroupAccept(QString,Group*)),this,SLOT(onEditGroupAccept(QString,Group*)));
    });
    connect(dialog,&GroupInfoDialog::copyClicked,[=]{
        GroupDialog *dialog=new GroupDialog(GroupDialog::COPYGROUP,this);
        dialog->loadGroup(sharedDataHelper.groupList[sharedDataHelper.groupList.index(name)]);
        dialog->show();
        connect(dialog,SIGNAL(newGroupAccept(Group*)),this,SLOT(onNewGroupAccept(Group*)));
    });
    connect(dialog,&GroupInfoDialog::deleteClicked,[=]{
        if(sharedDataHelper.groupList.count()==1){
            CustomMessageBox::warning(this,"删除分组","无法删除分组(至少存在一个分组!)");
        }else{
            int choice=CustomMessageBox::question(this,"删除分组","是否确认删除分组\""+name+"\"?\n警告:分组被删除后将无法撤回!");
            if(choice==CustomMessageBox::YES)
                deleteGroup(name);
        }
    });
}
void Widget::onAddGroupClicked(const QString&)
{
    GroupDialog *dialog=new GroupDialog(GroupDialog::NEWGROUP,this);
    dialog->show();
    connect(dialog,SIGNAL(newGroupAccept(Group*)),this,SLOT(onNewGroupAccept(Group*)));
}
void Widget::onNewGroupAccept(Group* group)
{
    if(group==nullptr||group->getGroupType()==-1)
        return;
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    //添加到sharedDataHelper.groupList中
    sharedDataHelper.groupList<<group;
    //添加到数据库中
    customDataBase.setDatabaseName(dataPathHelper.getCurrentAccountDataBasePath());
    QSqlQuery query(customDataBase.getDatabase());
    if(customDataBase.open()){
        QString newGroupTableName=dataBaseHelper.getGroupTableName(group->getGroupName());
        if(!customDataBase.tables().contains(newGroupTableName)){
            //如果分组不存在，创建记录分组信息的表
            query.exec(sharedDataHelper.groupTypeList[group->getGroupType()]->getCreateSql(newGroupTableName));
            //向sharedDataHelper.groupList表插入数据
            query.prepare("insert into "+dataBaseHelper.getGroupsTableName()+" (groupTypeName,groupName,KeyItemCount,createTime,lastEditTime,describe)"
                                                                             "VALUES (:1,:2,:3,:4,:5,:6)");
            query.bindValue(":1",group->getGroupTypeName());
            query.bindValue(":2",group->getGroupName());
            query.bindValue(":3",group->count());
            query.bindValue(":4",group->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.bindValue(":5",group->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.bindValue(":6",group->getDescribe());
            query.exec();
        }
        customDataBase.close();
    }
    //发射信号——分组数目改变
    emit groupCountChanged();
    FloatBox::Success("新建成功",this);
    ui->listView->setCurrentIndexByData(group->getGroupName());
}
void Widget::onEditGroupAccept(const QString& oldGroupName,Group* group)
{
    if(group==nullptr||group->getGroupType()==-1)
        return;
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    Group* oldGroup=sharedDataHelper.groupList[sharedDataHelper.groupList.index(oldGroupName)];
    oldGroup->setGroupName(group->getGroupName());
    oldGroup->setDescribe(group->getDescribe());
    oldGroup->setLastEditTime();
    // 更新数据库
    customDataBase.setDatabaseName(dataPathHelper.getCurrentAccountDataBasePath());
    QSqlQuery query(customDataBase.getDatabase());
    if(customDataBase.open()){
        QString oldGroupTableName=dataBaseHelper.getGroupTableName(oldGroupName);
        QString newGroupTableName=dataBaseHelper.getGroupTableName(oldGroup->getGroupName());
        query.exec("alter table "+oldGroupTableName+" rename to "+newGroupTableName);
        QString sql=QString("update "+dataBaseHelper.getGroupsTableName()+" set groupName='%1',lastEditTime='%2',describe='%3' where createTime='%4' ")
                .arg(oldGroup->getGroupName())
                .arg(oldGroup->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"))
                .arg(oldGroup->getDescribe())
                .arg(oldGroup->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query.exec(sql);
        customDataBase.close();
    }
    FloatBox::Success("修改成功",this);
    ui->listView->setCurrentIndexByData(group->getGroupName());
}
void Widget::deleteGroup(const QString& groupName)
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    int index=sharedDataHelper.groupList.index(groupName);
    if(index==-1)
        return;
    //连接数据库
    customDataBase.setDatabaseName(dataPathHelper.getCurrentAccountDataBasePath());
    QSqlQuery query(customDataBase.getDatabase());
    if(customDataBase.open()){
        //删除数据表
        query.exec("drop table "+dataBaseHelper.getGroupTableName(groupName));
        QString sql = QString("delete from "+dataBaseHelper.getGroupsTableName()+" where groupName = '%1' ").arg(groupName);
        //删除group
        sharedDataHelper.groupList.removeAt(index);
        query.exec(sql);
        customDataBase.close();
    }
    //发射信号——分组数目改变
    emit groupCountChanged();
    FloatBox::Success("删除成功",this);
    ui->listView->setCurrentIndexByData("分组管理");
}
