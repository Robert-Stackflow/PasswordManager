#include "dialogs/fielddialog.h"
#include "customField/abstractCustomField.h"
#include "customField/customTextEdit.h"
#include "customField/fieldfactory.h"
#include "ui_fielddialog.h"
#include "util/shareddatahelper.h"
#include "controller/custommessagebox.h"
#include "controller/floatbox.h"
#include <QEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
FieldDialog::FieldDialog(const QString &groupTypeName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FieldDialog)
{
    ui->setupUi(this);
    this->groupTypeName=groupTypeName;
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    if(!sharedDataHelper.groupTypeList.has(groupTypeName)){
        CustomMessageBox::critical(this,"错误","未找到分组类型,添加字段失败!");
        FloatBox::Fail("添加字段失败",this);
        this->close();
    }
    initWidget();
}
void FieldDialog::initWidget()
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    setWindowCustomTitle("Password Manager");
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(167,166,170));
    setGraphicsEffect(shadow);
    ui->dialogGridLayout->setMargin(20);
    fieldName=(customLineEdit*)FieldFactory::FieldController("字段名称",FieldData::LINEEDIT,FieldData::REQUIRED,FieldData::NORMAL,this,"输入字段名称");
    ui->infoLayout->addWidget(fieldName);
    fieldType=(customComboBox*)FieldFactory::FieldController("字段类型",FieldData::COMBOBOX,FieldData::REQUIRED,FieldData::NORMAL,this,"选择字段类型");
    ui->infoLayout->addWidget(fieldType);
    fieldPlaceholderText=(customLineEdit*)FieldFactory::FieldController("填写提示",FieldData::LINEEDIT,FieldData::NOTREQUIRED,FieldData::NORMAL,this,"输入填写提示");
    ui->infoLayout->addWidget(fieldPlaceholderText);
    fieldType->addItems(sharedDataHelper.fieldTypeList.getFieldTypeNames());
    QFile file(":qss/custom.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString temp = this->styleSheet;
        temp += QLatin1String(file.readAll());
        this->styleSheet=temp;
        this->setStyleSheet(styleSheet);
        file.close();
    }
    //绑定槽函数
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(onConfirmClicked()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(this,SIGNAL(ncActive(bool)),this,SLOT(onNcActived(bool)));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
}
void FieldDialog::onConfirmClicked(){
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    bool isUnique=!sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(groupTypeName)]->has(fieldName->text());
    if(fieldName->isValid()&&fieldType->isValid()&&fieldPlaceholderText->isValid()&&isUnique)
    {
        emit newFieldAccept(groupTypeName,FieldData(fieldName->text(),sharedDataHelper.fieldTypeList[fieldType->currentText()],FieldData::REQUIRED,fieldPlaceholderText->text()));
        this->close();
    }else{
        QString message="";
        if(!fieldName->isValid())
            message+="字段名称不合法!\n";
        if(!fieldType->isValid())
            message+="字段类型不合法!\n";
        if(!fieldPlaceholderText->isValid())
            message+="输入提示不合法!\n";
        if(!isUnique)
            message+="字段\""+fieldName->text()+"\"已存在!\n";
        CustomMessageBox::warning(this,"警告",message);
    }
}
void FieldDialog::onCancelClicked(){
    this->close();
}
FieldDialog::~FieldDialog()
{
    delete ui;
}
void FieldDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void FieldDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void FieldDialog::mouseReleaseEvent(QMouseEvent*)
{
    newField=nullptr;
    mousePressed = false;
}
void FieldDialog::onCloseClicked()
{
    this->close();
}
void FieldDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void FieldDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    this->setWindowTitle(newWindowTitle);
    ui->dialogTitleLabel->setText(newWindowTitle);
}
bool FieldDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void FieldDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
