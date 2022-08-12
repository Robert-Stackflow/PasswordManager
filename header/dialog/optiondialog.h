#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H
#include <QDialog>
#include "controller/customTitleBar.h"
namespace Ui {
class optionDialog;
}

class optionDialog : public QDialog
{
    Q_OBJECT

public:
    CustomTitleBar *m_titleBar;
    explicit optionDialog(QWidget *parent = nullptr);
    ~optionDialog();

private:
    Ui::optionDialog *ui;
    QString styleSheet;
};

#endif // OPTIONDIALOG_H
