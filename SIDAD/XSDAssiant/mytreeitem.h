#ifndef MYTREEITEM_H
#define MYTREEITEM_H

#include <QTabWidget>
#include <QTreeWidget>
#include "clickeditem.h"

class MyTreeItem : public QObject,public QTreeWidgetItem
{
    Q_OBJECT//信号和槽
public:
    ClickedItem *item_ptr = nullptr;
    MyTreeItem(QTreeWidgetItem *parent,const QStringList list):QTreeWidgetItem::QTreeWidgetItem(parent,list){}
    MyTreeItem(QTreeWidget *parent,const QStringList list):QTreeWidgetItem::QTreeWidgetItem(parent,list){}
    /*void setCheckState(int column,Qt::CheckState state)
    {
        QTreeWidgetItem::setCheckState(column,state);
        emit checkstate_changed(state == Qt::Unchecked ? false : true);
    }*/
signals:
    void checkstate_changed(bool);
};


#endif // MYTREEITEM_H
