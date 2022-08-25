#ifndef MYLABLE_H
#define MYLABLE_H

#include <QObject>
#include<QLabel>
class MyLable :public QLabel
{
    Q_OBJECT
public:
    MyLable(QWidget* parent = nullptr);
signals:
    void signClick();
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    bool m_set;//透明度的
    QPixmap oldMap;
};

#endif // MYLABLE_H
