#include "util/databasehelper.h"
DataBaseHelper* DataBaseHelper::sinstance=nullptr;
QString DataBaseHelper::settingsTableName="settings";
QString DataBaseHelper::accountsTableName="accounts";
QString DataBaseHelper::groupsTableName="groups";
QString DataBaseHelper::groupTypesTableName="groupTypes";
QString DataBaseHelper::autofillInfoTableName="autofillInfo";
QString DataBaseHelper::groupTableNamePrefix="group_";
QString DataBaseHelper::groupTypeTableNamePrefix="groupType_";
QString DataBaseHelper::getSettingsTableName()
{
    return settingsTableName;
}

DataBaseHelper::DataBaseHelper(){
}
DataBaseHelper& DataBaseHelper::instace(){
    if (!sinstance)
        sinstance = new DataBaseHelper();
    return *sinstance;
}
void DataBaseHelper::deleteInstace(){
    delete sinstance;
}
QString DataBaseHelper::getAccountsTableName(){
    return accountsTableName;
}
QString DataBaseHelper::getGroupsTableName(){
    return groupsTableName;
}
QString DataBaseHelper::getGroupTypesTableName(){
    return groupTypesTableName;
}
QString DataBaseHelper::getAutofillInfoTableName(){
    return autofillInfoTableName;
}
QString DataBaseHelper::getGroupTableName(QString groupName){
    return groupTableNamePrefix+groupName;
}
QString DataBaseHelper::getGroupTypeTableName(QString groupTypeName){
    return groupTypeTableNamePrefix+groupTypeName;
}
