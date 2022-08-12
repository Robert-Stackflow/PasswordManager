#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H
#include "abstractCustomField.h"
namespace Ui {
class customTextEdit;
}

class customTextEdit : public AbstractCustomField
{
    Q_OBJECT
private:
    Ui::customTextEdit *ui;
public:
    //构造函数与析构函数
    explicit customTextEdit(const FieldData& fieldData,QWidget* parent=nullptr);
    ~customTextEdit();
    //实现抽象函数
    bool isValid();
    QString text();
    void setFocus();
    AbstractCustomField *clone();
    QString getPlaceholderText();
    void setPlaceholderText(const QString &placeholderText);
    //与QTextEdit有关的属性
    void clear();
    QString toPlainText();
    void setText(const QString &text);
public slots:
    void onControllerEdited();
};

#endif // CUSTOMTEXTEDIT_H
