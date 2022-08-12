#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H
#include "abstractCustomField.h"
namespace Ui {
class customComboBox;
}

class customComboBox :public AbstractCustomField
{
    Q_OBJECT
private:
    Ui::customComboBox *ui;
public:
    //构造函数/析构函数
    explicit customComboBox(const FieldData& fieldData,QWidget* parent=nullptr);
    ~customComboBox();
    //实现抽象函数
    bool isValid();
    QString text();
    void setFocus();
    AbstractCustomField* clone();
    QString getPlaceholderText();
    void setPlaceholderText(const QString &placeholderText);
    //与QComboBox有关的属性
    int count();
    void addItem(const QString& item);
    void addItems(const QStringList& items);
    void addItems(const QList<QString>& items);
    int currentIndex();
    void setCurrentIndex(int index);
    QString currentText();
    void setCurrentText(const QString& text);
    void setEnable(bool enable);
    void setEditable(bool editable);
    bool isEnable();
    bool isEditable();
    void setCompleter(QCompleter* completer);
    void clear();
    QStringList items();
public slots:
    void onControllerCurrentIndexChanged(int);
    void onControllerEdited(const QString &arg);
signals:
    void currentIndexChanged();
};

#endif // CUSTOMCOMBOBOX_H
