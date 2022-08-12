#include "util/customdatabase.h"
#include "widget.h"
#include "ui_widget.h"
#include "util/databasehelper.h"
#include "util/datapathhelper.h"
#include "util/shareddatahelper.h"
#include "QsLog.h"
#include <QSqlQuery>
#pragma warning (disable:4100)
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
//void Widget::tableWidgetContextMenuRequested(QPoint pos)
//{
//    cursorPos=new QPoint;
//    *cursorPos=pos;
//    tableWidgetMenu->exec(QCursor::pos());
//}
//int Widget::fillMoveMenu(QMenu * menu, int type, int mode)
//{
//    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
//    int count=0;
//    for(int i=0;i<sharedDataHelper.groupList.count();i++){
//        if(sharedDataHelper.groupList[i]->getGroupType()==type&&sharedDataHelper.groupList[i]->getGroupName()!=sharedDataHelper.groupList[ui->tabWidget->currentIndex()]->getGroupName()){
//            count++;
//            QAction* temp=new QAction(sharedDataHelper.groupList[i]->getGroupName());
//            menu->addAction(temp);
//            if(mode==0)
//                connect(temp,SIGNAL(triggered()),this,SLOT(moveMenuSlot()));
//            else if(mode==1)
//                connect(temp,SIGNAL(triggered()),this,SLOT(copyMenuSlot()));
//        }
//    }
//    return count;
//}
//void Widget::openWebsiteSlot()
//{
//    int x=cursorPos->x();
//    int y=cursorPos->y();
//    QModelIndex index = tableWidgets[ui->tabWidget->currentIndex()]->indexAt(QPoint(x, y));
//    int row = index.row();
//    if(row==-1)
//        return;
//    QDesktopServices::openUrl(QUrl(sharedDataHelper.groupList->at(stackedWidget->currentIndex())->at(row), QUrl::TolerantMode));
//}
//void Widget::onCopyKeyItemInfoClicked()
//{
//    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
//    int x=cursorPos->x();
//    int y=cursorPos->y();
//    QModelIndex index = tableWidgets[ui->tabWidget->currentIndex()]->indexAt(QPoint(x, y));
//    int row = index.row();
//    if(row==-1)
//        return;
//    QClipboard *clip = QApplication::clipboard();
//    clip->setText(sharedDataHelper.groupList[ui->tabWidget->currentIndex()]->at(row)->toString());
//}
//void Widget::searchSlot(const QString& text)
//{
//    int rowCount=tableWidgets[ui->tabWidget->currentIndex()]->rowCount();
//    if (text=="")
//    {
//        for (int i = 0; i < rowCount; i++)
//            tableWidgets[ui->tabWidget->currentIndex()]->setRowHidden(i, false);
//    }
//    else
//    {
//        QList <QTableWidgetItem *> item = tableWidgets[ui->tabWidget->currentIndex()]->findItems(text,Qt::MatchCaseSensitive|Qt::MatchContains);
//        for (int i = 0; i < rowCount; i++)
//            tableWidgets[ui->tabWidget->currentIndex()]->setRowHidden(i, true);
//        if (!item.isEmpty()) {
//            for (int i = 0; i < item.count(); i++) {
//                tableWidgets[ui->tabWidget->currentIndex()]->setRowHidden(item.at(i)->row(),false);
//            }
//        }
//    }
//}
//void Widget::tableWidgetClickedSlot(int row,int column)
//{
//    //    int groupType=sharedDataHelper.groupList->at(stackedWidget->currentIndex())->getGroupType();
//    //    int certainColumn=0;
//    //    if(column==certainColumn)
//    //    {
//    //        if(tableWidgets[stackedWidget->currentIndex()]->item(row,column)->text()=="******")
//    //            tableWidgets[stackedWidget->currentIndex()]->item(row,column)->setText(sharedDataHelper.groupList->at(stackedWidget->currentIndex())->at(row)->password);
//    //        else
//    //            tableWidgets[stackedWidget->currentIndex()]->item(row,column)->setText("******");
//    //    }
//}
//void Widget::moveKeyItemSlot()
//{
//    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
//    CustomDataBase& customDataBase = CustomDataBase::instance();
//    QAction* send=(QAction*)sender();
//    QString newName=send->text();
//    int index=ui->tabWidget->currentIndex();
//    Group* currentGroup=sharedDataHelper.groupList[index];
//    QList<QTableWidgetItem*> selectItems = tableWidgets[ui->tabWidget->currentIndex()]->selectedItems();
//    QList<int> selectRows;
//    for(int i=0;i<selectItems.size();i++)
//    {
//        if(!selectRows.contains(selectItems[i]->row()))
//            selectRows<<selectItems[i]->row();
//    }
//    std::sort(selectRows.begin(),selectRows.end());
//    QString message=QString("是否将已选条目从 '%1' 移动到 '%2' :\n").arg(currentGroup->getGroupName()).arg(newName);
//    for(int i=selectRows.size()-1;i>=0;i--)
//    {
//        int row=selectRows[i];
//        message+=currentGroup->at(row)->toString();
//        message+="\n";
//    }
//    int choice=QMessageBox::question(this,"移动条目",message);
//    if(choice==QMessageBox::Yes)
//    {
//        for(int i=selectRows.size()-1;i>=0;i--)
//        {
//            int row=selectRows[i];
//            QSqlQuery query(customDataBase.getDatabase());
//            if(customDataBase.open())
//            {
//                //添加到新表中
//                newKeyItem=currentGroup->at(row);
//                newKeyItemFunction(newName);
//                //从原表中删除
//                QString sql = QString("delete from '%1' where createTime = '%2' ").arg(currentGroup->getGroupName()).arg(currentGroup->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
//                query.exec(sql);
//                currentGroup->removeAt(row);
//                currentGroup->setLastEditTime();
//                sql=QString("update sharedDataHelper.groupList set count='%1',lastEditTime='%2' where name='%3' ").arg(currentGroup->count()).arg(currentGroup->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss")).arg(currentGroup->getGroupName());
//                query.exec(sql);
//                tableWidgets[index]->removeRow(row);
//            }
//        }
//    }
//}
//void Widget::copyKeyItemSlot()
//{
//    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
//    QAction* send=(QAction*)sender();
//    QString newName=send->text();
//    int index=ui->tabWidget->currentIndex();
//    Group* currentGroup=sharedDataHelper.groupList[index];
//    QList<QTableWidgetItem*> selectItems = tableWidgets[ui->tabWidget->currentIndex()]->selectedItems();
//    QList<int> selectRows;
//    for(int i=0;i<selectItems.size();i++)
//    {
//        if(!selectRows.contains(selectItems[i]->row()))
//            selectRows<<selectItems[i]->row();
//    }
//    std::sort(selectRows.begin(),selectRows.end());
//    QString message=QString("是否将已选条目从 '%1' 移动到 '%2' :\n").arg(currentGroup->getGroupName()).arg(newName);
//    for(int i=selectRows.size()-1;i>=0;i--)
//    {
//        int row=selectRows[i];
//        message+=currentGroup->at(row)->toString();
//        message+="\n";
//    }
//    int choice=QMessageBox::question(this,"移动条目",message);
//    if(choice==QMessageBox::Yes)
//    {
//        for(int i=selectRows.size()-1;i>=0;i--)
//        {
//            int row=selectRows[i];
//            //添加到新表中
//            newKeyItem=new KeyItem(currentGroup->at(row)->getGroupType(),currentGroup->at(row)->getCreateTime(),currentGroup->at(row)->getLastEditTime(),sharedDataHelper.groupTypeList[index]->getFieldNames());
//            newKeyItemFunction(newName);
//        }
//    }
//}
