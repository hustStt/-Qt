#ifndef TYPE_H
#define TYPE_H

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOption>
#include <QApplication>
#pragma execution_character_set("utf-8")


#define MAX_MENU_NUM 4
#define MAX_TAB_NUM 4
#define MAX_ITEM_NUM 100
#define MAX_INPUT_NUM 10


#define IMAGE_W 50
#define IMAGE_H 50


#define IMAGE_PATH QApplication::applicationDirPath()+"/images/"
#define PATH QApplication::applicationDirPath()+"/../../"

typedef enum _NodeType
{
    //tab1
    ROOT_NODE = 0,
    SUB_SYS,
    DEVICE,
    SWITCH,
    MESSAGE,
    SOFTWARE,

    //tab2
    HARDWARE_NODE,
    DDS_NODE,

    //tab3
    SOFTWARE_NODE,

    DEFAULT_NODE

}NodeType;

typedef enum _ITEM_TYPE
{
    ELEMENT_ITEM = 0,
    OP_ITEM,
    OP_ITEM_SELECT
}ITEM_TYPE;

const QStringList _TABLE_HEAD[] =
{
    {"硬件",""},
    {"软件","标识"},
    {"消息","标识"}
};

const QString _ELEMENT_NAME[] =
{
    "子系统",
    "设备",
    "交换机"
};

class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 60; // 设置每个tabBar中item的大小
            s.rheight() = 60;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel)
        {
            if(const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
            {
                QRect allRect = tab->rect; //获取tab框

                if(tab->state & QStyle::State_Selected)
                {
                    painter->save();
                    painter->setPen(0xc9c9c9);//tab边框颜色
                    painter->setBrush(QBrush(Qt::white));//tab颜色
                    painter->drawRect(allRect.adjusted(0, 0, 0, 0));//调整高亮
                    painter->restore();
                }
                else
                {
                    painter->save();
                    painter->setPen(0xc9c9c9);//tab边框颜色
                    painter->setBrush(QBrush(0xf0f0f0));//tab颜色
                    painter->drawRect(allRect.adjusted(3, 0, -1, -1));//调整高亮
                    painter->restore();
                }


                QTextOption option;//文本样式
                option.setAlignment(Qt::AlignCenter);//设置对其方式

                if(tab->state & QStyle::State_Selected)
                {
                    painter->setPen(Qt::black);
                }
                else{
                    painter->setPen(Qt::black);
                }

                //painter words
                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab)
        {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

#endif // TYPE_H
