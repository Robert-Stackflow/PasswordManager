#include "customField/abstractCustomField.h"
#include <QMetaEnum>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
const FieldData &AbstractCustomField::getFieldData() const
{
    return fieldData;
}

AbstractCustomField::AbstractCustomField(const FieldData &fieldData,
                                         QWidget* parent):
    QWidget(parent),
    fieldData(fieldData)
{
    this->parent=parent;
    this->controllerLabelName=fieldData.fieldName;
    if(fieldData.isRequired==FieldData::REQUIRED){
        this->controllerLabelName='*'+this->controllerLabelName;
    }
    this->controllerLabelName+=':';
}
AbstractCustomField::~AbstractCustomField()
{
}
QString AbstractCustomField::toString(){
    return fieldData.toString();
}
void AbstractCustomField::setIsRequired(FieldData::isRequiredChoices newIsRequired)
{
    fieldData.isRequired = newIsRequired;
    if(fieldData.isRequired==FieldData::REQUIRED)
        controllerLabelName="*"+controllerLabelName;
    else
        controllerLabelName=controllerLabelName.right(this->controllerLabelName.length()-1);
}
