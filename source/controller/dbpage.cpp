#include "controller/dbpage.h"
#include "delegates/Delegates.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
SqlQueryModel::SqlQueryModel(QObject *parent) : QSqlQueryModel(parent)
{
    allCenter = false;
    alignCenterColumn.clear();
    alignRightColumn.clear();
}
QVariant SqlQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if (allCenter) {
        if(role == Qt::TextAlignmentRole ) {
            value = Qt::AlignCenter;
        }
    } else {
        int column = index.column();
        bool existCenter = alignCenterColumn.contains(column);
        bool existRight = alignRightColumn.contains(column);
        if(role == Qt::TextAlignmentRole) {
            if (existCenter) {
                value = Qt::AlignCenter;
            }
            if (existRight) {
                value = (QVariant)(Qt::AlignVCenter | Qt::AlignRight);
            }
        }
    }
    return value;
}

void SqlQueryModel::setAllCenter(bool allCenter)
{
    this->allCenter = allCenter;
}

void SqlQueryModel::setAlignCenterColumn(const QList<int> &alignCenterColumn)
{
    this->alignCenterColumn = alignCenterColumn;
}

void SqlQueryModel::setAlignRightColumn(const QList<int> &alignRightColumn)
{
    this->alignRightColumn = alignRightColumn;
}

DbCountThread::DbCountThread(QObject *parent) : QThread(parent)
{
    connName = "qt_sql_default_connection";
    sql = "select 1";
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}
void DbCountThread::run()
{
    {
        QDateTime dtStart = QDateTime::currentDateTime();
        QSqlDatabase dbConn = QSqlDatabase::addDatabase("QSQLITE",connName);
        dbConn.setDatabaseName(dbFile);
        QSqlQuery query(dbConn);
        dbConn.open();
        query.exec(sql);
        query.next();
        int count = query.value(0).toUInt();
        QDateTime dtEnd = QDateTime::currentDateTime();
        double msec = dtStart.msecsTo(dtEnd);
        emit receiveCount(count, msec);
        dbConn.close();
    }
    QSqlDatabase::removeDatabase(connName);
}
void DbCountThread::setDbFile(const QString &dbFile)
{
    this->dbFile=dbFile;
}
void DbCountThread::setConnName(const QString &connName)
{
    this->connName = connName;
}
void DbCountThread::setSql(const QString &sql)
{
    this->sql = sql;
}
QScopedPointer<DbPage> DbPage::self;
DbPage *DbPage::Instance()
{
    if (self.isNull()) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new DbPage);
        }
    }
    return self.data();
}

DbPage::DbPage(QObject *parent) : QObject(parent)
{
    startIndex = 0;
    tempSql = "";
    sql = "";
    queryModel = new SqlQueryModel;

    pageCurrent = 1;
    pageCount = 0;
    resultCount = 0;
    resultCurrent = 0;

    labPageCount = 0;
    labPageCurrent = 0;
    labResultCount = 0;
    labResultCurrent = 0;
    labResult = 0;
    labInfo = 0;

    tableView = 0;
    btnFirst = 0;
    btnPre = 0;
    btnNext = 0;
    btnLast = 0;

    countName = "*";
    connName = "qt_sql_default_connection";
    dbType = DbType_Sqlite;

    pageCurrent = 0;
    pageCount = 0;
    resultCount = 0;
    resultCurrent = 30;

    tableName = "";
    selectColumn = "*";
    orderSql = "";
    whereSql = "";
    columnNames.clear();
    columnWidths.clear();

    insertColumnIndex = -1;
    insertColumnName = "";
    insertColumnWidth = 50;
}

void DbPage::bindData(const QString &columnName, const QString &orderColumn, const QString &tableName,
                      QComboBox *cbox, const QString &connName)
{
    QSqlQuery query(QSqlDatabase::database(connName));
    query.exec("select " + columnName + " from " + tableName + " order by " + orderColumn + " asc");
    while (query.next()) {
        cbox->addItem(query.value(0).toString());
    }
}

void DbPage::bindData(const QString &columnName, const QString &orderColumn, const QString &tableName,
                      QList<QComboBox *> cboxs, const QString &connName)
{
    QSqlQuery query(QSqlDatabase::database(connName));
    query.exec("select " + columnName + " from " + tableName + " order by " + orderColumn + " asc");
    while (query.next()) {
        foreach (QComboBox *cbox, cboxs) {
            cbox->addItem(query.value(0).toString());
        }
    }
}

