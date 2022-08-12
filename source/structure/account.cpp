#include "structure/account.h"
QPixmap Account::getPortrait() const
{
    return portrait;
}

void Account::setPortrait(QPixmap newPortrait)
{
    portrait = newPortrait;
}

Account::Account()
{
    //无参构造函数
    this->createTime=QDateTime::currentDateTime();
    this->lastEditTime=QDateTime::currentDateTime();
}
Account::Account(const QString &newUsername,const QString &newPortraitPath)
{
    //用于新建用户
    this->username=newUsername;
    this->portraitPath=newPortraitPath;
    this->createTime=QDateTime::currentDateTime();
    this->lastEditTime=QDateTime::currentDateTime();
}
Account::Account(const QString &newUsername,QDateTime newCreateTime,QDateTime newLastEditTime,const QString &newPortraitPath)
{
    //用于加载用户信息
    this->username=newUsername;
    this->createTime=newCreateTime;
    this->lastEditTime=newLastEditTime;
    this->portraitPath=newPortraitPath;
}
const QString &Account::getUsername() const
{
    return username;
}

void Account::setUsername(const QString &newUsername)
{
    username = newUsername;
}

const QDateTime &Account::getCreateTime() const
{
    return createTime;
}

const QDateTime &Account::getLastEditTime() const
{
    return lastEditTime;
}

void Account::setLastEditTime()
{
    lastEditTime = QDateTime::currentDateTime();
}

const QString &Account::getPortraitPath() const
{
    return portraitPath;
}

void Account::setPortraitPath(const QString &newPortraitPath)
{
    portraitPath = newPortraitPath;
}
const QString &Account::getOriginalPortraitPath() const
{
    return originalPortraitPath;
}

void Account::setOriginalPortraitPath(const QString &newOriginalPortraitPath)
{
    originalPortraitPath = newOriginalPortraitPath;
}
QString Account::toString(){
    QString str=QString("username:'%1',portraitPath:'%2',createTime:'%3',lastEditTime:'%4'")
            .arg(username)
            .arg(portraitPath)
            .arg(createTime.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(lastEditTime.toString("yyyy-MM-dd hh:mm:ss"));
    return str;
}
