#ifndef NEWGROUPTYPEDIALOG_H
#define NEWGROUPTYPEDIALOG_H
#include <QDialog>
#include "structure/groupType.h"
#include "controller/customTitleBar.h"
#include "customField/customLineEdit.h"
namespace Ui {
class newGroupTypeDialog;
}
class newGroupTypeDialog : public QDialog
{
    Q_OBJECT
public:
    enum mode{
        NORMAL=-1,
        NEWGROUPTYPE,
        EDITGROUPTYPE
    };
public:
    //表头
    QStringList typeManagerTableWidgetHeader;
    QStringList editFieldsTableWidgetHeader;
    //自定义控件
    CustomTitleBar *m_titleBar;
    customLineEdit* typeName;
    customLineEdit* addFieldName;
    customLineEdit* addFieldTip;
    //辅助变量
    QString chosenOldName;
    GroupType* newGroupType;
    GroupType* currentGroupType;
    QList<AbstractCustomField*> tempFields;
    //辅助变量
    int currentRow=-1;
    mode currentMode=NORMAL;
private:
    Ui::newGroupTypeDialog *ui;
    QString styleSheet;
public:
    explicit newGroupTypeDialog(QWidget *parent = nullptr);
    ~newGroupTypeDialog();
    virtual bool eventFilter(QObject *obj, QEvent *e) override;
    virtual void closeEvent(QCloseEvent* event) override;
    void loadGroupTypes();
private:
    void initDialog();
private slots:
    void onTypeManagerTableWidgetClicked();
    void onEditTypeClicked();
    void onDeleteTypeClicked();
    void onCopyTypeClicked();
    void onNewTypeClicked();
    void onCancelClicked();
    void onConfirmClicked();
    void onAddFieldClicked();
    void onIsReqiuredCheckBoxToggled(bool);
signals:
    void groupTypeCountChanged();
    void typeManagerTableWidgetClicked();
};

#endif // NEWGROUPTYPEDIALOG_H