void DbPage::bindData(const QString &sql)
{
    {
        QSqlDatabase dbConn = QSqlDatabase::addDatabase("QSQLITE",connName);
        dbConn.setDatabaseName(dbFile);
        QSqlQuery query(dbConn);
        dbConn.open();
        queryModel->setQuery(sql, dbConn);
        tableView->setModel(queryModel);
        dbConn.close();
    }
    QSqlDatabase::removeDatabase(connName);
    int columnCount = tableView->model()->columnCount();
    int nameCount = columnNames.count();
    columnCount = columnCount > nameCount ? nameCount : columnCount;
    QList<QString> columnNames = this->columnNames;
    QList<int> columnWidths = this->columnWidths;
    if (insertColumnIndex >= 0) {
        columnCount++;
        columnNames.insert(insertColumnIndex, insertColumnName);
        columnWidths.insert(insertColumnIndex, insertColumnWidth);
        queryModel->insertColumn(insertColumnIndex);
    }
    for (int i = 0; i < columnCount; i++) {
        queryModel->setHeaderData(i, Qt::Horizontal, columnNames.at(i));
        tableView->setColumnWidth(i, columnWidths.at(i));
    }

    if (labPageCurrent != 0) {
        labPageCurrent->setText(QString("第 %1 页").arg(pageCurrent));
    }

    if (labPageCount != 0) {
        labPageCount->setText(QString("共 %1 页").arg(pageCount));
    }

    if (labResultCount != 0) {
        labResultCount->setText(QString("共 %1 条").arg(resultCount));
    }

    if (labResultCurrent != 0) {
        labResultCurrent->setText(QString("每页 %1 条").arg(resultCurrent));
    }

    if (labInfo != 0) {
        labInfo->setText(QString("共%1页,%2条记录").arg(pageCount).arg(resultCount));
    }

    //发送结果信号
    emit receivePage(pageCurrent, pageCount, resultCount, resultCurrent);
}

void DbPage::slot_receiveCount(quint32 count, double msec)
{
    if (labResult != 0) {
        labResult->setText(QString("查询用时 %1 秒").arg(QString::number(msec / 1000, 'f', 3)));
    }
    resultCount = count;
    int yushu = resultCount % resultCurrent;
    if (yushu == 0) {
        if (resultCount > 0 && resultCount < resultCurrent) {
            pageCount = 1;
        } else {
            pageCount = resultCount / resultCurrent;
        }
    } else {
        pageCount = (resultCount / resultCurrent) + 1;
    }
    boxSetPage->setRange(1,pageCount);
    if (pageCount <= 1) {
        btnFirst->setEnabled(false);
        btnLast->setEnabled(false);
        btnNext->setEnabled(false);
        btnPre->setEnabled(false);
    } else {
        btnFirst->setEnabled(true);
        btnLast->setEnabled(true);
        btnNext->setEnabled(true);
        btnPre->setEnabled(true);
    }
    if(startIndex==0)
    {
        btnFirst->setEnabled(false);
        btnPre->setEnabled(false);
    }
    tempSql = QString("select %1 from %2 %3 order by %4").arg(selectColumn).arg(tableName).arg(whereSql).arg(orderSql);
    sql = QString("%1 limit %2,%3;").arg(tempSql).arg(startIndex).arg(resultCurrent);
    bindData(sql);
}

void DbPage::first()
{
    if (pageCount > 1) {
        pageCurrent = 1;
        boxSetPage->setValue(pageCurrent);
    }
}

void DbPage::previous()
{
    if (pageCurrent > 1) {
        pageCurrent--;
        boxSetPage->setValue(pageCurrent);
    }
}

void DbPage::next()
{
    if (pageCurrent < pageCount) {
        pageCurrent++;
        boxSetPage->setValue(pageCurrent);
    }
}

