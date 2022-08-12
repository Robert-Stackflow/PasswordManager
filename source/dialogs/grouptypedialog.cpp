#include "dialogs/grouptypedialog.h"
#include "ui_grouptypedialog.h"
#include "controller/floatbox.h"
#include "customField/fieldfactory.h"
#include "qdebug.h"
#include "qtimer.h"
#include "util/shareddatahelper.h"
#include "controller/custommessagebox.h"
#include <QEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
GroupTypeDialog::GroupTypeDialog(mode currentMode,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupTypeDialog)
{
    ui->setupUi(this);
    initWidget();
    setCurrentMode(currentMode);
}
void GroupTypeDialog::initWidget()
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
    groupTypeName=(customLineEdit*)FieldFactory::FieldController("分组类型名称",FieldData::LINEEDIT,FieldData::REQUIRED,FieldData::NORMAL,this,"输入分组类型名称");
    ui->infoLayout->addWidget(groupTypeName);
    //绑定槽函数
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(onConfirmClicked()));
    connect(ui->cancel,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
    connect(this,SIGNAL(ncActive(bool)),this,SLOT(onNcActived(bool)));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
    groupTypeName->setFocus();
}
void GroupTypeDialog::onConfirmClicked(){
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    bool isUnique=!((currentMode==NEWGROUPTYPE&&sharedDataHelper.groupTypeList.has(groupTypeName->text()))||(currentMode==EDITGROUPTYPE&&groupTypeName->text()!=oldGroupTypeName&&sharedDataHelper.groupTypeList.has(groupTypeName->text())));
    if(groupTypeName->isValid()&&isUnique)
    {
        if(currentMode==NEWGROUPTYPE){
            newGroupType=new GroupType(groupTypeName->text(),QDateTime::currentDateTime(),QDateTime::currentDateTime(),ui->describe->toPlainText());
            emit newGroupTypeAccept(newGroupType);
        }else if(currentMode==EDITGROUPTYPE){
            newGroupType=new GroupType(groupTypeName->text(),QDateTime::currentDateTime(),QDateTime::currentDateTime(),ui->describe->toPlainText());
            emit editGroupTypeAccept(oldGroupTypeName,newGroupType);
        }else{
            newGroupType=sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(oldGroupTypeName)]->clone();
            newGroupType->setGroupTypeName(groupTypeName->text());
            newGroupType->setDescribe(ui->describe->toPlainText());
            emit newGroupTypeAccept(newGroupType);
        }
        this->close();
    }else{
        QString message="";
        if(!groupTypeName->isValid())
            message+="分组类型名称不能为空!\n";
        if(!isUnique)
            message+="分组类型\""+groupTypeName->text()+"\"已存在!\n";
        CustomMessageBox::warning(this,"警告",message);
    }
}
void GroupTypeDialog::onCancelClicked(){
    int choice=-1;
    if(currentMode==NEWGROUPTYPE)
        choice=CustomMessageBox::question(this,"取消","是否放弃新建分组");
    else if(currentMode==EDITGROUPTYPE)
        choice=CustomMessageBox::question(this,"取消","是否放弃修改分组");
    if(choice==CustomMessageBox::YES){
        newGroupType=nullptr;
        this->close();
    }
}
void GroupTypeDialog::setCurrentMode(mode newCurrentMode){
    this->currentMode=newCurrentMode;
    if(currentMode==EDITGROUPTYPE)
        setWindowCustomTitle("修改分组类型");
    else
        setWindowCustomTitle("新建分组类型");
}
void GroupTypeDialog::loadGroupType(GroupType* groupType)
{
    if(groupType==nullptr)
        return;
    if(currentMode==EDITGROUPTYPE){
        setWindowCustomTitle("修改分组类型\""+groupType->getGroupTypeName()+"\"");
        oldGroupTypeName=groupType->getGroupTypeName();
        groupTypeName->setText(groupType->getGroupTypeName());
        ui->describe->setText(groupType->getDescribe());
    }else{
        setWindowCustomTitle("复制分组类型\""+groupType->getGroupTypeName()+"\"至");
        oldGroupTypeName=groupType->getGroupTypeName();
        groupTypeName->setText(groupType->getGroupTypeName()+"_副本");
        ui->describe->setText(groupType->getDescribe()+"(复制自分组类型\""+groupType->getGroupTypeName()+"\")");
    }
}
GroupTypeDialog::~GroupTypeDialog()
{
    delete ui;
}
void GroupTypeDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void GroupTypeDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void GroupTypeDialog::mouseReleaseEvent(QMouseEvent*)
{
    newGroupType=nullptr;
    mousePressed = false;
}
void GroupTypeDialog::onCloseClicked()
{
    this->close();
}
void GroupTypeDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void GroupTypeDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    this->setWindowTitle(newWindowTitle);
    ui->dialogTitleLabel->setText(newWindowTitle);
}
bool GroupTypeDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void GroupTypeDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
//GroupTypeDialog::GroupTypeDialog(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::GroupTypeDialog)
//{
//    ui->setupUi(this);
//}

//GroupTypeDialog::~GroupTypeDialog()
//{
//    delete ui;
//}
