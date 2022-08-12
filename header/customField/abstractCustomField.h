#ifndef ABSTRACTCUSTOMFIELD_H
#define ABSTRACTCUSTOMFIELD_H
#include "customField/fielddata.h"
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
class AbstractCustomField :public QWidget{
    Q_OBJECT
protected:
    //控件
    QWidget* parent;
    FieldData fieldData;
    QString controllerLabelName;
public:
    //构造函数/析构函数
    explicit AbstractCustomField(const FieldData& fieldData,QWidget* parent=nullptr);
    ~AbstractCustomField();
    void setIsRequired(FieldData::isRequiredChoices newIsRequired);
    //虚函数
    virtual QString toString();
    //抽象函数
    virtual bool isValid()=0;
    virtual QString text()=0;
    virtual void setFocus()=0;
    virtual AbstractCustomField* clone()=0;
    virtual QString getPlaceholderText()=0;
    virtual void setPlaceholderText(const QString& placeholderText)=0;
    const FieldData &getFieldData() const;
};

#endif // ABSTRACTCUSTOMFIELD_H
