#ifndef NEWITEMDIALOG_H
#define NEWITEMDIALOG_H
#include <QDialog>
#include "controller/customTitleBar.h"
#include "customField/abstractCustomField.h"
#include "structure/keyItem.h"
namespace Ui {
class newItemDialog;
}

class newItemDialog : public QDialog
{
    Q_OBJECT
public:
    CustomTitleBar *m_titleBar;
    Ui::newItemDialog *ui;
    //辅助变量
    KeyItem* newKeyItem;
private:
    int currentGroupType;
    bool isDataValid=false;
    QList<AbstractCustomField*> fieldControllerList;
    QString styleSheet;
public:
    explicit newItemDialog(QWidget *parent = nullptr);
    ~newItemDialog();
    void loadGroupType(int index);
private:
    void InitDialog();
private slots:
    void onCancelClicked();
    void onConfirmClicked();
};

#endif // NEWITEMDIALOG_H
