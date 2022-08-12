#include "widget.h"
#include "ui_widget.h"
#include "util/databasetablenamegetter.h"
#include "util/datapathgetter.h"
#include "util/shareddata.h"
#include "QsLog.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
void Widget::saveMenuTriggered(){
    QAction* send=(QAction*)sender();
    ui->save->setDefaultAction(send);
}
void Widget::saveSlot(){
    SharedData& sharedData = SharedData::instance();
    Group* currentGroup=sharedData.groupList[ui->tabWidget->currentIndex()];
    QAction* activeAction=((QToolButton*)sender())->defaultAction();
    QString title="将分组 "+currentGroup->getGroupName()+" 另存为"+ui->save->defaultAction()->text()+"文件";
    if(activeAction==saveAsPDF){
        QString saveFilePath=QFileDialog::getSaveFileName(this,title,"./",tr("PDF files(*.pdf)"));
        if(saveFilePath=="")
            return;
        QDesktopServices::openUrl(QUrl(saveFilePath));
    }
    else if(activeAction==saveAsExcel){
        QString saveFilePath=QFileDialog::getSaveFileName(this,title,"./",tr("Xlsx files(*.xlsx)"));
        if(saveFilePath=="")
            return;
        QXlsx::Document xlsxW;
        for(int i=0,row=1;i<sharedData.groupList.count();i++)
        {
            Group* currentGroup=sharedData.groupList[i];
            QStringList header=sharedData.groupTypeList[currentGroup->getGroupType()]->getFieldNames();
            xlsxW.addSheet(currentGroup->getGroupName());
            xlsxW.selectSheet(currentGroup->getGroupName());
            for(int j=0;j<header.count();j++){
                xlsxW.write(QString('A'+j)+QString::number(row),header[j]);
                for(row=2;row<currentGroup->count();row++){
                    xlsxW.write(QString('A')+QString::number(row),currentGroup->at(row)->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
                    xlsxW.write(QString('A'+1)+QString::number(row),currentGroup->at(row)->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"));
                }
            }
        }
        xlsxW.selectSheet(sharedData.groupList[0]->getGroupName());
        if (xlsxW.saveAs(saveFilePath)){
            qDebug()<<"[Info] success to write xlsx file";
        }
        QDesktopServices::openUrl(QUrl(saveFilePath));
    }
    else if(activeAction==saveAsCSV){
        QString saveFilePath=QFileDialog::getSaveFileName(this,title,"./",tr("CSV files(*.csv)"));
        if(saveFilePath=="")
            return;
        QFile file(saveFilePath);
        if(!file.open(QIODevice::WriteOnly))
            return;
        int index=ui->tabWidget->currentIndex();
        Group* currentGroup=sharedData.groupList[index];
        QStringList header=sharedData.groupTypeList[currentGroup->getGroupType()]->getFieldNames();
        //写入表头
        for(int j=0;j<header.count();j++){
            file.write(header[j].toLocal8Bit().data());
            if(j<header.count()-1)
                file.write(",");
            else
                file.write("\n");
        }
        for(int row=0;row<currentGroup->count();row++){
            file.write(currentGroup->at(row)->getCreateTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().data());
            file.write(",");
            file.write(currentGroup->at(row)->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit().data());
            file.write("\n");
        }
        QDesktopServices::openUrl(QUrl(saveFilePath));
        file.close();
    }
}
