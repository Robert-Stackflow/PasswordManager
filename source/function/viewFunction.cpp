#include "QtAwesome.h"
#include "controller/floatbox.h"
#include "dialogs/groupdialog.h"
#include "dialogs/groupinfodialog.h"
#include "dialogs/userdialog.h"
#include "ui_homewidget.h"
#include "util/customdatabase.h"
#include "widget.h"
#include "ui_widget.h"
#include "util/databasehelper.h"
#include "util/datapathhelper.h"
#include "util/shareddatahelper.h"
#include "QsLog.h"
#include "widgets/customtablewidget.h"
#include "widgets/homewidget.h"
#include <QSvgRenderer>
#include <QMouseEvent>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
void Widget::onListViewClicked(const QModelIndex& index)
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    DataBaseHelper& dataBaseHelper = DataBaseHelper::instace();
    DataPathHelper& dataPathHelper = DataPathHelper::instance();
    QStringList columnNames;
    QList<int> columnWidths;
    CustomTableWidget* customTableWidget=nullptr;
    QString data = index.data().toString();
    QString databasePath=dataPathHelper.getCurrentAccountDataBasePath();
    QString parentData = ui->listView->parentModelIndex(index).data().toString();
    if(parentData=="分组管理"){
        if(data=="分组管理"){
            customTableWidget=new CustomTableWidget(CustomTableWidget::GROUPS,databasePath,"default",dataBaseHelper.getGroupsTableName(),columnNames,columnWidths,ui->rightMainWidget);
            connect(customTableWidget,SIGNAL(addClicked(QString)),this,SLOT(onAddGroupClicked(QString)));
        }else{
            if(sharedDataHelper.groupList.has(data)){
                customTableWidget=new CustomTableWidget(CustomTableWidget::GROUP,databasePath,"default",dataBaseHelper.getGroupTableName(data),columnNames,columnWidths,ui->rightMainWidget);
                customTableWidget->setDescribe(sharedDataHelper.groupList[sharedDataHelper.groupList.index(data)]->getDescribe());
                connect(customTableWidget,SIGNAL(moreClicked(QString)),this,SLOT(onGroupMoreClicked(QString)));
                connect(customTableWidget,SIGNAL(addClicked(QString)),this,SLOT(onAddKeyItemClicked(QString)));
            }
        }
    }else if(parentData=="分组类型管理"){
        if(data=="分组类型管理"){
            customTableWidget=new CustomTableWidget(CustomTableWidget::GROUPTYPES,databasePath,"default",dataBaseHelper.getGroupTypesTableName(),columnNames,columnWidths,ui->rightMainWidget);
            connect(customTableWidget,SIGNAL(addClicked(QString)),this,SLOT(onAddGroupTypeClicked(QString)));
        }else{
            if(sharedDataHelper.groupTypeList.has(data)){
                customTableWidget=new CustomTableWidget(CustomTableWidget::GROUPTYPE,databasePath,"default",dataBaseHelper.getGroupTypeTableName(data),columnNames,columnWidths,ui->rightMainWidget);
                customTableWidget->setDescribe(sharedDataHelper.groupTypeList[sharedDataHelper.groupTypeList.index(data)]->getDescribe());
                connect(customTableWidget,SIGNAL(moreClicked(QString)),this,SLOT(onGroupTypeMoreClicked(QString)));
                connect(customTableWidget,SIGNAL(addClicked(QString)),this,SLOT(onAddFieldClicked(QString)));
            }
        }
    }
    if(customTableWidget!=nullptr){
        customTableWidget->setName(data);
        customTableWidget->refresh();
    }else{
        CustomMessageBox::warning(this,"警告","资源不存在");
    }
    loadWidget(customTableWidget);
}
void Widget::refreshListView()
{
    SharedDataHelper& sharedDataHelper = SharedDataHelper::instance();
    QStringList listViewInfoList;
    listViewInfoList<<sharedDataHelper.groupList.getListViewInfo()<<sharedDataHelper.groupTypeList.getListViewInfo();
    ui->listView->setData(listViewInfoList);
    ui->listView->setStyleSheet(ui->listView->styleSheet()+"QListView:item{height:50px;}");
}
void Widget::onGroupCountChanged()
{
    refreshListView();
}
void Widget::onSettingsChanged()
{
    refreshListView();
}
void Widget::onGroupTypeCountChanged()
{
    refreshListView();
}
void Widget::onGroupTypeContentChanged()
{
    refreshListView();
}
void Widget::onGroupContentChanged()
{
    refreshListView();
}
void Widget::loadAccountPortrait(){
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    Account* activeAccount=sharedDataHelper.accountList.getActiveAccount();
    if(activeAccount==nullptr)
        return;
    ui->userPortrait->setPixmap(activeAccount->getPortrait());
    ui->userPortrait->setScaledContents(true);
    ui->usernameLabel->setToolTip(activeAccount->getUsername());
    ui->userPortrait->setStyleSheet("QLabel{border-radius:1px;border:0px;}");
    ui->usernameLabel->setText(activeAccount->getUsername());
}
void Widget::onLockClicked()
{
    qDebug("lock");
}
void Widget::onLogoutClicked()
{
    int result=CustomMessageBox::question(this,"退出登录","确认退出登录?");
    if(result==CustomMessageBox::YES){
        UserDialog* dialog=(UserDialog*)sender();
        dialog->close();
        FloatBox::Success("退出登录",this);
        QTimer::singleShot(500,this,[=]{
            QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
            anim->setDuration(500);
            anim->setStartValue(1);
            anim->setEndValue(0);
            anim->setEasingCurve(QEasingCurve::OutCurve);
            anim->start();
            connect(anim, &QPropertyAnimation::finished, [=] {
                close();
                QProcess::startDetached(qApp->applicationFilePath(), QStringList());
            });
        });
    }
}
void Widget::loadWidget(QWidget* widget)
{
    QLayoutItem *child;
    while ((child = ui->rightMainWidgetGridLayout->takeAt(0)) != 0){
        if(child->widget())
            child->widget()->setParent(NULL);
        delete child;
    }
    if(widget==nullptr)
        return;
    ui->rightMainWidgetGridLayout->addWidget(widget);
}
void Widget::onSideButtonClicked()
{
    ui->leftWidget->setVisible(!ui->leftWidget->isVisible());
    if(ui->leftWidget->isVisible())
        ui->sideButton->setToolTip("收起左栏"),ui->sideButton->setIcon(QIcon(":custom/icons/turnleft.png"));
    else
        ui->sideButton->setToolTip("展开左栏"),ui->sideButton->setIcon(QIcon(":custom/icons/turnright.png"));
}
void Widget::onMaximizeClicked()
{
    if (isMaximized())
        ui->maximizeButton->setIcon(QIcon(":custom/icons/maximize.png")),showNormal();
    else
        ui->maximizeButton->setIcon(QIcon(":custom/icons/restore.png")),showMaximized();
}
void Widget::onMinimizeClicked()
{
    showMinimized();
}
void Widget::onCloseClicked()
{
    close();
}
bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress) {
        if(obj==ui->userPortrait){
            SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
            Account* activeAccount=sharedDataHelper.accountList.getActiveAccount();
            if(activeAccount==nullptr)
                return false;
            UserDialog* dialog=new UserDialog(activeAccount->getPortrait(),activeAccount->getUsername(),this);
            dialog->show();
            connect(dialog,SIGNAL(logoutAccept()),this,SLOT(onLogoutClicked()));
            connect(dialog,SIGNAL(lockAccept()),this,SLOT(onLockClicked()));
        }
    }
    if(event->type() == QEvent::HoverEnter) {
        if(obj==ui->settingButton||obj==ui->aboutButton)
        {
            QToolButton* button=(QToolButton*)obj;
            QtAwesome* awesome = new QtAwesome(qApp);
            awesome->initFontAwesome();
            QVariantMap options;
            options.insert("color",QColor(255,255,255));
            if(obj==ui->settingButton)
                button->setIcon(awesome->icon(fa::gear,options));
            if(obj==ui->aboutButton)
                button->setIcon(awesome->icon(fa::infocircle,options));
        }
    }
    if(event->type() == QEvent::HoverLeave) {
        if(obj==ui->settingButton||obj==ui->aboutButton)
        {
            QToolButton* button=(QToolButton*)obj;
            QtAwesome* awesome = new QtAwesome(qApp);
            awesome->initFontAwesome();
            QVariantMap options;
            options.insert("color",QColor(150,150,150));
            if(obj==ui->settingButton)
                button->setIcon(awesome->icon(fa::gear,options));
            if(obj==ui->aboutButton)
                button->setIcon(awesome->icon(fa::infocircle,options));
        }
    }
    if(event->type() == QEvent::WindowStateChange){
        if (isMaximized())
            ui->maximizeButton->setIcon(QIcon(":custom/icons/restore.png"));
        else
            ui->maximizeButton->setIcon(QIcon(":custom/icons/maximize.png"));
    }
    return QObject::eventFilter(obj, event);
}
