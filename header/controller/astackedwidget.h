#ifndef ASTACKEDWIDGET_H
#define ASTACKEDWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>

class AStackedWidget : public QStackedWidget
{
    Q_OBJECT
private:
    int m_offset;
    int m_curIndex;
    int m_lastIndex;
    int m_duration;
    QList<QWidget*> m_widgetLst;
    QPropertyAnimation* m_moveAnimation;
public:
    AStackedWidget(QWidget *parent);
    ~AStackedWidget();
    int count() const;
    int currentIndex() const;
    void setDuration(int duration);
    int addWidget(QWidget * widget);
    int indexOf(QWidget * widget) const;
    int insertWidget(int index, QWidget * widget);
    QWidget * currentWidget() const;
    QWidget * widget(int index) const;
    void removeWidget(QWidget * widget);
    void resizeEvent(QResizeEvent *event);
    void onValueChanged(const QVariant &value);
    void moveAnimationStart();
    void setWidgetsVisible();
public Q_SLOTS:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *w);
Q_SIGNALS:
    void currentChanged(int);
    void widgetRemoved(int index);
};

#endif // ASTACKEDWIDGET_H
