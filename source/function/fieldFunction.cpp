#include "controller/floatbox.h"
#include "customField/customComboBox.h"
#include "customField/customLineEdit.h"
#include "customField/customTextEdit.h"
#include "dialogs/fielddialog.h"
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
void Widget::onAddFieldClicked(const QString& name)
{
    FieldDialog* dialog=new FieldDialog(name,this);
    dialog->show();
    connect(dialog,SIGNAL(newFieldAccept(QString,FieldData)),this,SLOT(onNewFieldAccept(QString,FieldData)));
}
void Widget::onNewFieldAccept(const QString &groupTypeName, const FieldData& fieldData)
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    GroupType* groupType=sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(groupTypeName)];
    groupType->append(fieldData);
    customDataBase.setDatabaseName(dataPathHelper.getCurrentAccountDataBasePath());
    QSqlQuery query(customDataBase.getDatabase());
    if(customDataBase.open()){
        if(customDataBase.tables().contains(dataBaseHelper.getGroupTypeTableName(groupTypeName))){
            //更新sharedData.groupTypeList
            QString sql=QString("update "+dataBaseHelper.getGroupTypesTableName()+" set fieldCount='%1',lastEditTime='%2' where createTime='%3' ")
                    .arg(groupType->count())
                    .arg(groupType->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"))
                    .arg(groupType->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.exec(sql);
            //更新记录类型中字段信息的表
            query.prepare("insert into "+dataBaseHelper.getGroupTypeTableName(groupTypeName)+" (groupTypeName,fieldName,isRequired,fieldTypeName,placeholderText)"
                                                                "VALUES (:1,:2,:3,:4,:5)");
            query.bindValue(":1",groupTypeName);
            query.bindValue(":2",fieldData.fieldName);
            query.bindValue(":3",fieldData.isRequired);
            query.bindValue(":4",fieldData.fieldTypeName);
            query.bindValue(":5",fieldData.placeholderText);
            query.exec();
            customDataBase.close();
        }
    }
    emit groupTypeContentChanged();
    ui->listView->setCurrentIndexByData(groupTypeName);
    FloatBox::Success("添加字段成功",this);
}
