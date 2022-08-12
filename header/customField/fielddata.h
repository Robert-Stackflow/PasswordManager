#ifndef FIELDDATA_H
#define FIELDDATA_H
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
class FieldData: public QObject
{
    friend class AbstractCustomField;
public:
    //枚举类
    enum isRequiredChoices{
        REQUIRED,
        NOTREQUIRED
    };
    enum controllerTypeChoices{
        LINEEDIT,
        COMBOBOX,
        TEXTEDIT
    };
    enum dataTypeChoices{
        NORMAL,
        PASSWORD,
        MAIL,
        MOBILE,
        WEBSITE
    };
    Q_ENUM(dataTypeChoices)
    Q_ENUM(isRequiredChoices)
    Q_ENUM(controllerTypeChoices)
    //静态常量
    static const QRegExp passwordRegExp;
    static const QValidator* passwordValidator;
    static const QRegExp websiteRegExp;
    static const QValidator* websiteValidator;
    static const QRegExp mobileRegExp;
    static const QValidator* mobileValidator;
    static const QRegExp mailRegExp;
    static const QValidator* mailValidator;
public:
    QString fieldName;
    QString fieldTypeName;
    QString placeholderText;
    dataTypeChoices dataType;
    isRequiredChoices isRequired;
    controllerTypeChoices controllerType;
public:
    FieldData(const QString& fieldName,
              controllerTypeChoices controllerType,
              isRequiredChoices isRequired=NOTREQUIRED,
              dataTypeChoices dataType=NORMAL,
              const QString& placeholderText="");
    FieldData(controllerTypeChoices controllerType,
              dataTypeChoices dataType=NORMAL);
    FieldData(const QString& fieldName,const FieldData& other,isRequiredChoices isRequired=NOTREQUIRED,const QString& placeholderText="");
    FieldData(const FieldData& other);
//    FieldData& operator=(FieldData&& other);
    FieldData& operator=(const FieldData& other);
    const QValidator* getValidator() const;
    QString toString()const;
    void setFieldTypeName(const QString& name);
    QStringList items();
private:
    void initPlaceholderText();
};
#endif // FIELDDATA_H
