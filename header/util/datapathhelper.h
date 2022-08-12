#ifndef DATAPATHHELPER_H
#define DATAPATHHELPER_H
#include <QString>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
class DataPathHelper
{
private:
    static DataPathHelper* sinstance;
    //头像目录
    static QString portraitDir;
    //数据库目录
    static QString dataDir;
    //日志目录
    static QString logsDir;
    //默认头像路径
    static QString defaultPortraitPath;
    DataPathHelper();
public:
    static DataPathHelper& instance();
    static void deleteInstance();
    static QString getPortraitDir();
    static QString getDataDir();
    static QString getLogsDir();
    static QString getAccountsDataBasePath();
    static QString getCurrentAccountDataBasePath();
    static QString getDefaultPortraitPath();
};

#endif // DATAPATHHELPER_H
