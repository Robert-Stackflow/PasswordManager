#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H
#include <QString>
#include <QStringList>
class DataBaseHelper
{
private:
    //单个实例
    static DataBaseHelper* sinstance;
    //-账户数据-表名
    static QString accountsTableName;
    //-用户设置-表名
    static QString settingsTableName;
    //-分组管理-表名
    static QString groupsTableName;
    //-分组类型管理-表名
    static QString groupTypesTableName;
    //-个人信息-表名
    static QString autofillInfoTableName;
    //-各个分组-表名前缀
    static QString groupTableNamePrefix;
    //-各个分组类型-表名前缀
    static QString groupTypeTableNamePrefix;
    DataBaseHelper();
public:
    static DataBaseHelper& instace();
    static void deleteInstace();
    static QString getAccountsTableName();
    static QString getGroupsTableName();
    static QString getGroupTypesTableName();
    static QString getAutofillInfoTableName();
    static QString getGroupTableName(QString groupName);
    static QString getGroupTypeTableName(QString groupTypeName);
    static QString getSettingsTableName();
};
#endif // DATABASEHELPER_H
