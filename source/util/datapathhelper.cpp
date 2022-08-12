#include "util/datapathhelper.h"
#include "util/shareddatahelper.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
DataPathHelper* DataPathHelper::sinstance=nullptr;
QString DataPathHelper::portraitDir="portrait/";
QString DataPathHelper::dataDir="data/";
QString DataPathHelper::logsDir="logs/";
QString DataPathHelper::defaultPortraitPath=":custom/icons/portrait.png";
DataPathHelper::DataPathHelper(){
}
DataPathHelper& DataPathHelper::instance(){
    if(!sinstance)
        sinstance=new DataPathHelper();
    return *sinstance;
}
void DataPathHelper::deleteInstance(){
    delete sinstance;
}
QString DataPathHelper::getPortraitDir()
{
    return portraitDir;
}

QString DataPathHelper::getDataDir()
{
    return dataDir;
}
QString DataPathHelper::getLogsDir()
{
    return logsDir;
}
QString DataPathHelper::getAccountsDataBasePath()
{
    return dataDir+"accounts.pwd";
}

QString DataPathHelper::getCurrentAccountDataBasePath()
{
    SharedDataHelper& sharedData = SharedDataHelper::instance();
    return dataDir+QString::number(sharedData.accountList.getActiveAccount()->getCreateTime().toTime_t())+".pwd";
}
QString DataPathHelper::getDefaultPortraitPath()
{
    return defaultPortraitPath;
}
