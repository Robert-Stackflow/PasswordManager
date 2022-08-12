#ifndef CUSTOMDATABASE_H
#define CUSTOMDATABASE_H

#include <QObject>
#include <QSqlDatabase>

class CustomDataBase
{
private:
    static CustomDataBase* sinstance;
    QSqlDatabase database;
public:
    CustomDataBase();
    bool open();
    void close();
    QString databaseName();
    QStringList tables();
    QSqlError lastError();
    void setDatabaseName(const QString &name);
    QSqlDatabase& getDatabase();
    static CustomDataBase& instance();
    static void deleteInstance();
};

#endif // CUSTOMDATABASE_H
