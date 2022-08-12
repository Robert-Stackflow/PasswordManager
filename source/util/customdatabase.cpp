#include "util/customdatabase.h"
#include "util/databasehelper.h"
#include "util/datapathhelper.h"
#include "util/shareddatahelper.h"
#include "QsLog.h"
#include <QMessageBox>
#include <QSqlError>
CustomDataBase* CustomDataBase::sinstance=nullptr;
CustomDataBase::CustomDataBase()
{
    database=QSqlDatabase::addDatabase("QSQLITE",QString::asprintf("%d",0));
}
void CustomDataBase::setDatabaseName(const QString& name)
{
    database.setDatabaseName(name);
}
QString CustomDataBase::databaseName()
{
    return database.databaseName();
}
QStringList CustomDataBase::tables(){
    return database.tables();
}
bool CustomDataBase::open()
{
    SharedDataHelper& sharedData = SharedDataHelper::instance();
    QString username;
    if(sharedData.accountList.getActiveAccount()==nullptr)
        username="PUBLIC";
    else
        username=sharedData.accountList.getActiveAccount()->getUsername();
    bool flag=database.open();
    if(!flag){
        QsLogging::Logger::getFileLogger(username,"DATABASE");
        QLOG_WARN()<<"Database Connection Error:"<<database.databaseName()<<"!";
        QMessageBox::critical(0, QObject::tr("Database Connection Error!"), database.lastError().text());
    }else{
        QsLogging::Logger::getFileLogger(username,"DATABASE");
        QLOG_DEBUG()<<"Database Connection:"<<database.databaseName();
    }
    return flag;
}
QSqlError CustomDataBase::lastError(){
    return database.lastError();
}
void CustomDataBase::close()
{
    database.close();
}
QSqlDatabase& CustomDataBase::getDatabase()
{
    return database;
}
CustomDataBase& CustomDataBase::instance()
{
    if(!sinstance)
        sinstance=new CustomDataBase();
    return *sinstance;
}
void CustomDataBase::deleteInstance()
{
    delete sinstance;
}
