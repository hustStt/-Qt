#include "clickeditem.h"
#include "mydockwidget.h"
int id_count = 0;

void ClickedItem::setItemInfo()
{
    sons.clear();
    if(elements_flag)
    {
        id = id_count++;
    }
    else
    {
        id = -1;//ELEMENT_ITEM
    }
    switch(nodetype)
    {
        case SUB_SYS:
        {
            item_pixmap = new QPixmap(IMAGE_PATH + "system_icon.jpg");
            item_name = "子系统" + QString::number(id);
            pen.setStyle(Qt::DashLine);
            width = 6*IMAGE_W;
            height = 6*IMAGE_H;
            rest_size[0] = width - IMAGE_W*3/2;
            rest_size[1] = width - IMAGE_W*3/2;
            break;
        }

        case DEVICE:
        {
            item_pixmap = new QPixmap(IMAGE_PATH + "device_icon.jpg");
            item_name = "设备" + QString::number(id);
            break;
        }
        case SWITCH:
        {
            item_pixmap = new QPixmap(IMAGE_PATH + "switch_icon.jpg");
            item_name = "交换机" + QString::number(id);
            break;
        }
        case SOFTWARE:
        {
            item_pixmap = new QPixmap(IMAGE_PATH + "software_icon.jpg");
            item_name = "软件" + QString::number(id);
            break;
        }
        case MESSAGE:
        {
            item_pixmap = new QPixmap(IMAGE_PATH + "message_icon.jpg");
            item_name = "消息" + QString::number(id);
            break;
        }
        default:break;
    }
    rightClickedMenu = new RightClickedMenu();
    update_signal = update_ui::get_instance();
}

void ClickedItem::addSon(const QMimeData *mimeData)
{
    QStringList list = mimeData->text().split(' ');
    int mark = list.at(0).toInt() == SOFTWARE;

    ClickedItem *son;
    if(mark){
        son = new ClickedItem(parent->x + parent->width - parent->rest_size[mark],parent->y + IMAGE_H * 7/2,list.at(0).toInt(),OP_ITEM);
        qDebug()<<"addson";
    }
    else
        son = new ClickedItem(x + width - rest_size[mark],y + IMAGE_H,list.at(0).toInt(),OP_ITEM);
    son->setParent(this);
    sons.push_back(son);
    son->item_name = list.at(1);
    pushBackItem(son);
}

void ClickedItem::setParent(ClickedItem *item)
{
    int wi = nodetype == SUB_SYS ? 6 * IMAGE_W : 2 * IMAGE_W;
    parent = item;
    item->sons.push_back(this);
    depth = item->depth + 1;
    int mark = nodetype == SOFTWARE;
    if(nodetype == SOFTWARE){
        setPos(item->parent->x + item->parent->width - item->parent->rest_size[1],item->parent->y + IMAGE_H * 7/2);
        qDebug()<<"setp";
    }
    else
        setPos(item->x + item->width - item->rest_size[0],item->y + IMAGE_H);


    //调整高度 只有是子系统才会改变高度
    if(nodetype == SUB_SYS)
    {
        if(item->height <= 6 * IMAGE_H)
            for(ClickedItem *tail = item;tail != nullptr;tail = tail->parent)
            {
                tail->setSize(tail->width,tail->height + 2 * IMAGE_H);
            }
    }

    //调整长度
    int temp = wi;
    for(ClickedItem *tail = item;tail != nullptr;tail = tail->parent)
    {
        if(tail->nodetype != SUB_SYS)tail = tail->parent;
        if(tail->rest_size[mark] - temp < IMAGE_W)
        {
            int temp1 = temp - tail->rest_size[mark];
            //qDebug()<<temp1<<tail->rest_size[mark];
            tail->setSize(tail->width +  temp1 + IMAGE_W,tail->height);
            tail->rest_size[mark] -= temp;
            //qDebug()<<tail->rest_size[mark]<<temp;
            temp = temp1 + IMAGE_W;
            //调整此item后的各个结点的位置
            ClickedItem *pre = tail -> parent;
            if(pre != nullptr)
            {
                int pos_mark = pre->sons.size();
                for(unsigned i = 0;i < pre->sons.size();i++)
                {
                    if(pre->sons[i] == tail)pos_mark = i;
                    if(i > pos_mark)
                    {
                        pre->sons[i]->setPos(pre->sons[i]->x + temp - IMAGE_W,pre->sons[i]->y);
                        qDebug()<<temp - IMAGE_W;
                    }
                }
            }
        }
        else
        {
            tail->rest_size[mark] -= temp;
            break;
        }
    }
}

void ClickedItem::setSize(int w, int h)
{
    if(nodetype != SUB_SYS)return;
    rest_size[0] += w - width;
    rest_size[1] += w - width;
    width = w;
    height = h;
    updateTree();
}

