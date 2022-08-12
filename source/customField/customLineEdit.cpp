#include "customField/customLineEdit.h"
#include "ui_customLineEdit.h"
#include <QDebug>
#include <QMetaEnum>
#pragma warning (disable:4100)
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
customLineEdit::customLineEdit(const FieldData &fieldData, QWidget* parent) :
    AbstractCustomField(fieldData,parent),
    ui(new Ui::customLineEdit)
{
    ui->setupUi(this);
    //设置controllerLabel
    ui->controllerLabel->setAlignment(Qt::AlignCenter);
    ui->controllerLabel->setText(this->controllerLabelName);
    ui->controllerLabel->setStyleSheet("border:0px;background-color:transparent");
    //设置controllerJudge
    ui->controllerJudge->setStyleSheet("border:0px;background-color:transparent");
    //根据dataType初始化
    if(fieldData.dataType==FieldData::NORMAL){
        connect(ui->controller,SIGNAL(textChanged(QString)),this,SLOT(onControllerEdited(QString)));
    }else if(fieldData.dataType==FieldData::WEBSITE||fieldData.dataType==FieldData::MOBILE||fieldData.dataType==FieldData::MAIL){
        ui->controller->setValidator(fieldData.getValidator());
        connect(ui->controller,SIGNAL(textEdited(QString)),this,SLOT(onControllerEdited(QString)));
    }else if(fieldData.dataType==FieldData::PASSWORD){
        ui->controller->setValidator(fieldData.getValidator());
        //设置初始样式:不显示密码
        ui->controller->setEchoMode(QLineEdit::Password);
        ui->controllerJudge->setIcon(QIcon(":/custom/icons/hidepassword.png"));
        ui->controllerJudge->setStatusTip("nosee");
        ui->controllerJudge->setCursor(Qt::PointingHandCursor);
        //绑定槽函数
        connect(ui->controllerJudge,SIGNAL(clicked()),this,SLOT(onPasswordJudgeClicked()));
    }
    ui->controller->setPlaceholderText(fieldData.placeholderText);
}
void customLineEdit::setFocus()
{
    ui->controller->setFocus();
}
customLineEdit::~customLineEdit()
{
    delete ui;
}
bool customLineEdit::isValid(){
    int position=0;
    QString value=QString(ui->controller->text());
    return !((value==""&&fieldData.isRequired==FieldData::REQUIRED)||(value!=""&&fieldData.getValidator()!=nullptr&&QValidator::Acceptable!=fieldData.getValidator()->validate(value,position)));
}
void customLineEdit::onControllerEdited(const QString &arg)
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
void customLineEdit::onPasswordJudgeClicked()
{
    if(ui->controllerJudge->statusTip()=="nosee"){
        ui->controllerJudge->setIcon(QIcon(":/custom/icons/showpassword.png"));
        ui->controllerJudge->setStatusTip("see");
        ui->controller->setEchoMode(QLineEdit::Normal);
    }else if(ui->controllerJudge->statusTip()=="see"){
        ui->controllerJudge->setIcon(QIcon(":/custom/icons/hidepassword.png"));
        ui->controllerJudge->setStatusTip("nosee");
        ui->controller->setEchoMode(QLineEdit::Password);
    }
}
void customLineEdit::setPlaceholderText(const QString &placeholderText)
{
    fieldData.placeholderText=placeholderText;
    ui->controller->setPlaceholderText(fieldData.placeholderText);
}
QString customLineEdit::text(){
    return ui->controller->text();
}
void customLineEdit::setText(const QString &text)
{
    ui->controller->setText(text);
}
void customLineEdit::clear()
{
    ui->controller->clear();
    if(fieldData.dataType!=FieldData::PASSWORD){
        ui->controllerJudge->setIcon(QIcon());
        ui->controllerJudge->setStatusTip("");
    }
}
void customLineEdit::setMaxLength(int length){
    ui->controller->setMaxLength(length);
}
QString customLineEdit::getPlaceholderText(){
    return fieldData.placeholderText;
}
AbstractCustomField* customLineEdit::clone(){
    return new customLineEdit(fieldData,parent);
}
void customLineEdit::setCompleter(QCompleter* completer){
    ui->controller->setCompleter(completer);
}
