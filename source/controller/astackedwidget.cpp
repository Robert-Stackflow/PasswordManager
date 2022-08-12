#include "controller/astackedwidget.h"
#include "qstackedwidget.h"
#include <QDebug>
#include <QPropertyAnimation>

AStackedWidget::AStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    m_offset = 0;
    m_curIndex = 0;
    m_lastIndex = 0;
    m_duration = 500;
    m_moveAnimation = new QPropertyAnimation(this, "");
    m_moveAnimation->setDuration(m_duration);
    connect(m_moveAnimation, &QPropertyAnimation::valueChanged, this, &AStackedWidget::onValueChanged);
}

AStackedWidget::~AStackedWidget()
{

}

int AStackedWidget::count() const
{
    return m_widgetLst.size();
}

int AStackedWidget::currentIndex() const
{
    return m_curIndex;
}

void AStackedWidget::setDuration(int duration)
{
    m_duration = duration;
}

int AStackedWidget::addWidget(QWidget * widget)
{
    int index = indexOf(widget);
    if (index >= 0){
        return index;
    }
    widget->setParent(this);
    m_widgetLst.append(widget);
    return count() - 1;
}

int AStackedWidget::indexOf(QWidget * widget) const
{
    return m_widgetLst.indexOf(widget);
}

int AStackedWidget::insertWidget(int index, QWidget * widget)
{
    int curindex = indexOf(widget);
    if (curindex >= 0) {
        return curindex;
    }
    widget->setParent(this);
    m_widgetLst.insert(index, widget);
    return index;
}

QWidget * AStackedWidget::currentWidget() const
{
    if (m_curIndex >= 0 && m_curIndex < count()){
        return m_widgetLst.at(m_curIndex);
    }
    return nullptr;
}

QWidget * AStackedWidget::widget(int index) const
{
    if (index >= 0 && index < count()) {
        return m_widgetLst.at(index);
    }
    return nullptr;
}

void AStackedWidget::removeWidget(QWidget * widget)
{
    int index = indexOf(widget);
    if (index >= 0) {
        m_widgetLst.removeAll(widget);
        emit widgetRemoved(index);
    }
}

void AStackedWidget::setCurrentWidget(QWidget * widget)
{
    int index = indexOf(widget);
    if (index >= 0 && m_curIndex != index) {
        setCurrentIndex(index);
    }
}

void AStackedWidget::setCurrentIndex(int index)
{
    if (index >= 0 && m_curIndex != index) {
        m_lastIndex = m_curIndex;
        m_curIndex = index;
        moveAnimationStart();
        emit currentChanged(index);
    }
}

void AStackedWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int size = count();
    for (int i = 0; i < size; i++) {
        m_widgetLst.at(i)->resize(this->width(), this->height());
    }

    if (m_moveAnimation->state() == QAbstractAnimation::Running) {
        moveAnimationStart();
    }
    else {
        setWidgetsVisible();
    }
}

void AStackedWidget::onValueChanged(const QVariant &value)
{
    m_offset = value.toInt();
    m_widgetLst.at(m_curIndex)->move(m_offset, 0);
    if (m_curIndex > m_lastIndex) {
        m_widgetLst.at(m_lastIndex)->move(m_offset - this->width(), 0);
    }
    else {
        m_widgetLst.at(m_lastIndex)->move(this->width() + m_offset, 0);
    }
}

void AStackedWidget::moveAnimationStart()
{
    m_moveAnimation->stop();
    setWidgetsVisible();
    int startOffset = m_offset;
    if (m_curIndex > m_lastIndex) {
        if (startOffset == 0) startOffset = this->width();
        else startOffset = this->width() - qAbs(startOffset);
    }
    else {
        if (startOffset == 0) startOffset = -this->width();
        else startOffset = qAbs(startOffset) - this->width();
    }
    m_moveAnimation->setDuration(qAbs(startOffset) * m_duration / this->width());
    m_moveAnimation->setStartValue(startOffset);
    m_moveAnimation->setEndValue(0);
    m_moveAnimation->start();
}

void AStackedWidget::setWidgetsVisible()
{
    int size = count();
    for (int i = 0; i < size; i++) {
        if (m_lastIndex == i || m_curIndex == i)
            m_widgetLst.at(i)->setVisible(true);
        else {
            m_widgetLst.at(i)->setVisible(false);
        }
    }
}
