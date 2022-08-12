#include "dialogs/keyitemdialog.h"
#include "qdebug.h"
#include "ui_keyitemdialog.h"
#include "util/shareddatahelper.h"
#include "controller/custommessagebox.h"
#include "controller/floatbox.h"
#include "customField/fieldfactory.h"
#include <QEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
KeyItemDialog::KeyItemDialog(const QString &groupName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyItemDialog)
{
    ui->setupUi(this);
    this->groupName=groupName;
    initWidget();
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    if(!sharedDataHelper.groupList.has(groupName)){
        CustomMessageBox::critical(this,"错误","未找到分组,添加条目失败!");
        FloatBox::Fail("添加条目失败",this);
        this->close();
    }else{
        GroupType* groupType=sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(sharedDataHelper.groupList[sharedDataHelper.groupList.index(groupName)]->getGroupTypeName())];
        if(groupType==nullptr)
            return;
        for(int i=0;i<groupType->count();i++){
            AbstractCustomField* field=FieldFactory::FieldController(groupType->at(i));
            field->setParent(this);
            infoLayout->addWidget(field);
            controllerList<<field;
        }
    }
}
void KeyItemDialog::initWidget()
{
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
    infoLayout=new QVBoxLayout(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setLayout(infoLayout);
    setWindowCustomTitle("在分组\""+groupName+"\"中添加条目");
    //绑定槽函数
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(onConfirmClicked()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(this,SIGNAL(ncActive(bool)),this,SLOT(onNcActived(bool)));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
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
}
void KeyItemDialog::onConfirmClicked(){
    bool isValid=true;
    QStringList valueList;
    QString message="存在不合法字段:\n";
    foreach(AbstractCustomField* field,controllerList){
        if(field==nullptr)
            continue;
        if(!field->isValid())
            message+=(field->getFieldData().fieldName+"\n"),isValid=false;
        valueList<<field->text();
    }
    if(!isValid){
        CustomMessageBox::warning(this,"警告",message);
    }else{
        newKeyItem=new KeyItem(groupName,valueList);
        emit newKeyItemAccept(groupName,newKeyItem);
        this->close();
    }
}
void KeyItemDialog::onCancelClicked(){
    this->close();
}
KeyItemDialog::~KeyItemDialog()
{
    delete ui;
}
void KeyItemDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void KeyItemDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void KeyItemDialog::mouseReleaseEvent(QMouseEvent*)
{
    newKeyItem=nullptr;
    mousePressed = false;
}
void KeyItemDialog::onCloseClicked()
{
    this->close();
}
void KeyItemDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void KeyItemDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    this->setWindowTitle(newWindowTitle);
    ui->dialogTitleLabel->setText(newWindowTitle);
}
bool KeyItemDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void KeyItemDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
