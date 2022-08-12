#include "customField/customComboBox.h"
#include "qcompleter.h"
#include "ui_customComboBox.h"
#include "util/shareddatahelper.h"
#include <QDebug>
#include <QMetaEnum>
#pragma warning (disable:4100)
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
customComboBox::customComboBox(const FieldData &fieldData,
                               QWidget* parent):
    AbstractCustomField(fieldData,parent),
    ui(new Ui::customComboBox)
{
    ui->setupUi(this);
    //设置controller
    ui->controller->setEditable(true);
    //设置controllerLabel
    ui->controllerLabel->setAlignment(Qt::AlignCenter);
    ui->controllerLabel->setText(this->controllerLabelName);
    ui->controllerLabel->setStyleSheet("border:0px;background-color:transparent");
    //设置controllerJudge
    ui->controllerJudge->setStyleSheet("border:0px;background-color:transparent");
    //根据dataType初始化
    if(fieldData.dataType==FieldData::NORMAL){
        connect(ui->controller,SIGNAL(currentTextChanged(QString)),this,SLOT(onControllerEdited(QString)));
        connect(ui->controller,SIGNAL(currentIndexChanged(int)),this,SLOT(onControllerCurrentIndexChanged(int)));
    }else if(fieldData.dataType==FieldData::WEBSITE||fieldData.dataType==FieldData::MOBILE||fieldData.dataType==FieldData::MAIL){
        ui->controller->setValidator(fieldData.getValidator());
        connect(ui->controller,SIGNAL(currentTextChanged(QString)),this,SLOT(onControllerEdited(QString)));
        connect(ui->controller,SIGNAL(currentIndexChanged(int)),this,SLOT(onControllerCurrentIndexChanged(int)));
    }
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    QCompleter* completer=nullptr;
    switch(fieldData.dataType){
    case FieldData::NORMAL:
    case FieldData::PASSWORD:
        break;
    case FieldData::WEBSITE:
        completer=new QCompleter(sharedDataHelper.autofillInfo.getWebsites());
        ui->controller->addItems(sharedDataHelper.autofillInfo.getWebsites());
        break;
    case FieldData::MAIL:
        completer=new QCompleter(sharedDataHelper.autofillInfo.getMails());
        ui->controller->addItems(sharedDataHelper.autofillInfo.getMails());
        break;
    case FieldData::MOBILE:
        completer=new QCompleter(sharedDataHelper.autofillInfo.getMobiles());
        ui->controller->addItems(sharedDataHelper.autofillInfo.getMobiles());
        break;
    }
    if(completer!=nullptr)
        ui->controller->setCompleter(completer);
    ui->controller->setCurrentIndex(-1);
    ui->controller->setPlaceholderText(fieldData.placeholderText);
    ui->controller->lineEdit()->setPlaceholderText(fieldData.placeholderText);
}
void customComboBox::setFocus()
{
    ui->controller->setFocus();
}
customComboBox::~customComboBox()
{
    delete ui;
}
bool customComboBox::isValid(){
    int position=0;
    QString value=QString(ui->controller->currentText());
    return !((value==""&&fieldData.isRequired==FieldData::REQUIRED)||(value!=""&&fieldData.getValidator()!=nullptr&&QValidator::Acceptable!=fieldData.getValidator()->validate(value,position)));
}
void customComboBox::onControllerCurrentIndexChanged(int){
    emit currentIndexChanged();
}
void customComboBox::onControllerEdited(const QString &arg)
{
    if(fieldData.isRequired==FieldData::NOTREQUIRED&&fieldData.dataType==FieldData::NORMAL){
        ui->controllerJudge->setIcon(QIcon());
        ui->controllerJudge->setStatusTip("");
    }else if(!isValid()){
        ui->controllerJudge->setIcon(QIcon(":custom/icons/wrong.png"));
        ui->controllerJudge->setStatusTip("wrong");
    }else{
        ui->controllerJudge->setIcon(QIcon(":custom/icons/right.png"));
        ui->controllerJudge->setStatusTip("right");
    }
}
void customComboBox::setPlaceholderText(const QString &placeholderText){
    fieldData.placeholderText=placeholderText;
    ui->controller->setPlaceholderText(fieldData.placeholderText);
    ui->controller->lineEdit()->setPlaceholderText(fieldData.placeholderText);
}
QString customComboBox::getPlaceholderText(){
    return fieldData.placeholderText;
}
int customComboBox::count(){
    return ui->controller->count();
}
void customComboBox::addItem(const QString& item){
    ui->controller->addItem(item);
}
void customComboBox::addItems(const QStringList& items){
    ui->controller->addItems(items);
}
void customComboBox::addItems(const QList<QString> &items){
    QStringList itemList;
    for(int i=0;i<items.count();i++)
        itemList<<items[i];
    ui->controller->addItems(itemList);
}
void customComboBox::clear(){
    ui->controller->clear();
}
QStringList customComboBox::items()
{
    QStringList itemList;
    for(int i=0;i<ui->controller->count();i++)
        itemList<<ui->controller->itemText(i),qDebug()<<itemList;
    return itemList;
}
int customComboBox::currentIndex(){
    return ui->controller->currentIndex();
}
QString customComboBox::currentText(){
    return ui->controller->currentText();
}
QString customComboBox::text(){
    return currentText();
}
void customComboBox::setCurrentIndex(int index){
    ui->controller->setCurrentIndex(index);
}
void customComboBox::setCurrentText(const QString& text){
    ui->controller->setCurrentText(text);
}
void customComboBox::setEnable(bool enable){
    ui->controller->setEnabled(enable);
}
void customComboBox::setEditable(bool editable){
    ui->controller->setEditable(editable);
}
bool customComboBox::isEnable(){
    return ui->controller->isEnabled();
}
bool customComboBox::isEditable(){
    return ui->controller->isEditable();
}
AbstractCustomField* customComboBox::clone(){
    return new customComboBox(fieldData,parent);
}
void customComboBox::setCompleter(QCompleter* completer){
    ui->controller->setCompleter(completer);
}
