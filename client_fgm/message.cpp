#include "message.h"
#include <QDateTime>
#include <QPainter>
#include <QDebug>
Message::Message(QWidget *parent) : QWidget(parent)
{
    //设置字体

    QFont textFont("MicrosoftYaHei", 12);
    this->setFont(textFont);
    //缓冲按钮暂定
}
Message::Message(QString messagename,QWidget *parent): QWidget(parent)
{
    QFont textFont("MicrosoftYaHei", 12);
    this->setFont(textFont);
    MessageName = messagename;
}
QPixmap Message::paintHead(const QChar &name)
{
    QSize size(50,50); //指定图片大小;
    QImage image(size,QImage::Format_ARGB32);
    //以ARGB32格式构造一个QImage
    //image.fill(qRgba(0,0,0,100));//填充图片背景,0-255为透明度
    image.fill(qRgba(52,73,94,255));
    QPainter painter(&image); //为这个QImage构造一个QPainter
    painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
    //设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
    //改变组合模式和上面的填充方式可以画出透明的图片。

    //改变画笔和字体
    QPen pen = painter.pen();
    pen.setColor(QColor(255,255,255,255));
    QFont font = painter.font();
    font.setFamily("Microsoft YaHei");
    font.setBold(true);//加粗
    font.setPixelSize(25);//改变字体大小

    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(image.rect(),Qt::AlignCenter, name);
    //将Hello写在Image的中心ui
    return QPixmap::fromImage(image);
}
void Message::setTextContent(QString text, QString time, QSize size, UserType type)
{
    m_msg = text;
    m_userType = type;
    m_time = time;
    m_currentTime = QDateTime::fromTime_t(time.toInt()).toString("hh:mm");//时间戳 自1970年过去的秒数
    m_wholeSize = size;

    //自定义头像 next
    m_meRightIcon = paintHead(MessageName[0]);
    m_otherLeftIcon = paintHead(MessageName[0]);
    //需增加名称显示 next
}
QSize Message::setSize(QString str)
{
    int minHeight = 30;           //聊天气泡最小高度
    int iconWidth = 40;           //头像宽度 同头像长度
    int iconSpaceWidth = 20;      //头像与聊天界面框 长度距离
    int iconSpaceHeight= 10;      //头像与聊天界面框 高度距离
    int iconRectWidth = 5;        //头像与小三角的距离
    int triWidth = 6;             //小三角长度
    int kuangTMP = 20;            //??????
    int textSpaceWidth = 12;      //聊天气泡中 文本距单侧气泡框空白宽度

    m_msg = str;
    m_kuangWidth = this->width() - kuangTMP - 2 * (iconWidth + iconSpaceWidth + iconRectWidth);
    m_textWidth = m_kuangWidth - 2 * textSpaceWidth;
    m_spaceWidth = this->width() - m_textWidth;
    m_iconLeftRect = QRect(iconSpaceWidth, iconSpaceHeight, iconWidth, iconWidth);
    m_iconRightRect = QRect(this->width() - iconSpaceWidth - iconWidth, iconSpaceHeight, iconWidth, iconWidth);

    QSize size = getStringSize(m_msg);
    int height = size.height() < minHeight ? minHeight : size.height();

    m_triLeftRect = QRect(iconSpaceWidth + iconWidth + iconRectWidth, m_lineHeight / 2, triWidth, height - m_lineHeight);//???
    m_triRightRect = QRect(this->width() - iconSpaceWidth - iconWidth - iconRectWidth - triWidth, m_lineHeight / 2, triWidth, height - m_lineHeight);

    if(size.width() < (m_textWidth + m_spaceWidth))
    {
        m_kuangLeftRect.setRect(m_triLeftRect.x() + m_triLeftRect.width(), m_lineHeight / 4 * 3, size.width() - m_spaceWidth + 2 * textSpaceWidth, height - m_lineHeight);
        m_kuangRightRect.setRect(this->width() - size.width() + m_spaceWidth - 2 * textSpaceWidth - iconWidth - iconSpaceWidth - iconRectWidth - triWidth,
                                 m_lineHeight / 4 * 3, size.width() - m_spaceWidth + 2 * textSpaceWidth, height - m_lineHeight);
    }
    else
    {
        m_kuangLeftRect.setRect(m_triLeftRect.x() + m_triLeftRect.width(), m_lineHeight / 4 * 3, m_kuangWidth, height - m_lineHeight);
        m_kuangRightRect.setRect(iconWidth + kuangTMP + iconSpaceWidth + iconRectWidth - triWidth, m_lineHeight/4*3, m_kuangWidth, height-m_lineHeight);
    }

    m_textLeftRect.setRect(m_kuangLeftRect.x() + textSpaceWidth, m_kuangLeftRect.y() + iconSpaceHeight,
                           m_kuangLeftRect.width() - 2 * textSpaceWidth, m_kuangLeftRect.height() - 2 * iconSpaceHeight);
    m_textRightRect.setRect(m_kuangRightRect.x() + textSpaceWidth, m_kuangRightRect.y() + iconSpaceHeight,
                            m_kuangRightRect.width() - 2 * textSpaceWidth, m_kuangRightRect.height() -2 * iconSpaceHeight);

    return QSize(size.width(), height);
}
QSize Message::getStringSize(QString str)
{
    QFontMetricsF fm(this->font());
    m_lineHeight = fm.lineSpacing();   //行间距
    int nCount = str.count("\n");      //\n数量
    int textMaxWidth;

    if (nCount == 0)
    {//实际文本无换行
        textMaxWidth = fm.width(str);//返回给定文本中字符的宽度
        if (textMaxWidth > m_textWidth)
        {//实际文本宽度大于当前可实现文本宽度，根据当前文本宽度重新编辑文本 \n
            textMaxWidth = m_textWidth;
            int size = m_textWidth / fm.width(" ");//每行字符数
            int num = fm.width(str) / m_textWidth;//需换行数
            nCount += num;
            QString strAfter = "";
            for (int i = 0; i < num ; i++)
            {
                strAfter += str.mid(i * size, (i + 1) * size) + "\n";
            }
            str = strAfter;
        }
    }
    else
    {//实际文本有换行
        for (int i = 0; i <= nCount; i++)
        {
            QString strSplit = str.split("\n").at(i);//依据实际文本的换行符进行分割
            textMaxWidth = fm.width(strSplit) > textMaxWidth ? fm.width(strSplit) : textMaxWidth;
            if (fm.width(strSplit) > m_textWidth)
            {
                textMaxWidth = m_textWidth;
                int size = m_textWidth / fm.width(" ");//每行字符数
                int num = fm.width(strSplit) / m_textWidth;//需换行数
                num = ((i + num) * fm.width(" ") + fm.width(strSplit)) / m_textWidth; //???
                nCount += num;
                QString strAfter = "";
                for (int i = 0; i < num; i++)
                {
                    strAfter += strSplit.mid(i * size, (i + 1) * size) + "\n";
                }
                str.replace(strSplit, strAfter);//需改进
            }
        }
    }

    //换行效果需增强
    return QSize(textMaxWidth + m_spaceWidth, (nCount + 1) * m_lineHeight + 2 * m_lineHeight);
}
void Message::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
    painter.setPen(Qt::NoPen);//无线
    painter.setBrush(QBrush(Qt::gray));//形状填充为 灰色 纯色图案

    if(m_userType == UserType::userOther) { // 聊天对象
        //放置头像
        painter.drawPixmap(m_iconLeftRect, m_otherLeftIcon);

        //框加边
        QColor col_KuangB(234, 234, 234);
        painter.setBrush(QBrush(col_KuangB));
        painter.drawRoundedRect(m_kuangLeftRect.x() - 1,m_kuangLeftRect.y() - 1,m_kuangLeftRect.width()+2,m_kuangLeftRect.height()+2,4,4);//圆角框

        //框 气泡
        QColor col_Kuang(255,255,255);
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(m_kuangLeftRect,4,4);

        //三角
        QPointF points[3] = {
            QPointF(m_triLeftRect.x(), 30),//
            QPointF(m_triLeftRect.x()+m_triLeftRect.width(), 25),
            QPointF(m_triLeftRect.x()+m_triLeftRect.width(), 35),
        };
        QPen pen;
        pen.setColor(col_Kuang);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);//画多边形

        //三角加边
        QPen penSanJiaoBian;
        penSanJiaoBian.setColor(col_KuangB);
        painter.setPen(penSanJiaoBian);
        painter.drawLine(QPointF(m_triLeftRect.x() - 1, 30), QPointF(m_triLeftRect.x()+m_triLeftRect.width(), 24));
        painter.drawLine(QPointF(m_triLeftRect.x() - 1, 30), QPointF(m_triLeftRect.x()+m_triLeftRect.width(), 36));

        //内容
        QPen penText;
        penText.setColor(QColor(51,51,51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);//左对齐 中心水平对齐
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);//包围字体
        painter.setFont(this->font());
        painter.drawText(m_textLeftRect, m_msg,option);//写文本
    }  else if(m_userType == UserType::userMe) { // 自己
        //头像
        painter.drawPixmap(m_iconRightRect, m_meRightIcon);
        //框
        QColor col_Kuang(75,164,242);
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(m_kuangRightRect,4,4);

        //三角
        QPointF points[3] = {
            QPointF(m_triRightRect.x()+m_triRightRect.width(), 30),
            QPointF(m_triRightRect.x(), 25),
            QPointF(m_triRightRect.x(), 35),
        };
        QPen pen;
        pen.setColor(col_Kuang);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        //内容
        QPen penText;
        penText.setColor(Qt::white);
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(m_textRightRect,m_msg,option);
    }  else if(m_userType == UserType::userTime) { // 时间
        QPen penText;
        penText.setColor(QColor(153,153,153));
        painter.setPen(penText);
        QTextOption option(Qt::AlignCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        QFont te_font = this->font();
        te_font.setFamily("MicrosoftYaHei");
        te_font.setPointSize(10);
        painter.setFont(te_font);
        painter.drawText(this->rect(),m_currentTime,option);
    }
}
