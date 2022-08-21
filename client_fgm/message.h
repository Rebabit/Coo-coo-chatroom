#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>

class Message : public QWidget
{
    Q_OBJECT
public:
    explicit Message(QWidget *parent = nullptr);
    Message(QString messagename,QWidget *parent = nullptr);

    //发送者类别
    enum UserType
    {
        userMe,    //自己
        userOther, //他人
        userTime,  //时间
    };

    //设置基本属性
    void setTextContent(QString txt, QString t, QSize allSize, UserType type);
    //计算聊天气泡的size
    QSize setSize(QString str);
    //设置文本动态换行 并获取发送文字的size
    QSize getStringSize(QString str);
    //画图事件
    void paintEvent(QPaintEvent *event);
    //制作头像
    QPixmap paintHead(const QChar &name);

    inline  UserType userType()
    {
        return m_userType;
    }
    inline QString text()
    {
        return m_msg;
    }
    inline QString time()
    {
        return  m_time;
    }
    QString MessageName;

private:
    UserType m_userType;

    QSize m_wholeSize; //总规格 ???
    QString m_msg;
    QString m_time;
    QString m_currentTime;

    QPixmap m_meRightIcon;//自己 右头像
    QPixmap m_otherLeftIcon;//他人 左头像

    QRect m_iconRightRect;//左头像 所在矩形
    QRect m_iconLeftRect;//右头像 所在矩形
    QRect m_triRightRect;//右三角
    QRect m_triLeftRect;//左三角
    QRect m_kuangLeftRect;// ???
    QRect m_kuangRightRect;// ???
    QRect m_textLeftRect;//???
    QRect m_textRightRect;//???

    int m_kuangWidth;//聊天框宽度 ???
    int m_textWidth;//聊天气泡内文本宽度
    int m_spaceWidth;//???
    int m_lineHeight;//基线之间的距离 ???
signals:

};

#endif // MESSAGE_H
