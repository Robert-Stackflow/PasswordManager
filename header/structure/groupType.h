#ifndef GROUPTYPE_H
#define GROUPTYPE_H
#include <QDateTime>
#include "customField/abstractCustomField.h"
class GroupType{
private:
    QString groupTypeName;
    QString describe;
    QDateTime createTime;
    QDateTime lastEditTime;
    QList<FieldData> customFieldList;
public:
    //构造函数
    GroupType(QString groupTypeName, QString describe, QList<FieldData> customFieldList);
    GroupType(QString groupTypeName,QDateTime createTime,QDateTime lastEditTime,QString describe);
    //Getter和Setter
    const QString &getDescribe() const;
    void setDescribe(const QString &newDescribe);
    const QDateTime &getCreateTime() const;
    const QDateTime &getLastEditTime() const;
    void setLastEditTime();
    const QString &getGroupTypeName() const;
    void setGroupTypeName(const QString &newGroupTypeName);
    void setCustomFieldList(const QList<FieldData> &newCustomFieldList);
    //基本属性
    int count();
    void clear();
    GroupType* clone();
    bool has(const QString &name);
    int index(const QString &name);
    FieldData at(int index);
    bool removeAt(int index);
    bool removeOne(const QString &name);
    bool remove(const QStringList &names);
    void append(const FieldData &newField);
    //重载运算符
    FieldData operator[](int index);
    GroupType operator<<(const FieldData &newField);
    //获取字段名称列表
    QStringList getFieldNames();
    //获取分组类型对应的sql语句
    QString getCreateSql(const QString& tableName);
    QString getUpdateSql(const QString &tableName);
    QString getInsertSql(const QString &tableName);
    //获取列表信息
    QString getListViewInfo();
};
#endif // GROUPTYPE_H
