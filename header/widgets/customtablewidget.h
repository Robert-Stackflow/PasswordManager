#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QWidget>
#include "controller/dbpage.h"
namespace Ui {
class CustomTableWidget;
}

class CustomTableWidget : public QWidget
{
    Q_OBJECT
public:
    enum MODE{
        GROUPS,
        GROUP,
        GROUPTYPES,
        GROUPTYPE
    };
private:
    QString styleSheet;
    Ui::CustomTableWidget *ui;
    MODE currentMode;
    DbPage *dbPage;
    QStringList columnNames;    //字段名集合
    QList<int> columnWidths;    //字段宽度集合
    int resultCount=20;            //每页记录条数
    QString dbFilePath;         //数据库文件路径
    QString connName;           //数据库连接名称
    QString tableName;          //数据表名称
    QString countName;          //统计记录条数的字段名称
public:
    explicit CustomTableWidget(QWidget *parent = nullptr);
    explicit CustomTableWidget(MODE currentMode,QString dbFilePath,QString connName,QString tableName,
                               QStringList columnNames,QList<int> columnWidths,QWidget *parent = nullptr);
    ~CustomTableWidget();
private:
    void initWidget();
public:
    void refresh();
    void setName(const QString& name);
    void setDescribe(const QString& describe);
    void setCountName(const QString& countName);
    void setResultCount(int resultCount);
    void setConnName(const QString& connName);
    void setDbFilePath(const QString& dbFilePath);
    void setTableName(const QString& tableName);
    void setColumnNames(QStringList columnNames);
    void setColumnWidths(QList<int> columnWidths);
    void setVerticalHeaderVisible(bool verticalHeaderVisible);
    void setHorizontalHeaderVisible(bool horizontalHeaderVisible);
    void setToolTips(const QString& refreshTip,const QString& exportTip,const QString& importTip,const QString& addTip,const QString& moreTip);
private slots:
    void onButtonsClicked();
    void onCountPerPageChanged(int);
    void onContextMenu(QPoint pos);
    void onHeaderContextMenu(QPoint pos);
signals:
    void moreClicked(const QString& name);
    void addClicked(const QString& name);
    void exportClicked(const QString& name);
};

#endif // CUSTOMTABLEWIDGET_H
