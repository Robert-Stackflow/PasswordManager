#include "customField/fielddata.h"
#include "qdebug.h"
#include "qmetaobject.h"
#include "util/shareddatahelper.h"
const QRegExp FieldData::passwordRegExp=QRegExp("[a-zA-Z0-9.@#%*+*/]+");
const QValidator* FieldData::passwordValidator=new QRegExpValidator(FieldData::passwordRegExp,0);
const QRegExp FieldData::websiteRegExp=QRegExp("^(http|https)://([\\w-]+\\.)+[\\w-]+(/[\\w-./?%&=]*)?$");
const QValidator* FieldData::websiteValidator=new QRegExpValidator(FieldData::websiteRegExp,0);
const QRegExp FieldData::mobileRegExp=QRegExp("^1(3\\d|4[5-9]|5[0-35-9]|6[2567]|7[0-8]|8\\d|9[0-35-9])\\d{8}$");
const QValidator* FieldData::mobileValidator=new QRegExpValidator(FieldData::mobileRegExp,0);
const QRegExp FieldData::mailRegExp=QRegExp("^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$");
const QValidator* FieldData::mailValidator=new QRegExpValidator(FieldData::mailRegExp,0);
FieldData::FieldData(const QString& fieldName,
                     controllerTypeChoices controllerType,
                     isRequiredChoices isRequired,
                     dataTypeChoices dataType,
                     const QString& placeholderText):
    fieldName(fieldName),
    placeholderText(placeholderText),
    dataType(dataType),
    isRequired(isRequired),
    controllerType(controllerType)
{
    initPlaceholderText();
}
FieldData &FieldData::operator=(const FieldData &other)
{
    this->fieldName=other.fieldName;
    this->placeholderText=other.placeholderText;
    this->dataType=other.dataType;
    this->isRequired=other.isRequired;
    this->controllerType=other.controllerType;
    this->fieldTypeName=other.fieldTypeName;
    return *this;
}
FieldData::FieldData(const FieldData& other)
{
    this->fieldName=other.fieldName;
    this->placeholderText=other.placeholderText;
    this->dataType=other.dataType;
    this->isRequired=other.isRequired;
    this->controllerType=other.controllerType;
    this->fieldTypeName=other.fieldTypeName;
    initPlaceholderText();
}
FieldData::FieldData(controllerTypeChoices controllerType,dataTypeChoices dataType)
{
    this->dataType=dataType;
    this->controllerType=controllerType;
    initPlaceholderText();
}
FieldData::FieldData(const QString& fieldName, const FieldData &other, isRequiredChoices isRequired, const QString& placeholderText)
{
    this->fieldName=fieldName;
    this->placeholderText=placeholderText;
    this->dataType=other.dataType;
    this->fieldTypeName=other.fieldTypeName;
    this->isRequired=isRequired;
    this->controllerType=other.controllerType;
    initPlaceholderText();
}
const QValidator* FieldData::getValidator() const
{
    const QValidator* validator=nullptr;
    if(dataType==MOBILE)
        validator=mobileValidator;
    else if(dataType==MAIL)
        validator=mailValidator;
    else if(dataType==WEBSITE)
        validator=websiteValidator;
    else if(dataType==PASSWORD)
        validator=passwordValidator;
    return validator;
}
QString FieldData::toString()const
{
    return QString(QString("{fieldName}:")+QString(fieldName)
                   +QString(",{controllerType}:")+QString(QMetaEnum::fromType<FieldData::controllerTypeChoices>().valueToKey(controllerType))
                   +QString(",{isRequired}:")+QString(QMetaEnum::fromType<FieldData::isRequiredChoices>().valueToKey(isRequired))
                   +QString(",{dataType}:")+QString(QMetaEnum::fromType<FieldData::dataTypeChoices>().valueToKey(dataType)));
}
void FieldData::setFieldTypeName(const QString& name)
{
    this->fieldTypeName=name;
}
void FieldData::initPlaceholderText(){
    if(placeholderText==""){
        switch(dataType){
        case FieldData::NORMAL:
            this->placeholderText=QString("");
            break;
        case FieldData::PASSWORD:
            this->placeholderText=QString("Enter Password");
            break;
        case FieldData::WEBSITE:
            this->placeholderText=QString("Enter website");
            break;
        case MAIL:
            this->placeholderText=QString("Enter email");
            break;
        case MOBILE:
            this->placeholderText=QString("Enter phone number");
            break;
        }
    }
}
QStringList FieldData::items()
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    switch(dataType){
    case FieldData::NORMAL:
    case FieldData::PASSWORD:
        break;
    case FieldData::WEBSITE:
        return sharedDataHelper.autofillInfo.getWebsites();
        break;
    case FieldData::MAIL:
        return sharedDataHelper.autofillInfo.getMails();
        break;
    case FieldData::MOBILE:
        return sharedDataHelper.autofillInfo.getMobiles();
        break;
    }
    return QStringList();
}
