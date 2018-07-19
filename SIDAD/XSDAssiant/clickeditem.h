#ifndef CLICKEDITEM_H
#define CLICKEDITEM_H

#pragma execution_character_set("utf-8")
#include <QGraphicsItem>
#include "type.h"
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <vector>
#include <QAction>
#include <rightclickedmenu.h>
#include "update_ui.h"

class ClickedItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    int x,y; //pos
    int root = 0;
    int width = IMAGE_W*2;
    int height = IMAGE_H*2;
    int rest_size[2];//每层剩余的空间，子系统使用
    int nodetype;//类型 用于区分节点
    int id = 0;//item唯一标识
    ClickedItem *parent = nullptr;//父节点指针
    int depth = 0;
    void* nodeptr = NULL;//指向记录其信息的指针
    void *item_node = NULL;//指向对应treeweight中项的指针
    void *item_node_temp = NULL;//指向对应treeweight中项的指针
    std::vector <ClickedItem *> sons;
    int elements_flag = ELEMENT_ITEM;//区分状态
    int changeState = 1;//能否修改标识
    QString item_name;//名称
    QPen pen;
    QPixmap *item_pixmap;//图标
    RightClickedMenu *rightClickedMenu;
    update_ui * update_signal;
    QAction *temp_act[2];
    ClickedItem():x(-100),y(-100),nodetype(SUB_SYS)
    {
        sons.clear();
        setAcceptDrops(true);
        setItemInfo();
    }
    ClickedItem(int _x, int _y):x(_x),y(_y),nodetype(SUB_SYS)
    {
        sons.clear();
        setAcceptDrops(true);
        setItemInfo();
    }
    ClickedItem(int _x, int _y,int nodetype,int node_flag = ELEMENT_ITEM):x(_x),y(_y),nodetype(nodetype),elements_flag(node_flag)
    {
        sons.clear();
        setAcceptDrops(true);
        setItemInfo();
    }
    ClickedItem(int _x, int _y,int _width,int _height,int nodetype,int node_flag = ELEMENT_ITEM):
        x(_x),y(_y),width(_width),height(_height),nodetype(nodetype),elements_flag(node_flag)
    {
        sons.clear();
        setAcceptDrops(true);
        setItemInfo();
    }

    void setItemInfo();
    void addSon(const QMimeData *mimeData);
    void setParent(ClickedItem *item);
    void setSize(int w,int h);
    void setPos(int x,int y);
    void updateTree();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    //void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

    void del_software_item();
    void del_dev_item();
    void del_subsys();
    void del_software_ele();
    void del_message_ele();
signals:
    void sendString(QString qstr);
    void pushBackItem(ClickedItem *item);
public slots:
    void show_item();
    void del_item();
};

#endif // CLICKEDITEM_H
