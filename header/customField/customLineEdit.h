#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H
#include "abstractCustomField.h"
namespace Ui {
class customLineEdit;
}

class customLineEdit : public AbstractCustomField
{
    Q_OBJECT
private:
    Ui::customLineEdit *ui;
public:
    //构造函数/析构函数
    explicit customLineEdit(const FieldData& fieldData,QWidget* parent=nullptr);
    ~customLineEdit();
    //实现抽象函数
    bool isValid();
    QString text();
    void setFocus();
    AbstractCustomField *clone();
    QString getPlaceholderText();
    void setPlaceholderText(const QString &placeholderText);
    //与QLineEdit相关的属性
    void clear();
    void setText(const QString &text);
    int maxLength();
    void setMaxLength(int length);
    void setCompleter(QCompleter* completer);
public slots:
    void onPasswordJudgeClicked();
    void onControllerEdited(const QString &arg);
};

#endif // CUSTOMLINEEDIT_H
