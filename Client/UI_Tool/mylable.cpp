#include "mylable.h"
#include <QPainter>
MyLable::MyLable(QWidget* parent):QLabel (parent)
{
   m_set = false;

}

void MyLable::paintEvent(QPaintEvent *e)
{
    if(nullptr != pixmap())
    {
        QPainter painter(this);
        //设置反锯齿
        painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::SmoothPixmapTransform);
        QPainterPath path;
        int round = qMin(width(),height());
        path.addEllipse(0,0,round,round);
        painter.setClipPath(path);
        oldMap = *pixmap(); //获取原来的
        if(m_set)
        {
            QPixmap pixMap(":/images/images/2.jpg");
            painter.drawPixmap(-1,-1,width()+10,height()+10,pixMap);
        }
        else {
            painter.drawPixmap(-1,-1,width()+10,height()+10,oldMap);
        }


    }
    else {
        QLabel::paintEvent(e);
    }
}

void MyLable::mousePressEvent(QMouseEvent *ev)
{
     m_set = false;
     update();
     QLabel::mousePressEvent(ev);
}

void MyLable::mouseMoveEvent(QMouseEvent *ev)
{
    m_set = false;
    update();
    QLabel::mouseMoveEvent(ev);
}

void MyLable::mouseReleaseEvent(QMouseEvent *ev)
{
    m_set = false;
    update();
    emit signClick();
    QLabel::mouseReleaseEvent(ev);
}
