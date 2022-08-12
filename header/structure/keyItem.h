#ifndef KEYITEM_H
#define KEYITEM_H
#include <QString>
#include <QDateTime>
class KeyItem{
private:
    QString groupName;
    QStringList fieldValueList;
    QDateTime createTime;
    QDateTime lastEditTime;
public:
    KeyItem(const QString& groupName,QStringList fieldValueList);
    KeyItem(const QString & groupName,QDateTime createTime,QDateTime lastEditTime,QStringList fieldValueList);
    QString getGroupName() const;
    const QDateTime &getCreateTime() const;
    const QDateTime &getLastEditTime() const;
    void setLastEditTime();
    QString toString();
    //重载运算符
    QString operator[](int index);
    KeyItem operator<<(QString newFieldValue);
    //增删元素与返回索引
    KeyItem* clone();
    QString at(int index);
    void append(QString newFieldValue);
};
#endif // KEYITEM_H
