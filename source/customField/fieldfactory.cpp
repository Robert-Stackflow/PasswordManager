#include "customField/fieldfactory.h"
#include "customField/customComboBox.h"
#include "customField/customLineEdit.h"
#include "customField/customTextEdit.h"
AbstractCustomField* FieldFactory::FieldController(const QString& fieldName,
                                                   FieldData::controllerTypeChoices controllerType,
                                                   FieldData::isRequiredChoices isRequired,
                                                   FieldData::dataTypeChoices dataType,
                                                   QWidget* parent,
                                                   const QString& placeholderText)
{
    FieldData fieldData(fieldName,controllerType,isRequired,dataType,placeholderText);
    return FieldController(fieldData,parent);
}
AbstractCustomField* FieldFactory::FieldController(AbstractCustomField* field){
    return field->clone();
}
AbstractCustomField* FieldFactory::FieldController(const FieldData& fieldData,QWidget* parent)
{
    AbstractCustomField* newField=nullptr;
    if(fieldData.controllerType==FieldData::LINEEDIT)
        newField=new customLineEdit(fieldData,parent);
    else if(fieldData.controllerType==FieldData::COMBOBOX)
        newField=new customComboBox(fieldData,parent);
    else if(fieldData.controllerType==FieldData::TEXTEDIT)
        newField=new customTextEdit(fieldData,parent);
    if(newField!=nullptr&&fieldData.placeholderText!="")
        newField->setPlaceholderText(fieldData.placeholderText);
    return newField;
}