void ClickedItem::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
    update();
}

void ClickedItem::updateTree()
{
    qDebug()<<"1";
    for(ClickedItem *tail = this;tail != nullptr;tail = tail->parent)
    {
        this->update();
    }
}

QRectF ClickedItem::boundingRect() const
{
    return QRectF(x - IMAGE_W/3,y - IMAGE_H/4,width,height);
}

void ClickedItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //画方框
    qDebug()<<"paint"<<id;
    if(elements_flag == OP_ITEM)
    {
        switch(nodetype)
        {
            case SUB_SYS : painter->setBrush(QColor(237,237,237,255));break;
            case SWITCH :
            case DEVICE : painter->setBrush(QColor(91,155,213,255));break;
            case SOFTWARE : painter->setBrush(QColor(237,125,49,255));break;
            case MESSAGE : painter->setBrush(QColor(226,239,217,255));break;
        }

        //painter->setBrush(QColor(237,237,237,255));
        if(nodetype != SOFTWARE && nodetype != MESSAGE)
            painter->drawRect(x,y,width - IMAGE_W*1/3,height - IMAGE_H*1/3);
        else
        {
            painter->drawEllipse(x,y,width - IMAGE_W*1/3,height - IMAGE_H*1/3);
            painter->drawLine(x + (width - IMAGE_W*1/3)/2,y,parent->x + (parent->width - IMAGE_W*1/3)/2,parent->y + parent->height - IMAGE_H*1/3);
        }

        //原始状态
        painter->setBrush(Qt::NoBrush);
        painter->drawPixmap(x + IMAGE_W/3,y + IMAGE_H/10,IMAGE_W,IMAGE_H,*item_pixmap);

        //elements frame
        pen.setWidth(1);
        painter->setPen(pen);

    }
    else if(elements_flag == OP_ITEM_SELECT)
    {
        //放大状态
        pen.setWidth(2);
        painter->setPen(pen);
        painter->setBrush(Qt::white);
        painter->drawRect(x - IMAGE_W/3,y - IMAGE_H/4,width,height);//画矩形
        painter->drawPixmap(x - IMAGE_W/3,y - IMAGE_H/4,IMAGE_W*2,IMAGE_H*2,*item_pixmap);
    }
    else
    {
        //elements frame状态
        //painter->drawPixmap(x,y,IMAGE_W,IMAGE_H,*item_pixmap);
        painter->drawPixmap(x + IMAGE_W/3,y + IMAGE_H/10,IMAGE_W,IMAGE_H,*item_pixmap);
    }

    //画名称
    painter->setPen(Qt::black);
    if(item_name.length() > 4)
        painter->drawText(x + IMAGE_H*1/3,y + IMAGE_H*7/5,item_name);
    else
        painter->drawText(x + IMAGE_H*5/6 - item_name.length()*IMAGE_W/8,y + IMAGE_H*7/5,item_name);
    //qDebug()<<item_name;
}


void ClickedItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setSelected(true);
}

void ClickedItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(elements_flag||!(event->buttons() & Qt::LeftButton))return;
    QDrag *drag = new QDrag(this->scene());
    QMimeData *mimeData =new QMimeData();
    mimeData->setText(QString::number(nodetype) + " " + item_name);
    drag->setMimeData(mimeData);//MimeData可以用来传递数据
    drag->setPixmap(*item_pixmap);//设置跟随的图片
    drag->setHotSpot(QPoint(IMAGE_H/10,IMAGE_W/10));//设置图片中心点
    Qt::DropAction dropAction = drag->exec();
    updateTree();
}

void ClickedItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    QStringList list = event->mimeData()->text().split(' ');
    if(!elements_flag)return;
    if(list.at(0).toInt() == SUB_SYS && nodetype != SUB_SYS)return;
    if(list.at(0).toInt() == DEVICE && nodetype != SUB_SYS)return;
    if(list.at(0).toInt() == SWITCH && nodetype != SUB_SYS)return;
    if(list.at(0).toInt() == SOFTWARE && nodetype != DEVICE)return;
    if(list.at(0).toInt() != SUB_SYS && depth == 0)return;
    if(list.at(0).toInt() == MESSAGE)return;


    pen.setColor(Qt::red);
    pen.setStyle(Qt::DashLine);
    elements_flag = 2;
    event->accept();
    updateTree();
}

void ClickedItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(!elements_flag)return;
    pen.setColor(Qt::black);
    if(nodetype != SUB_SYS)
        pen.setStyle(Qt::SolidLine);
    elements_flag = 1;
    updateTree();

}
/*
void ClickedItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}
*/
void ClickedItem::dropEvent(QGraphicsSceneDragDropEvent  *event)
{
    QStringList list = event->mimeData()->text().split(' ');
    if(!elements_flag)return;
    if(list.at(0).toInt() == SUB_SYS && nodetype != SUB_SYS)return;
    if(list.at(0).toInt() == DEVICE && nodetype != SUB_SYS)return;
    if(list.at(0).toInt() == SWITCH && nodetype != SUB_SYS)return;
    if(list.at(0).toInt() == SOFTWARE && nodetype != DEVICE)return;
    if(list.at(0).toInt() != SUB_SYS && depth == 0)return;
    if(list.at(0).toInt() == MESSAGE)return;

    if(!elements_flag)return;
    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    elements_flag = 1;
    event->accept();

    addSon(event->mimeData());
    updateTree();
}


void ClickedItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
       qDebug()<<"leftbutton";
       //qDebug()<<item_name<<id;
       //qDebug()<<nodeptr;
        MyDockWidget::get_instance()->set_data(this);
        MyDockWidget::get_instance()->show_data();
    }
    if(event->button() == Qt::RightButton)
    {
        if(elements_flag == ELEMENT_ITEM&&(nodetype == SUB_SYS||nodetype == DEVICE\
                                           || nodetype == SWITCH)){
            return;
        }
        qDebug()<<"rightbutton";
//        MyDockWidget::get_instance()->del_layout();
//        MyDockWidget::get_instance()->hide();
        temp_act[0] = rightClickedMenu->addAction(0,QString("查看"));
        temp_act[1] = rightClickedMenu->addAction(1,QString("删除"));
        connect(temp_act[0],SIGNAL(triggered(bool)),this,SLOT(show_item()));
        connect(temp_act[1],SIGNAL(triggered(bool)),this,SLOT(del_item()));
        rightClickedMenu->ShowMenu();
    }
    //updateTree();
}

void ClickedItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button() == Qt::RightButton)
    {
        setSelected(true);
        qDebug()<<"doublerightbutton";
    }
    else
    {
        setSelected(true);
        qDebug()<<"doubleleftbutton";
    }
    updateTree();
}

void ClickedItem::show_item(){
    MyDockWidget::get_instance()->set_data(this);
    MyDockWidget::get_instance()->show_data();
}

void ClickedItem::del_item(){
    //this->hide();
    switch(nodetype)
    {
        case SUB_SYS:
        {
            if(!root){
                qDebug()<<"SUB_SYS";
                del_subsys();
            }
            break;
        }
        case DEVICE:
        {
            qDebug()<<"DEVICE";
            del_dev_item();
            break;
        }
        case SWITCH:
        {
            qDebug()<<"SWITCH";
            del_dev_item();
            break;
        }
        case SOFTWARE:
        {
            if(elements_flag){
                qDebug()<<"SOFTWARE ITEM";
                del_software_item();
            } else {
                qDebug()<<"SOFTWARE ELE";
                del_software_ele();
            }
            break;
        }
        case MESSAGE:
        {
            if(elements_flag){
                qDebug()<<"MESSAGE ITEM";
            } else {
                qDebug()<<"MESSAGE ELE";
                del_message_ele();
            }
            break;
        }
        default:break;
    }
    MyDockWidget::get_instance()->hide();
    update_signal->send_update_signal();
}

void ClickedItem::del_software_item(){
    ClickedItem *grandParent = this->parent->parent;
    ClickedItem *parent = this->parent;
    int mark = 0;
    int temp = 2 * IMAGE_W;

    for(int i = 0;i<parent->sons.size();i++){
        if(parent->sons[i]->id = this->id){
            parent->sons.erase(parent->sons.begin()+i);
            break;
        }
    }
    for(int i = 0;i<grandParent->sons.size();++i){
        for(int j = 0;j<grandParent->sons[i]->sons.size();j++){
            if(grandParent->sons[i]->sons[j]->id > this->id){
                grandParent->sons[i]->sons[j]->setPos(grandParent->sons[i]->sons[j]->x\
                                       - temp,grandParent->sons[i]->sons[j]->y);
            }
        }
    }
    delete this;
}

void ClickedItem::del_dev_item(){
    if(this->sons.size() != 0){
        return;
    }
    int mark = 0;
    int temp =  IMAGE_W;
    ClickedItem *parent = this->parent;
    int size = parent->sons.size();
    for(int i = 0;i<size;i++){
        if(parent->sons[i]->id = this->id){
            parent->sons.erase(parent->sons.begin()+i);
            break;
        }
    }
    for(int i = 0;i<size;i++){
        if(parent->sons[i]->id > this->id){
            parent->sons[i]->setPos(parent->sons[i]->x - temp,parent->sons[i]->y);
        }
    }
    delete this;
}


void ClickedItem::del_subsys(){
    if(this->sons.size() != 0){
        return;
    }

}

void ClickedItem::del_software_ele(){

}

void ClickedItem::del_message_ele(){

}
