#ifndef FIELDTYPES_H
#define FIELDTYPES_H
#include "customField/fielddata.h"
class FieldTypes{
private:
    QStringList nameList;
    QList<FieldData> fieldDataList;
public:
    FieldTypes();
    int count();
    QStringList getFieldTypeNames();
    FieldData& operator[](const QString& key);
    void insert(const QString& key,const FieldData& value);
};
#endif // FIELDTYPES_H
