#include "structure/fieldTypes.h"
#include "qdebug.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
FieldTypes::FieldTypes(){
}
QStringList FieldTypes::getFieldTypeNames(){
    return nameList;
}
int FieldTypes::count(){
    return nameList.count();
}
FieldData &FieldTypes::operator [](const QString& name){
    return fieldDataList[nameList.indexOf(name)];
}
void FieldTypes::insert(const QString& name, const FieldData &fieldData){
    nameList<<name;
    fieldDataList<<fieldData;
    fieldDataList[nameList.indexOf(name)].setFieldTypeName(name);
}
