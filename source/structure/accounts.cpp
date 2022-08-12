#include "structure/accounts.h"
Account *Accounts::getActiveAccount() const
{
    return activeAccount;
}

void Accounts::setActiveAccount(const QString& username)
{
    for(int i=0;i<count();i++)
        if(accountList[i]->getUsername()==username)
            activeAccount = accountList[i];
}

Accounts::Accounts(){
    activeAccount=nullptr;
}
Accounts Accounts::operator<<(Account* newAccount){
    accountList<<newAccount;
    return *this;
}
int Accounts::count(){
    return accountList.count();
}
bool Accounts::has(const QString & username){
if(index(username)==-1)
    return false;
else
    return true;
}
int Accounts::index(const QString & username){
    for(int i=0;i<count();i++)
        if(accountList[i]->getUsername()==username)
            return i;
    return -1;
}
Account* Accounts::operator[](int index){
    return accountList[index];
}
Account *Accounts::at(int index){
    return accountList[index];
}
void Accounts::append(Account* newAccount){
    accountList<<newAccount;
}
QStringList Accounts::getUsernames(){
    QStringList list;
    for(int i=0;i<count();i++)
        list<<accountList[i]->getUsername();
    return list;
}
