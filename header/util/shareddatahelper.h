#ifndef SHAREDDATAHELPER_H
#define SHAREDDATAHELPER_H
#include "structure/accounts.h"
#include "structure/autofillInfo.h"
#include "structure/fieldTypes.h"
#include "structure/groupTypes.h"
#include "structure/groups.h"
#include "structure/account.h"
#include <QSqlDatabase>
class SharedDataHelper{
private:
    static SharedDataHelper* sinstance;
public:
    static Groups groupList;
    static Accounts accountList;
    static GroupTypes groupTypeList;
    static FieldTypes fieldTypeList;
    static AutofillInfo autofillInfo;
    static QSqlDatabase database;
    SharedDataHelper();
public:
    static SharedDataHelper& instance();
    static void deleteInstace();
};
#endif // SHAREDDATAHELPER_H
