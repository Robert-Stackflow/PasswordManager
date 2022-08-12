#include "dialogs/groupdialog.h"
#include "controller/floatbox.h"
#include "qdebug.h"
#include "qtimer.h"
#include "ui_groupdialog.h"
#include "util/shareddatahelper.h"
#include "controller/custommessagebox.h"
#include "customField/fieldfactory.h"
#include <QEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
GroupDialog::GroupDialog(mode currentMode,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupDialog)
{
    ui->setupUi(this);
    initWidget();
    setCurrentMode(currentMode);
}
void GroupDialog::initWidget()
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
    groupName=(customLineEdit*)FieldFactory::FieldController("分组名称",FieldData::LINEEDIT,FieldData::REQUIRED,FieldData::NORMAL,this,"输入分组名称");
    ui->infoLayout->addWidget(groupName);
    groupType=(customComboBox*)FieldFactory::FieldController("分组类型",FieldData::COMBOBOX,FieldData::REQUIRED,FieldData::NORMAL,this,"选择分组类型");
    ui->infoLayout->addWidget(groupType);
    groupType->addItems(sharedDataHelper.groupTypeList.getGroupTypeNames());
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
    groupName->setFocus();
}
void GroupDialog::onConfirmClicked(){
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    bool isUnique=!((currentMode==NEWGROUP&&sharedDataHelper.groupList.has(groupName->text()))||(currentMode==EDITGROUP&&groupName->text()!=oldGroupName&&sharedDataHelper.groupList.has(groupName->text())));
    if(groupName->isValid()&&groupType->isValid()&&isUnique)
    {
        if(currentMode==NEWGROUP){
            newGroup=new Group(groupType->currentText(),groupName->text(),QDateTime::currentDateTime(),QDateTime::currentDateTime(),ui->describe->toPlainText());
            emit newGroupAccept(newGroup);
        }else if(currentMode==EDITGROUP){
            newGroup=new Group(groupType->currentText(),groupName->text(),QDateTime::currentDateTime(),QDateTime::currentDateTime(),ui->describe->toPlainText());
            emit editGroupAccept(oldGroupName,newGroup);
        }else{
            newGroup=sharedDataHelper.groupList[sharedDataHelper.groupList.index(oldGroupName)]->clone();
            newGroup->setGroupName(groupName->text());
            newGroup->setDescribe(ui->describe->toPlainText());
            emit newGroupAccept(newGroup);
        }
        this->close();
    }else{
        QString message="";
        if(!groupName->isValid())
            message+="分组名称不能为空!\n";
        if(!groupType->isValid())
            message+="分组类型不合法!\n";
        if(!isUnique)
            message+="分组\""+groupName->text()+"\"已存在!\n";
        CustomMessageBox::warning(this,"警告",message);
    }
}
void GroupDialog::onCancelClicked(){
    int choice=-1;
    if(currentMode==NEWGROUP)
        choice=CustomMessageBox::question(this,"取消","是否放弃新建分组");
    else if(currentMode==EDITGROUP)
        choice=CustomMessageBox::question(this,"取消","是否放弃修改分组");
    if(choice==CustomMessageBox::YES){
        newGroup=nullptr;
        this->close();
    }
}
void GroupDialog::setCurrentMode(mode newCurrentMode){
    this->currentMode=newCurrentMode;
    if(currentMode==EDITGROUP)
        groupType->setEnable(false),setWindowCustomTitle("修改分组");
    else
        groupType->setEnable(true),setWindowCustomTitle("新建分组");
}
void GroupDialog::loadGroup(Group* group)
{
    if(group==nullptr)
        return;
    if(currentMode==EDITGROUP){
        setWindowCustomTitle("修改分组\""+group->getGroupName()+"\"");
        oldGroupName=group->getGroupName();
        groupName->setText(group->getGroupName());
        groupType->setCurrentText(group->getGroupTypeName());
        ui->describe->setText(group->getDescribe());
    }else{
        setWindowCustomTitle("复制分组\""+group->getGroupName()+"\"至");
        oldGroupName=group->getGroupName();
        groupName->setText(group->getGroupName()+"_副本");
        groupType->setCurrentText(group->getGroupTypeName());
        ui->describe->setText(group->getDescribe()+"(复制自分组\""+group->getGroupName()+"\")");
    }
}
GroupDialog::~GroupDialog()
{
    delete ui;
}
void GroupDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void GroupDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void GroupDialog::mouseReleaseEvent(QMouseEvent*)
{
    newGroup=nullptr;
    mousePressed = false;
}
void GroupDialog::onCloseClicked()
{
    this->close();
}
void GroupDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void GroupDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    this->setWindowTitle(newWindowTitle);
    ui->dialogTitleLabel->setText(newWindowTitle);
}
bool GroupDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void GroupDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
