#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QString>
#include <QDateTime>
#include <QPixmap>
class Account
{
private:
    QPixmap portrait;
    QString username;
    QDateTime createTime;
    QDateTime lastEditTime;
    QString portraitPath;
    QString originalPortraitPath;
public:
    //构造函数
    Account();
    Account(const QString &newUsername,const QString &newPortraitPath="");
    Account(const QString &newUsername,QDateTime newCreateTime,QDateTime newLastEditTime,const QString &newPortraitPath);
    //Getter和Setter
    const QString &getUsername() const;
    void setUsername(const QString &newUsername);
    const QString &getPassword() const;
    void setPassword(const QString &newPassword);
    const QDateTime &getCreateTime() const;
    const QDateTime &getLastEditTime() const;
    void setLastEditTime();
    const QString &getPortraitPath() const;
    void setPortraitPath(const QString &newPortraitPath);
    QString toString();
    const QString &getOriginalPortraitPath() const;
    void setOriginalPortraitPath(const QString &newOriginalPortraitPath);
    QPixmap getPortrait() const;
    void setPortrait(QPixmap newPortrait);
};

#endif // ACCOUNT_H
