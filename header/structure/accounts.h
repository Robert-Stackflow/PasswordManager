#ifndef ACCOUNTS_H
#define ACCOUNTS_H
#include "account.h"
class Accounts
{
private:
    Account* activeAccount;
    QList<Account*> accountList;
public:
    Accounts();
    //重载运算符
    Account *operator[](int index);
    Accounts operator<<(Account* newAccount);
    //基本操作
    int count();
    Account *at(int index);
    void append(Account* newAccount);
    bool has(const QString & username);
    int index(const QString & username);
    //获取与设置当前用户
    Account *getActiveAccount() const;
    void setActiveAccount(const QString &username);
    //获取用户名列表
    QStringList getUsernames();
};

#endif // ACCOUNTS_H
