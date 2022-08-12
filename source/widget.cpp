#include "widget.h"
#include "controller/custommessagebox.h"
#include "controller/floatbox.h"
#include "dialogs/exportdialog.h"
#include "dialogs/settingdialog.h"
#include "qdebug.h"
#include "widgets/homewidget.h"
#include "dialogs/aboutdialog.h"
#include "qtimer.h"
#include "ui_widget.h"
#include "widgets/homewidget.h"
#include <QCoreApplication>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QScrollBar>
#include <QFontDatabase>
#include "QtAwesome.h"
#include "widgets/lockerwidget.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
Widget::Widget(QWidget *parent)
    : FramelessWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}
void Widget::openLocker()
{
    if(isLogin){
        QTimer::singleShot(500,this,[=]{
            QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
            anim->setDuration(500);
            anim->setStartValue(1);
            anim->setEndValue(0);
            anim->setEasingCurve(QEasingCurve::OutCurve);
            anim->start();
            connect(anim, &QPropertyAnimation::finished, [=] {
                this->close();
                LockerWidget* lockerWidget = new LockerWidget(this);
                lockerWidget->show();
                lockerWidget->move((QApplication::desktop()->width() - lockerWidget->width())/2,(QApplication::desktop()->height() - lockerWidget->height())/2);
                connect(lockerWidget,SIGNAL(closeLockerAccept()),this,SLOT(closeLockerReceived()));
                isLogin=false;
            });
        });
    }
}
void Widget::closeLockerReceived()
{
    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::OutCurve);
    anim->start();
    this->show();
    isLogin=true;
}

bool Widget::getIsLogin() const
{
    return isLogin;
}
void Widget::loginReceive(){
    initWidget();
    this->show();
    isLogin=true;
    setGeometry(0,0,1600,950);
    move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);
    move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);
}
void Widget::initWidget()
{
#ifdef Q_OS_WIN
    setResizeableAreaWidth(8);
    setTitleBar(ui->titleBarWidget);
    setBtnMin(ui->minimizeButton);
    setBtnMax(ui->maximizeButton);
    setBtnClose(ui->closeButton);
#endif
    //渐变动画显示窗体
    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::OutCurve);
    anim->start();
    //初始化
    this->setWindowTitle("Password Manager");
    installEventFilter(this);
    ui->userPortrait->installEventFilter(this);
    ui->aboutButton->installEventFilter(this);
    ui->settingButton->installEventFilter(this);
    //设置头像
    loadAccountPortrait();
    //初始化NavListWidget
    ui->listView->readData("config.xml");
    QStringList a;
    ui->listView->setData(a);
    ui->listView->setIcoColorBg(false);
    ui->listView->setColorLine(QColor(32, 53, 74));
    ui->listView->setColorBg(QColor(46,46,46), QColor(7, 193, 96), QColor(56, 205, 127, 150));
    ui->listView->setColorText(QColor(255, 255, 255), QColor(252, 252, 252), QColor(0, 0, 0));
    ui->listView->setStyleSheet(ui->listView->styleSheet()+"QListView:item{50px;}");
    QFile file(":qss/custom.qss");
    file.open(QFile::ReadOnly);
    ui->listView->verticalScrollBar()->setStyleSheet(file.readAll());
    //绑定槽函数
    connect(ui->aboutButton,SIGNAL(clicked()),this,SLOT(onAboutClicked()));
    connect(ui->sideButton,SIGNAL(clicked()),this,SLOT(onSideButtonClicked()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->maximizeButton,SIGNAL(clicked()),this,SLOT(onMaximizeClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked()),this,SLOT(onMinimizeClicked()));
    connect(ui->stickTopButton,SIGNAL(clicked()),this,SLOT(onStickTopClicked()));
    connect(ui->settingButton,SIGNAL(clicked()),this,SLOT(onSettingClicked()));
    connect(this,SIGNAL(groupCountChanged()),this,SLOT(onGroupCountChanged()));
    connect(this,SIGNAL(groupTypeCountChanged()),this,SLOT(onGroupTypeCountChanged()));
    connect(this,SIGNAL(groupContentChanged()),this,SLOT(onGroupContentChanged()));
    connect(this,SIGNAL(groupTypeContentChanged()),this,SLOT(onGroupTypeContentChanged()));
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),this,SLOT(onListViewClicked(QModelIndex)));
    connect(ui->listView,SIGNAL(customCurrentIndexChanged(QModelIndex)),this,SLOT(onListViewClicked(QModelIndex)));
    //加载用户数据
    loadUserData();
    //测试初始化
    ui->listView->setCurrentIndexByData("默认分组");
    ui->leftWidget->hide();
    //测试Fontawesome
    QtAwesome* awesome = new QtAwesome( qApp );
    awesome->initFontAwesome();
    QVariantMap options;
    options.insert("color",QColor(150, 150, 150));
    ui->settingButton->setIcon(awesome->icon(fa::gear,options));
    ui->aboutButton->setIcon(awesome->icon(fa::infocircle,options));
}
Widget::~Widget()
{
    delete ui;
}
void Widget::onStickTopClicked()
{
    ExportDialog* dialog=new ExportDialog(this);
    dialog->show();
}
void Widget::onSettingClicked()
{
    SettingDialog* dialog=new SettingDialog(this);
    dialog->show();
    connect(dialog,SIGNAL(settingsChanged()),this,SLOT(onSettingsChanged()));
}
void Widget::onAboutClicked()
{
    AboutDialog* dialog=new AboutDialog(this);
    dialog->show();
    connect(dialog,SIGNAL(settingsChanged()),this,SLOT(onSettingsChanged()));
}
