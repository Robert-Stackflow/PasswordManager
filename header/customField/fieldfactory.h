#ifndef FIELDFACTORY_H
#define FIELDFACTORY_H
#include "abstractCustomField.h"
#include <QObject>

class FieldFactory
{
public:
    FieldFactory();
    static AbstractCustomField* FieldController(const FieldData &fieldData, QWidget* parent=nullptr);
    static AbstractCustomField* FieldController(AbstractCustomField* field);
    static AbstractCustomField* FieldController(const QString& fieldName,FieldData::controllerTypeChoices controllerType,FieldData::isRequiredChoices isRequired=FieldData::NOTREQUIRED,FieldData::dataTypeChoices dataType=FieldData::NORMAL,QWidget* parent=nullptr,const QString& newPlaceholderText="");
};

#endif // FIELDFACTORY_H