void DbPage::last()
{
    if (pageCount > 0) {
        pageCurrent = pageCount;
        boxSetPage->setValue(pageCurrent);
    }
}
void DbPage::setPage(int pageIndex)
{
    boxSetPage->setEnabled(false);
    if (pageIndex >= 1 && pageIndex <= (int)pageCount) {
        pageCurrent = pageIndex;
        startIndex = (pageCurrent - 1)*resultCurrent;
        sql = QString("%1 limit %2,%3;").arg(tempSql).arg(startIndex).arg(resultCurrent);
        bindData(sql);
        btnFirst->setEnabled(true);
        btnPre->setEnabled(true);
    }
    if (pageCount > 0 && pageCurrent <= 1) {
        btnFirst->setEnabled(false);
        btnPre->setEnabled(false);
    }else{
        btnFirst->setEnabled(true);
        btnPre->setEnabled(true);
    }
    if (pageCount > 0 && pageCurrent >= (int)pageCount) {
        btnLast->setEnabled(false);
        btnNext->setEnabled(false);
    }else{
        btnLast->setEnabled(true);
        btnNext->setEnabled(true);
    }
    boxSetPage->setEnabled(true);
}
void DbPage::setControl(QTableView *tableView,
                        QLabel *labPageCount, QLabel *labPageCurrent,
                        QLabel *labResultCount, QLabel *labResultCurrent,
                        QLabel *labResult, QLabel *labInfo,
                        QAbstractButton *btnFirst, QAbstractButton *btnPre,
                        QAbstractButton *btnNext, QAbstractButton *btnLast,
                        QSpinBox *boxSetPage,const QString &countName, const QString &connName)
{
    this->tableView = tableView;
    this->labPageCount = labPageCount;
    this->labPageCurrent = labPageCurrent;
    this->labResultCount = labResultCount;
    this->labResultCurrent = labResultCurrent;
    this->labResult = labResult;
    this->labInfo = labInfo;
    this->btnFirst = btnFirst;
    this->btnPre = btnPre;
    this->btnNext = btnNext;
    this->btnLast = btnLast;
    this->boxSetPage=boxSetPage;
    this->countName = countName;
    this->connName = connName;
    connect(btnFirst, SIGNAL(clicked()), this, SLOT(first()));
    connect(btnPre, SIGNAL(clicked()), this, SLOT(previous()));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(next()));
    connect(btnLast, SIGNAL(clicked()), this, SLOT(last()));
    connect(boxSetPage,SIGNAL(valueChanged(int)),this,SLOT(setPage(int)));
}
void DbPage::setDbFile(const QString &dbFile){
    this->dbFile=dbFile;
}
void DbPage::setConnName(const QString &connName)
{
    this->connName = connName;
}

void DbPage::setDbType(const DbPage::DbType &dbType)
{
    this->dbType = dbType;
}

void DbPage::setTableName(const QString &tableName)
{
    this->tableName = tableName;
}

void DbPage::setSelectColumn(const QString &selectColumn)
{
    this->selectColumn = selectColumn;
}

void DbPage::setOrderSql(const QString &orderSql)
{
    this->orderSql = orderSql;
}

void DbPage::setWhereSql(const QString &whereSql)
{
    this->whereSql = whereSql;
}

void DbPage::setResultCurrent(int resultCurrent)
{
    this->resultCurrent = resultCurrent;
}

void DbPage::setColumnNames(const QList<QString> &columnNames)
{
    this->columnNames = columnNames;
}

void DbPage::setColumnWidths(const QList<int> &columnWidths)
{
    this->columnWidths = columnWidths;
}

void DbPage::setAllCenter(bool allCenter)
{
    queryModel->setAllCenter(allCenter);
}

void DbPage::setAlignCenterColumn(const QList<int> &alignCenterColumn)
{
    queryModel->setAlignCenterColumn(alignCenterColumn);
}

void DbPage::setAlignRightColumn(const QList<int> &alignRightColumn)
{
    queryModel->setAlignRightColumn(alignRightColumn);
}

void DbPage::setInsertColumnIndex(int insertColumnIndex)
{
    this->insertColumnIndex = insertColumnIndex;
}

void DbPage::setInsertColumnName(const QString &insertColumnName)
{
    this->insertColumnName = insertColumnName;
}

void DbPage::setInsertColumnWidth(int insertColumnWidth)
{
    this->insertColumnWidth = insertColumnWidth;
}

void DbPage::select()
{
    startIndex = 0;
    pageCurrent = 1;

    slot_receiveCount(resultCurrent, 0);

    btnFirst->setEnabled(false);
    btnLast->setEnabled(false);
    btnNext->setEnabled(false);
    btnPre->setEnabled(false);

    QString info = "正在查询...";

    if (labInfo != 0) {
        labInfo->setText(info);
    }

    if (labPageCurrent != 0) {
        labPageCurrent->setText(info);
    }

    if (labPageCount != 0) {
        labPageCount->setText(info);
    }

    if (labResultCount != 0) {
        labResultCount->setText(info);
    }

    if (labResultCurrent != 0) {
        labResultCurrent->setText(info);
    }

    if (labResult != 0) {
        labResult->setText(info);
    }
    tempSql = QString("select count(%1) from %2 %3").arg(countName).arg(tableName).arg(whereSql);
    DbCountThread *dbCountThread = new DbCountThread(this);
    connect(dbCountThread, SIGNAL(receiveCount(quint32, double)), this, SIGNAL(receiveCount(quint32, double)));
    connect(dbCountThread, SIGNAL(receiveCount(quint32, double)), this, SLOT(slot_receiveCount(quint32, double)));
    dbCountThread->setConnName(connName);
    dbCountThread->setDbFile(dbFile);
    dbCountThread->setSql(tempSql);
    dbCountThread->start();
}
