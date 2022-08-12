#include "customField/customTextEdit.h"
#include "ui_customTextEdit.h"
#include <QDebug>
#include <QMetaEnum>
#pragma warning (disable:4100)
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
customTextEdit::customTextEdit(const FieldData &fieldData, QWidget* parent):
    AbstractCustomField(fieldData,parent),
    ui(new Ui::customTextEdit)
{
    ui->setupUi(this);
    //设置controllerLabel
    ui->controllerLabel->setText(this->controllerLabelName);
    ui->controllerLabel->setAlignment(Qt::AlignCenter);
    ui->controllerLabel->setStyleSheet("border:0px;background-color:transparent");
    //设置controllerJudge
    ui->controllerJudge->setStyleSheet("border:0px;background-color:transparent");
    if(fieldData.dataType==FieldData::NORMAL){
        connect(ui->controller,SIGNAL(textChanged()),this,SLOT(onControllerEdited()));
    }
    ui->controller->setPlaceholderText(fieldData.placeholderText);
}
void customTextEdit::onControllerEdited(){
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
void customTextEdit::setFocus()
{
    ui->controller->setFocus();
}
void customTextEdit::clear(){
    ui->controller->clear();
}
void customTextEdit::setPlaceholderText(const QString& placeholderText){
    fieldData.placeholderText=placeholderText;
    ui->controller->setPlaceholderText(fieldData.placeholderText);
}
QString customTextEdit::toPlainText(){
    return ui->controller->toPlainText();
}
QString customTextEdit::text(){
    return toPlainText();
}
void customTextEdit::setText(const QString &text){
    ui->controller->setText(text);
}
QString customTextEdit::getPlaceholderText(){
    return fieldData.placeholderText;
}
bool customTextEdit::isValid(){
    if(fieldData.isRequired==FieldData::REQUIRED&&toPlainText()=="")
        return false;
    return true;
}

customTextEdit::~customTextEdit(){
    delete ui;
}
AbstractCustomField* customTextEdit::clone(){
    return new customTextEdit(fieldData,parent);
}
