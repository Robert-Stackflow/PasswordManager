#include "widgets/customtablewidget.h"
#include "controller/floatbox.h"
#include "delegates/customstyleddelegate.h"
#include "ui_customtablewidget.h"
#include "QtAwesome.h"
#include "delegates/Delegates.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
CustomTableWidget::CustomTableWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CustomTableWidget),
    resultCount(20)
{
    ui->setupUi(this);
    initWidget();
}
CustomTableWidget::CustomTableWidget(MODE currentMode,QString dbFilePath,QString connName,QString tableName,
                                     QStringList columnNames,QList<int> columnWidths,QWidget *parent):
    QWidget(parent),
    ui(new Ui::CustomTableWidget),
    currentMode(currentMode)
{
    ui->setupUi(this);
    setDbFilePath(dbFilePath);
    setConnName(connName);
    setTableName(tableName);
    setColumnNames(columnNames);
    setColumnWidths(columnWidths);
    initWidget();
}
CustomTableWidget::~CustomTableWidget()
{
    delete ui;
}
void CustomTableWidget::initWidget()
{
    QtAwesome* awesome = new QtAwesome(qApp);
    awesome->initFontAwesome();
    ui->btnRollback->setIcon(awesome->icon(0xf0e2));
    ui->btnCommit->setIcon(awesome->icon(0xf058));
    ui->btnRefresh->setIcon(awesome->icon(0xf021));
    ui->btnExport->setIcon(awesome->icon(0xf56e));
    ui->btnImport->setIcon(awesome->icon(0xf56f));
    ui->btnAdd->setIcon(awesome->icon(0xf055));
    ui->btnMore->setIcon(awesome->icon(0xf141));
    ui->btnFirst->setIcon(awesome->icon(0xf100));
    ui->btnPre->setIcon(awesome->icon(0xf104));
    ui->btnNext->setIcon(awesome->icon(0xf105));
    ui->btnLast->setIcon(awesome->icon(0xf101));
    ui->btnMore->setIcon(awesome->icon(fa::ellipsish));
    ui->countPerPage->addItem("10条每页",10);
    ui->countPerPage->addItem("20条每页",20);
    ui->countPerPage->addItem("40条每页",40);
    ui->countPerPage->addItem("50条每页",50);
    ui->countPerPage->addItem("80条每页",80);
    ui->countPerPage->addItem("100条每页",100);
    ui->countPerPage->addItem("150条每页",150);
    ui->countPerPage->addItem("200条每页",200);
    ui->countPerPage->setView(new QListView());
    ui->countPerPage->setCurrentIndex(1);
    connect(ui->countPerPage,SIGNAL(currentIndexChanged(int)),this,SLOT(onCountPerPageChanged(int)));
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
    columnNames.clear();
    columnWidths.clear();
    countName = "rowid";
    QSqlQueryModel* model=new QSqlQueryModel();
    ui->tableMain->setModel(model);
    dbPage = new DbPage(this);
    dbPage->setAllCenter(true);
    dbPage->setControl(ui->tableMain,nullptr,nullptr,nullptr,nullptr,nullptr,ui->pageDescribe,
                       ui->btnFirst, ui->btnPre, ui->btnNext, ui->btnLast,ui->boxSetPage, countName);
    ui->tableMain->verticalHeader()->setDefaultSectionSize(35);
    ui->tableMain->horizontalHeader()->setStretchLastSection(true);
    QFont font("kaiti",11);
    ui->tableMain->horizontalHeader()->setFont(font);
    ui->tableMain->verticalHeader()->setFont(font);
    ui->tableMain->verticalHeader()->hide();
    ui->tableMain->setShowGrid(false);
    ui->tableMain->setAlternatingRowColors(true);
    ui->tableMain->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableMain->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableMain->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableMain->setItemDelegate(new CustomStyledDelegate());
    QStringList columns;
    switch(currentMode){
    case GROUPS:
        //        ui->tableMain->setItemDelegateForColumn(4, new ComboBoxDelegate(currentMode,ui->nameLabel->text(),ui->tableMain->model()->headerData(4,Qt::Horizontal).toString(),this));
        //        ui->tableMain->setItemDelegateForColumn(5, new ComboBoxDelegate(currentMode,ui->nameLabel->text(),ui->tableMain->model()->headerData(5,Qt::Horizontal).toString(),this));
        //        ui->tableMain->setItemDelegateForColumn(6, new ComboBoxDelegate(currentMode,ui->nameLabel->text(),ui->tableMain->model()->headerData(6,Qt::Horizontal).toString(),this));
        ui->btnMore->setVisible(false);
        setToolTips("刷新","导出分组到文件","从文件导入分组","新建分组","");
        break;
    case GROUP:
        ui->btnImport->setVisible(false);
        ui->tableMain->setItemDelegateForColumn(4, new ComboBoxDelegate(currentMode,ui->nameLabel->text(),ui->tableMain->model()->headerData(4,Qt::Horizontal).toString(),this));
        ui->tableMain->setItemDelegateForColumn(5, new ComboBoxDelegate(currentMode,ui->nameLabel->text(),ui->tableMain->model()->headerData(5,Qt::Horizontal).toString(),this));
        ui->tableMain->setItemDelegateForColumn(6, new ComboBoxDelegate(currentMode,ui->nameLabel->text(),ui->tableMain->model()->headerData(6,Qt::Horizontal).toString(),this));
        setToolTips("刷新","导出当前分组到文件","","新建条目","查看分组信息");
        break;
    case GROUPTYPES:
        ui->btnMore->setVisible(false);
        setToolTips("刷新","导出分组类型到.xml文件","从xml文件导入分组类型","新建分组类型","");
        break;
    case GROUPTYPE:
        ui->btnImport->setVisible(false);
        setToolTips("刷新","导出当前分组类型到文件","","添加字段","查看分组类型信息");
        break;
    }
    connect(ui->btnAdd,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->btnMore,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->btnExport,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->btnRefresh,SIGNAL(clicked()),this,SLOT(onButtonsClicked()));
    connect(ui->tableMain->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(onHeaderContextMenu(QPoint)));
    connect(ui->tableMain, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(onContextMenu(QPoint)));
}
void CustomTableWidget::setConnName(const QString& connName)
{
    this->connName=connName;
}
void CustomTableWidget::setDbFilePath(const QString& dbFilePath)
{
    this->dbFilePath=dbFilePath;
}
void CustomTableWidget::setTableName(const QString& tableName)
{
    this->tableName=tableName;
}
void CustomTableWidget::setColumnNames(QStringList columnNames)
{
    this->columnNames=columnNames;
}
void CustomTableWidget::setColumnWidths(QList<int> columnWidths)
{
    this->columnWidths=columnWidths;
}
void CustomTableWidget::setResultCount(int resultCount)
{
    this->resultCount=resultCount;
}
void CustomTableWidget::setCountName(const QString& countName)
{
    this->countName=countName;
}
void CustomTableWidget::setName(const QString& name)
{
    ui->nameLabel->setText(name);
}
void CustomTableWidget::setDescribe(const QString& describe)
{
    ui->describeLabel->setText(describe);
    ui->describeLabel->setText(ui->describeLabel->fontMetrics().elidedText(ui->describeLabel->text(),Qt::ElideRight,ui->describeLabel->width()));
}
void CustomTableWidget::refresh()
{
    if(dbPage==nullptr)
        return;
    QString sql = "where 1=1";
    dbPage->setDbFile(dbFilePath);
    dbPage->setConnName(connName);
    dbPage->setTableName(tableName);
    dbPage->setColumnNames(columnNames);
    dbPage->setResultCurrent(resultCount);
    dbPage->setColumnWidths(columnWidths);
    dbPage->setOrderSql(QString("%1 %2").arg(countName).arg("asc"));
    dbPage->setWhereSql(sql);
    dbPage->select();
}
void CustomTableWidget::onButtonsClicked()
{
    QAbstractButton* button = (QPushButton*)sender();
    if(button == ui->btnRefresh)
    {
        refresh();
        FloatBox::Success("刷新成功",this);
    }
    if(button == ui->btnMore)
        emit moreClicked(ui->nameLabel->text());
    if(button == ui->btnAdd)
        emit addClicked(ui->nameLabel->text());
    if(button == ui->btnExport)
        emit exportClicked(ui->nameLabel->text());
}
void CustomTableWidget::onCountPerPageChanged(int index)
{
    setResultCount(ui->countPerPage->currentData().toInt());
    refresh();
}
void CustomTableWidget::onContextMenu(QPoint pos)
{
    auto index = ui->tableMain->indexAt(pos);
    if (index.isValid())
    {
        QMenu *popMenu;
        popMenu = new QMenu(ui->tableMain);
        QList<QAction*> actions;
        if(currentMode==GROUPS){
            QAction *openGroup = new QAction("打开分组");
            QAction *editGroup = new QAction("编辑分组");
            QAction *copyGroup = new QAction("复制分组");
            QAction *deleteGroup = new QAction("删除分组");
            QAction *exportGroup = new QAction("导出分组");
            actions<<openGroup<<editGroup<<copyGroup<<deleteGroup<<exportGroup;
            popMenu->addActions(actions);
        }else if(currentMode==GROUP){
            QAction *editKeyItem = new QAction("编辑条目");
            QAction *copyKeyItem = new QAction("复制条目");
            QAction *deleteKeyItem = new QAction("删除条目");
            QAction *exportKeyItem = new QAction("复制条目信息");
            QMenu *copyKeyItemToGroup = new QMenu("复制到其他分组");
            QMenu *moveKeyItemToGroup = new QMenu("移动到其他分组");
            actions<<editKeyItem<<copyKeyItem<<deleteKeyItem<<exportKeyItem;
            popMenu->addActions(actions);
            popMenu->addSeparator();
            popMenu->addMenu(copyKeyItemToGroup);
            popMenu->addMenu(moveKeyItemToGroup);
        }else if(currentMode==GROUPTYPES){
            QAction *openGroupType = new QAction("查看分组类型");
            QAction *editGroupType = new QAction("编辑分组类型");
            QAction *copyGroupType = new QAction("复制分组类型");
            QAction *deleteGroupType = new QAction("删除分组类型");
            QAction *exportGroupType = new QAction("导出分组类型");
            actions<<openGroupType<<editGroupType<<copyGroupType<<deleteGroupType<<exportGroupType;
            popMenu->addActions(actions);
        }else if(currentMode==GROUPTYPE){
            QAction *editField = new QAction("编辑字段");
            QAction *copyField = new QAction("复制字段");
            QAction *deleteField = new QAction("删除字段");
            actions<<editField<<copyField<<deleteField;
            popMenu->addActions(actions);
        }
        popMenu->exec(QCursor::pos());
    }
}
void CustomTableWidget::onHeaderContextMenu(QPoint pos)
{
    QMenu *popMenu;
    popMenu = new QMenu(ui->tableMain->horizontalHeader());
    QAction* hideColumn=new QAction("隐藏列");
    QAction* hideTimeColumn=new QAction("隐藏时间列");
    QMenu* hiddenColumns=new QMenu("已隐藏的列");
    QAction* showAllColumn=new QAction("显示所有列");
    QAction* showIdColumn=new QAction("显示行号列");
    popMenu->addAction(hideColumn);
    popMenu->addAction(hideTimeColumn);
    popMenu->addMenu(hiddenColumns);
    popMenu->addSeparator();
    popMenu->addAction(showAllColumn);
    popMenu->addAction(showIdColumn);
    popMenu->exec(QCursor::pos());
}
void CustomTableWidget::setVerticalHeaderVisible(bool verticalHeaderVisible)
{
    ui->tableMain->verticalHeader()->setVisible(verticalHeaderVisible);
}
void CustomTableWidget::setHorizontalHeaderVisible(bool horizontalHeaderVisible)
{
    ui->tableMain->horizontalHeader()->setVisible(horizontalHeaderVisible);
}
void CustomTableWidget::setToolTips(const QString& refreshTip,const QString& exportTip,const QString& importTip,const QString& addTip,const QString& moreTip)
{
    ui->btnRefresh->setToolTip(refreshTip);
    ui->btnExport->setToolTip(exportTip);
    ui->btnImport->setToolTip(importTip);
    ui->btnAdd->setToolTip(addTip);
    ui->btnMore->setToolTip(moreTip);
}
