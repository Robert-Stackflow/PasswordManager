#include "dialogs/keyitemdialog.h"
#include "qsqlerror.h"
#include "widget.h"
#include "ui_widget.h"
#include "controller/floatbox.h"
#include "util/databasehelper.h"
#include "util/datapathhelper.h"
#include "util/shareddatahelper.h"
#include "util/customdatabase.h"
#include "QsLog.h"
#include <QSqlQuery>
#pragma warning (disable:4100)
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
void Widget::onAddKeyItemClicked(const QString& name)
{
    KeyItemDialog* dialog=new KeyItemDialog(name,this);
    dialog->show();
    connect(dialog,SIGNAL(newKeyItemAccept(QString,KeyItem*)),this,SLOT(onNewKeyItemAccept(QString,KeyItem*)));
}
void Widget::onNewKeyItemAccept(const QString& groupName,KeyItem* newKeyItem)
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    if(newKeyItem!=nullptr&&sharedDataHelper.groupList.has(groupName)){
        Group* currentGroup=sharedDataHelper.groupList[sharedDataHelper.groupList.index(groupName)];
        GroupType* currentGroupType=sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(currentGroup->getGroupTypeName())];
        currentGroup->append(newKeyItem);
        currentGroup->setLastEditTime();
        QString databaseName=dataPathHelper.getCurrentAccountDataBasePath();
        customDataBase.setDatabaseName(databaseName);
        QSqlQuery query(customDataBase.getDatabase());
        if(customDataBase.open()){
            QString sql=QString("update "+dataBaseHelper.getGroupsTableName()+" set KeyItemCount='%1',lastEditTime='%2' where groupName='%3' ")
                    .arg(currentGroup->count())
                    .arg(currentGroup->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"))
                    .arg(currentGroup->getGroupName());
            query.exec(sql);
            query.prepare(sharedDataHelper.groupTypeList[currentGroup->getGroupType()]->getInsertSql(dataBaseHelper.getGroupTableName(groupName)));
            query.bindValue(":1",groupName);
            for(int i=1;i<=sharedDataHelper.groupTypeList[currentGroup->getGroupType()]->count();i++)
                query.bindValue(":"+QString::number(i+1),newKeyItem->at(i-1));
            query.bindValue(":"+QString::number(sharedDataHelper.groupTypeList[currentGroup->getGroupType()]->count()+2),newKeyItem->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.bindValue(":"+QString::number(sharedDataHelper.groupTypeList[currentGroup->getGroupType()]->count()+3),newKeyItem->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"));
            query.exec();
            customDataBase.close();
        }
        emit groupContentChanged();
        ui->listView->setCurrentIndexByData(groupName);
        FloatBox::Success("添加条目成功",this);
        for(int i=0;i<currentGroupType->count();i++)
        {
            switch(currentGroupType->at(i).dataType){
            case FieldData::MAIL:
                sharedDataHelper.autofillInfo.addMail(newKeyItem->at(i),"");
                break;
            case FieldData::MOBILE:
                sharedDataHelper.autofillInfo.addMobile(newKeyItem->at(i));
                break;
            case FieldData::WEBSITE:
                sharedDataHelper.autofillInfo.addWebsite(newKeyItem->at(i));
                break;
            default:
                break;
            }
        }
        updateAutofillInfo();
    }
}
void Widget::onRemoveKeyItemClicked()
{
    //    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    //    CustomDataBase& customDataBase = CustomDataBase::instance();
    //    QList<QTableWidgetItem*> selectItems = tableWidgets[ui->tabWidget->currentIndex()]->selectedItems();
    //    QList<int> selectRows;
    //    for(int i=0;i<selectItems.size();i++)
    //    {
    //        if(!selectRows.contains(selectItems[i]->row()))
    //            selectRows<<selectItems[i]->row();
    //    }
    //    std::sort(selectRows.begin(),selectRows.end());
    //    QString message="是否删除已选条目:\n";
    //    for(int i=selectRows.size()-1;i>=0;i--)
    //    {
    //        int row=selectRows[i];
    //        message+=sharedDataHelper.groupList[ui->tabWidget->currentIndex()]->at(row)->toString();
    //        message+="\n";
    //    }
    //    int choice=QMessageBox::question(this,"删除条目",message);
    //    if(choice==QMessageBox::Yes)
    //    {
    //        for(int i=selectRows.size()-1;i>=0;i--)
    //        {
    //            int row=selectRows[i];
    //            QSqlQuery query(customDataBase.getDatabase());
    //            if(customDataBase.open())
    //            {
    //                int index=ui->tabWidget->currentIndex();
    //                Group* currentGroup=sharedDataHelper.groupList.at(index);
    //                QString sql = QString("delete from '%1' where createTime = '%2' ").arg(currentGroup->getGroupName()).arg(currentGroup->at(row)->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
    //                query.exec(sql);
    //                currentGroup->removeAt(row);
    //                currentGroup->setLastEditTime();
    //                sql=QString("update sharedDataHelper.groupList set count='%1',lastEditTime='%2' where name='%3' ").arg(currentGroup->count()).arg(currentGroup->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss")).arg(currentGroup->getGroupName());
    //                query.exec(sql);
    //                tableWidgets[index]->removeRow(row);
    //            }
    //        }
    //    }
}
