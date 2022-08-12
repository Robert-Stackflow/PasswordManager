#include "widgets/lockerwidget.h"
#include "QsLog.h"
#include "controller/custommessagebox.h"
#include "qpropertyanimation.h"
#include "qsqlquery.h"
#include "qtimer.h"
#include "ui_lockerwidget.h"
#include "util/databasehelper.h"
#include "util/customdatabase.h"
#include "util/datapathhelper.h"
#include "util/shareddatahelper.h"
#include "controller/floatbox.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
LockerWidget::LockerWidget(QWidget *parent) :
    FramelessWidget(parent),
    ui(new Ui::LockerWidget)
{
    ui->setupUi(this);
    initWidget();
}
void LockerWidget::initWidget()
{
    //设置无边框
    setBtnClose(ui->closeButton);
    setTitleBar(ui->topWidget);
    addIgnoreWidget(ui->programNameLabel);
    ui->password->setValidator(FieldData::passwordValidator);
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(onCloseClicked()));
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(onConfirmClicked()));
    QObject::connect(ui->portrait,&QToolButton::clicked,[=]{
        CustomMessageBox::information(this,"提示","用户已锁定，输入安全锁密码以解锁");
    });
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    Account* activeAccount=sharedDataHelper.accountList.getActiveAccount();
    if(activeAccount==nullptr)
        return;
    ui->usernameLabel->setText(activeAccount->getUsername());
}
void LockerWidget::onConfirmClicked()
{
    SharedDataHelper &sharedDataHelper = SharedDataHelper::instance();
    DataPathHelper &dataPathHelper = DataPathHelper::instance();
    CustomDataBase& customDataBase = CustomDataBase::instance();
    Account* currentAccount = sharedDataHelper.accountList.getActiveAccount();
    QString currentUsername = currentAccount->getUsername();
    QString databaseName = dataPathHelper.getAccountsDataBasePath();
    //判断安全锁密码是否正确
    customDataBase.setDatabaseName(databaseName);
    QSqlQuery query(customDataBase.getDatabase());
    if (customDataBase.open())
    {
        QString selectSql = "select password from accounts where username='" + currentUsername + "'";
        query.exec(selectSql);
        if (query.first()){
            if (query.value(0).toString() == ui->password->text()){
                FloatBox::Success("解锁成功",this);
                QTimer::singleShot(500,this,[=]{
                    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
                    anim->setDuration(500);
                    anim->setStartValue(1);
                    anim->setEndValue(0);
                    anim->setEasingCurve(QEasingCurve::OutCurve);
                    anim->start();
                    emit closeLockerAccept();
                    connect(anim, &QPropertyAnimation::finished, [=] {
                        this->close();
                    });
                });
            }else
                CustomMessageBox::warning(this, "警告", "安全锁密码不正确");
        }
        customDataBase.close();
    }
}
void LockerWidget::onCloseClicked()
{
    close();
}
LockerWidget::~LockerWidget()
{
    delete ui;
}
