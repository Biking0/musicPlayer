//鼠标事件，光标移入，移出，单击

#include "eventlabel.h"
#include <QDebug>
#include <QPainter>

EventLabel::EventLabel(QWidget *parent):QLabel(parent)
{

}

EventLabel::~EventLabel()
{

}

//光标移入事件
void EventLabel::enterEvent(QEvent *)
{
    emit enterSignal();
}

//单击事件
void EventLabel::mousePressEvent(QMouseEvent * ev)
{
    emit clickSignal();
}

//移出事件
void EventLabel::leaveEvent(QEvent *)
{
    emit leaveSignal();
}
