#include "dialogs/groupinfodialog.h"
#include "ui_groupinfodialog.h"
#include <QEvent>
#include <Windows.h>
#include <WinUser.h>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
GroupInfoDialog::GroupInfoDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupInfoDialog)
{
    ui->setupUi(this);
    initWidget();
}
void GroupInfoDialog::initWidget()
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
    ui->dialogGridLayout->setMargin(20);
    ui->describelabel->setText(ui->describelabel->fontMetrics().elidedText(ui->describelabel->text(),Qt::ElideRight,ui->describelabel->width()));
    connect(this,SIGNAL(ncActive(bool)),this,SLOT(onNcActived(bool)));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
    connect(ui->editButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->copyButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
}
void GroupInfoDialog::onButtonsClicked()
{
    this->close();
    QPushButton* button=(QPushButton*)sender();
    if(button==ui->copyButton)
        emit copyClicked();
    else if(button==ui->editButton)
        emit editClicked();
    else if(button==ui->deleteButton)
        emit deleteClicked();
}
GroupInfoDialog::~GroupInfoDialog()
{
    delete ui;
}
void GroupInfoDialog::loadGroup(Group* group)
{
    if(group==nullptr)
        return;
    setWindowCustomTitle(group->getGroupName());
    ui->describelabel->setText(group->getDescribe());
    ui->groupNameLabel->setText(group->getGroupName());
    ui->describelabel->setText(ui->describelabel->fontMetrics().elidedText(ui->describelabel->text(),Qt::ElideRight,ui->describelabel->width()));
    ui->groupTypeLabel->setText("分组类型:"+group->getGroupTypeName());
    ui->createTimeLabel->setText("创建时间:"+group->getCreateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->lastEditTimeLabel->setText("修改时间:"+group->getLastEditTime().toString("yyyy-MM-dd hh:mm:ss"));
}
void GroupInfoDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons())) {
        this->move(e->globalPos()-mousePoint);
        e->accept();
    }
}
void GroupInfoDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos()-this->pos();
        e->accept();
    }
}
void GroupInfoDialog::mouseReleaseEvent(QMouseEvent*)
{
    mousePressed = false;
}
void GroupInfoDialog::onCloseClicked()
{
    this->close();
}
void GroupInfoDialog::onMinimizeClicked()
{
    this->showMinimized();
}
void GroupInfoDialog::setWindowCustomTitle(const QString& newWindowTitle)
{
    this->setWindowTitle(newWindowTitle);
    ui->dialogTitleLabel->setText(newWindowTitle);
}
bool GroupInfoDialog::nativeEvent(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast< MSG * >(message);
    if(msg->message == WM_NCACTIVATE)
    {
        bool active = (bool)(msg->wParam);
        emit ncActive(active);
    }
    return false;
}
void GroupInfoDialog::onNcActived(bool active)
{
    if(active)
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:white;}");
    else
        ui->titleBarWidget->setStyleSheet("QWidget#titleBarWidget{background:#f5f5f5;}");
}
