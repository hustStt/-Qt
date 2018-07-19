#include "mydockwidget.h"
#include <QDebug>
#include <vector>
#include "mytreeitem.h"

MyDockWidget * MyDockWidget::_instance = NULL;

MyDockWidget::MyDockWidget() : QDockWidget("元件信息")
{
    item = nullptr;
}

MyDockWidget* MyDockWidget::get_instance()
{
    if(_instance == NULL)
    {
        _instance = new MyDockWidget();
        _instance->Controls_initial();
    }
    return _instance;
}

void MyDockWidget::changeItem()
{
    qDebug()<<"changeitem";
    if(item == nullptr) return;
    switch(item->nodetype)
    {
        case SUB_SYS:
        {
            if(!item->elements_flag) return;
            if(item->root){
                if(!item->elements_flag) return;
                change_sys((XSDSystem *)item->nodeptr);
                break;
            } else {
                change_subsys((XSDSubSystem*)item->nodeptr);
                break;
            }
        }
        case DEVICE:
        {
            if(!item->elements_flag) return;
            qDebug()<<"device";
            change_device((XSDDevice*)item->nodeptr);
            break;
        }
        case SWITCH:
        {
            if(!item->elements_flag) return;
            change_switch((XSDSwitch*)item->nodeptr);
            break;
        }
        case SOFTWARE:
        {
            if(item->elements_flag){
                change_connection((SubSoftwareConnection*)item->nodeptr);
            } else{
                change_software((XSDSoftWare*)item->nodeptr);
            }
            break;
        }
        case MESSAGE:
        {
            change_message((Msg*)item->nodeptr);
            break;
        }
        default:break;
    }

}

void MyDockWidget::set_data(ClickedItem *item){
    this->item = item;
}

void MyDockWidget::show_data(){

    if(item == nullptr) return;
    switch(item->nodetype)
    {
        case SUB_SYS:
        {
            if(!item->elements_flag) return;
            if(item->root){
                if(!item->elements_flag) return;
                qDebug()<<"ROOT_NODE";
                show_sys((XSDSystem *)item->nodeptr);
                break;
            } else {
                qDebug()<<"SUB_SYS";
                show_subsys((XSDSubSystem*)item->nodeptr);
                break;
            }
        }

        case DEVICE:
        {
            if(!item->elements_flag) return;
            qDebug()<<"DEVICE";
            show_device((XSDDevice*)item->nodeptr);
            break;
        }
        case SWITCH:
        {
            if(!item->elements_flag) return;
            qDebug()<<"SWITCH";
            show_switch((XSDSwitch*)item->nodeptr);
            break;
        }
        case SOFTWARE:
        {
            qDebug()<<"SOFTWARE";
            if(item->elements_flag){
                show_connection((SubSoftwareConnection*)item->nodeptr);
            } else{
                show_software((XSDSoftWare*)item->nodeptr);
            }
            break;
        }
        case MESSAGE:
        {
            qDebug()<<"MESSAGE";
            show_message((Msg*)item->nodeptr);
            break;
        }
        default:break;
    }

    this->show();
    myframe->setFrameStyle(QFrame::Box);
    myframe->setLayout(op_stack_layout);

    setWidget(myframe);
    //setStyleSheet("background-color:white;");
}

void MyDockWidget::clear_data(){
    for(int i = 0;i < MAX_INPUT_NUM;i++){
        input_label[i]->clear();
        input_lineedit[i]->clear();
        input_box[i]->clear();
        add_input_lineedit[i]->clear();
    }
}

void MyDockWidget::Controls_initial(){
    op_stack_layout = new QVBoxLayout();
    myframe = new QFrame();
    /*
     * for(int i = 0;i < MAX_INPUT_NUM;i++){
        input_label[i] = new QLabel();
        input_lineedit[i] = new QLineEdit();
        input_box[i] = new QComboBox();
        add_input_lineedit[i] = new QLineEdit();
    }
    save_button = new QPushButton("确定修改");
    */
}


void MyDockWidget::del_layout(){
    QLayoutItem *child;
    while((child = op_stack_layout->takeAt(0)) != 0)
    {
        //qDebug()<<"test1";
        if(child->widget() != NULL)
        {
            //qDebug()<<"test2";
            QWidget* p_widget= child->widget();
            op_stack_layout->removeWidget(p_widget);
            delete p_widget;
        }
        //qDebug()<<"test3";
        op_stack_layout->removeItem(child);
        delete child;
    }
    //qDebug()<<"test4";
    //clear_data();
}

void MyDockWidget::show_sys(XSDSystem *sys){
    del_layout();
    for(int i = 0;i < 2;++i)
    {
        op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_XSDSystem[i] + ":"));
        op_stack_layout->addWidget(input_lineedit[i] = new QLineEdit(*(&sys->name +i)));
    }
    op_stack_layout->addWidget(input_label[2] = new QLabel());
    op_stack_layout->addWidget(save_button = new QPushButton("确定修改"));
    connect(save_button,SIGNAL(clicked(bool)),this,SLOT(changeItem()));
}

void MyDockWidget::show_subsys(XSDSubSystem *subsys){
    del_layout();
    for(int i = 0;i < 3;++i)
    {
        op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_XSDSubSystem[i] + ":"));
        op_stack_layout->addWidget(input_lineedit[i] = new QLineEdit(*(&subsys->id +i)));
    }
    op_stack_layout->addWidget(input_label[3] = new QLabel());
    op_stack_layout->addWidget(save_button = new QPushButton("确定修改"));
    connect(save_button,SIGNAL(clicked(bool)),this,SLOT(changeItem()));
}

void MyDockWidget::show_device(XSDDevice *dev){
    del_layout();
    count = 0;
    int i;
    for(i = 0;i < 3;++i)
    {
        op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_XSDDevice[i] + ":"));
        if(i != 2)
            op_stack_layout->addWidget(input_lineedit[i] = new QLineEdit(*(&dev->id + i)));
        else{
            op_stack_layout->addWidget(input_box[i] = new QComboBox());
            for(int j = 0;j < 5;++j){
                input_box[i]->addItem(_XSDNodeType[j]);
                if(_XSDNodeType[j] == *(&dev->id + i)){
                    input_box[i]->setCurrentIndex(j);
                }
            }
        }
    }
    if(item->changeState){
        input_lineedit[0]->setFocusPolicy(Qt::NoFocus);
    }
    op_stack_layout->addWidget(input_label[3] = new QLabel());
    next_button = new QPushButton("查看" + CHS_XSDDevice[3]);
    op_stack_layout->addWidget(next_button);
    connect(next_button,SIGNAL(clicked(bool)),this,SLOT(show_interface()));
    op_stack_layout->addWidget(save_button = new QPushButton("确定修改"));
    connect(save_button,SIGNAL(clicked(bool)),this,SLOT(changeItem()));
}

void MyDockWidget::show_interface(){
    save_button->disconnect();
    del_layout();
    XSDDevice *dev = (XSDDevice *)(item->nodeptr);
    XSDNetworkInterface *Inter;
    if(dev->networkinterface_vec.size()>count){
        Inter = dev->networkinterface_vec[count++];
    }
    for(int i = 0;i < 4;++i)
    {
        op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_XSDNetworkInterface[i] + ":"));
        op_stack_layout->addWidget(add_input_lineedit[i] = new QLineEdit(*(&Inter->MACadress + i)));
    }
    op_stack_layout->addWidget(input_label[4] = new QLabel());
    if(count<dev->networkinterface_vec.size()){
        op_stack_layout->addWidget(next_button = new QPushButton("下个" + CHS_XSDDevice[3]));
        connect(next_button,SIGNAL(clicked(bool)),this,SLOT(show_interface()));
    }
    else{
        swi = false;
        op_stack_layout->addWidget(next_button = new QPushButton("添加" + CHS_XSDDevice[3]));
        connect(next_button,SIGNAL(clicked(bool)),this,SLOT(add_interface()));
    }
    op_stack_layout->addWidget(save_button = new QPushButton("确定修改"));
    connect(save_button,SIGNAL(clicked(bool)),this,SLOT(save_interface()));
}

void MyDockWidget::add_interface(){
    count++;//添加新接口 count加一
    if(swi == false){
        for(int i = 0;i < 4;++i){
            add_input_lineedit[i]->setText("");
        }
        swi = true;
        return;
    }
    save_interface();

    for(int i = 0;i < 4;++i){
        add_input_lineedit[i]->setText("");
    }
}

void MyDockWidget::save_interface(){
    XSDDevice *dev = (XSDDevice *)(item->nodeptr);
    if(count<=dev->networkinterface_vec.size()){
        qDebug()<<"已存在的接口 执行修改";
        XSDNetworkInterface *intf = dev->networkinterface_vec[count-1];
        for(int i = 0;i < 4;++i)
        {
            *(&intf->MACadress + i) = add_input_lineedit[i]->text();
        }
        return;
    }
    qDebug()<<"新添加的接口 执行保存";
    XSDNetworkInterface *interface = new XSDNetworkInterface();
    for(int i = 0;i < 4;++i)
    {
        *(&interface->MACadress + i) = add_input_lineedit[i]->text();
    }
    dev->networkinterface_vec.push_back(interface);
}

QString NumberToQString(std::vector <unsigned int> temp){
    QString s;
    s.clear();
    for(int i = 0;i < temp.size();i++){
        //qDebug()<<temp[i];
        s += QString::number(temp[i],10);
        //qDebug()<<s;
        if(i != temp.size()-1){
            s += QString(" ");
        }
    }
    return s;
}

void MyDockWidget::show_switch(XSDSwitch *swi){
    del_layout();
    for(int i = 0;i < 4;++i)
    {
        op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_XSDSwitch[i] + ":"));
        op_stack_layout->addWidget(input_lineedit[i] = new QLineEdit());
    }
    for(int i = 0;i < 2;i++)
    {
        input_lineedit[i]->setText(*(&swi->id +i));
    }
    input_lineedit[2]->setText(swi->ip_vec.back());
    input_lineedit[3]->setText(NumberToQString(swi->portnumber_vec));
    input_label[3]->setText(CHS_XSDDevice[3] + " (如有多个请用空格隔开):");
    op_stack_layout->addWidget(input_label[4] = new QLabel());
    op_stack_layout->addWidget(save_button = new QPushButton("确定修改"));
    connect(save_button,SIGNAL(clicked(bool)),this,SLOT(changeItem()));
}

void MyDockWidget::show_software(XSDSoftWare *software){
    del_layout();
    for(int i = 0;i < 5;++i)
    {
        op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_XSDSoftWare[i] + ":"));
        if(i != 2)
            op_stack_layout->addWidget(input_lineedit[i] = new QLineEdit(*(&software->softid + i)));
        else
        {
            op_stack_layout->addWidget(input_box[i] = new QComboBox());
            for(int j = 0;j < 2;++j){
                input_box[i]->addItem(_XSDSoftwareType[j]);
                if(_XSDSoftwareType[j] == *(&software->softid + i)){
                    input_box[i]->setCurrentIndex(j);
                }
            }
        }
    }
    if(item->changeState){
        input_lineedit[0]->setFocusPolicy(Qt::NoFocus);
    }
    op_stack_layout->addWidget(input_label[5] = new QLabel());
    op_stack_layout->addWidget(save_button = new QPushButton("确定修改"));
    connect(save_button,SIGNAL(clicked(bool)),this,SLOT(changeItem()));
}

void MyDockWidget::show_message(Msg *msg){
    del_layout();
    count = 0;
    for(int i = 0;i < 2;++i)
    {
        op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_Msg[i] + ":"));
        op_stack_layout->addWidget(input_lineedit[i] = new QLineEdit(*(&msg->name + i)));
    }
    for(int i = 2;i < 4;++i)
    {
        op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_Msg[i] + ":"));
        op_stack_layout->addWidget(input_box[i] = new QComboBox());
    }
    for(int i = 0;i < 3;++i){
        input_box[2]->addItem(_XSDMsgType[i]);
        if(_XSDMsgType[i] == msg->msgtype){
            input_box[2]->setCurrentIndex(i);
        }
    }
    for(int i = 0;i < 2;++i){
        input_box[3]->addItem(_XSDTransportType[i]);
        if(_XSDTransportType[i] == msg->transporttype){
            input_box[3]->setCurrentIndex(i);
        }
    }
    if(item->changeState){
        input_lineedit[1]->setFocusPolicy(Qt::NoFocus);
    }
    op_stack_layout->addWidget(input_label[4] = new QLabel());
    next_button = new QPushButton("查看" + CHS_Msg[4]);
    op_stack_layout->addWidget(next_button);
    connect(next_button,SIGNAL(clicked(bool)),this,SLOT(show_struct_info()));
    op_stack_layout->addWidget(save_button = new QPushButton("确定修改"));
    connect(save_button,SIGNAL(clicked(bool)),this,SLOT(changeItem()));
}

void MyDockWidget::show_struct_info(){
    del_layout();
    Msg *msg = (Msg *)(item->nodeptr);
    MsgElement *ele;
    if(msg->msgstruct&&count<msg->msgstruct->element_vec.size())
        ele = msg->msgstruct->element_vec[count++];
    for(int i = 0;i < 9;++i)
    {
        if(i == 1 || i == 2)
            op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_MsgElement[i] + "(*):"));
        else
            op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_MsgElement[i] + ":"));
        if(i == 4){
            op_stack_layout->addWidget(add_input_lineedit[i] = new QLineEdit(QString::number(ele->maxlength)));
        } else {
            op_stack_layout->addWidget(add_input_lineedit[i] = new QLineEdit(*(&ele->name + i)));
        }
    }
    qDebug()<<"count"<<count;
    if(count<msg->msgstruct->element_vec.size()){
        op_stack_layout->addWidget(next_button = new QPushButton("下个" + CHS_Msg[4]));
        connect(next_button,SIGNAL(clicked(bool)),this,SLOT(show_struct_info()));
    }
    else{
        swi = false;
        op_stack_layout->addWidget(next_button = new QPushButton("添加" + CHS_Msg[4]));
        connect(next_button,SIGNAL(clicked(bool)),this,SLOT(add_struct_info()));
    }
    op_stack_layout->addWidget(save_button = new QPushButton("确定修改"));
    connect(save_button,SIGNAL(clicked(bool)),this,SLOT(save_struct_info()));
}

void MyDockWidget::add_struct_info(){
    count++;
    if(swi == false){
        for(int i = 0;i < 9;++i){
            add_input_lineedit[i]->setText("");
        }
        swi = true;
        return;
    }
    save_struct_info();
    for(int i = 0;i < 9;++i){
        add_input_lineedit[i]->setText("");
    }
}

void MyDockWidget::save_struct_info(){
    Msg *msg = (Msg *)(item->nodeptr);
    if(count<=msg->msgstruct->element_vec.size()){
        qDebug()<<"已存在的结构 执行修改";
        MsgElement *ele = msg->msgstruct->element_vec[count-1];
        for(int i = 0;i < 4;++i)
        {
            *(&ele->name + i) = add_input_lineedit[i]->text();
        }
        ele->maxlength = add_input_lineedit[4]->text().toInt();
        for(int i = 5;i < 9;++i)
        {
            *(&ele->name + i) = add_input_lineedit[i]->text();
        }
        return;
    }
    MsgElement *ele = new MsgElement();
    for(int i = 0;i < 4;++i)
    {
        *(&ele->name + i) = add_input_lineedit[i]->text();
    }
    ele->maxlength = add_input_lineedit[4]->text().toInt();
    for(int i = 5;i < 9;++i)
    {
        *(&ele->name + i) = add_input_lineedit[i]->text();
    }
    msg->msgstruct->element_vec.push_back(ele);
}

void MyDockWidget::show_connection(SubSoftwareConnection *conn){
    //qDebug()<<conn->device_id<<conn->software_id;
    del_layout();
    for(int i = 0;i < 3;++i)
    {
        op_stack_layout->addWidget(input_label[i] = new QLabel(CHS_SubSoftwareConnection[i] + ":"));
        op_stack_layout->addWidget(input_lineedit[i] = new QLineEdit(*(&conn->device_id+i)));
    }

    input_lineedit[0]->setFocusPolicy(Qt::NoFocus);
    input_lineedit[1]->setFocusPolicy(Qt::NoFocus);

    op_stack_layout->addWidget(input_label[3] = new QLabel());

    op_stack_layout->addWidget(save_button = new QPushButton("确定修改"));
    connect(save_button,SIGNAL(clicked(bool)),this,SLOT(changeItem()));
}

void MyDockWidget::show_dds(){

}

void MyDockWidget::change_sys(XSDSystem *sys){
    for(int i = 0;i < 2;i++)
    {
        *(&sys->name +i) = input_lineedit[i]->text();
    }
    qDebug()<<"xx";
    item->item_name = sys->name;
    item->update();
}

void MyDockWidget::change_subsys(XSDSubSystem *subsys){
    for(int i = 0;i < 3;i++)
    {
        *(&subsys->id +i) = input_lineedit[i]->text();
    }
    item->item_name = subsys->name;
    item->update();

    MyTreeItem *temp = (MyTreeItem *)(item->item_node);
    temp->setText(0,subsys->name);
    //qDebug()<<temp->text(0);
}

void MyDockWidget::change_device(XSDDevice *dev){
    for(int i = 0;i < 3;++i)
    {
        if(i == 2)
        {
            *(&dev->id + i) = input_box[i]->currentText();
        }
        else
        {
            *(&dev->id + i) = input_lineedit[i]->text();
        }
    }

    item->item_name = dev->name;
    item->update();

    MyTreeItem *temp = (MyTreeItem *)(item->item_node);
    temp->setText(0,dev->name);
}

void MyDockWidget::change_switch(XSDSwitch *swi){
    for(int i = 0;i < 2;i++)
    {
        *(&swi->id +i) = input_lineedit[i]->text();
    }
    swi->ip_vec.clear();
    swi->portnumber_vec.clear();
    //ip
    swi->ip_vec.push_back(input_lineedit[2]->text());
    //port
    QStringList list = input_lineedit[3]->text().split(' ');
    for(int i = 0;i < list.size();++i)
    {
        swi->portnumber_vec.push_back(list.value(i).toInt());
    }
    item->item_name = swi->name;
    item->update();

    MyTreeItem *temp = (MyTreeItem *)(item->item_node);
    temp->setText(0,swi->name);
}

void MyDockWidget::change_software(XSDSoftWare *software){
    changeName sname;
    sname.oldName = software->name;
    for(int i = 0;i < 5;i++)
    {
        if(i != 2)
        {
            *(&software->softid + i) = input_lineedit[i]->text();
        }
        else
        {
            *(&software->softid + i) = input_box[i]->currentText();
        }
    }
    sname.newName = software->name;
    item->item_name = software->name;
    item->update();

    sendSoftwareItem(sname);

    MyTreeItem *temp = (MyTreeItem *)(item->item_node);
    temp->setText(0,software->name);
}

void MyDockWidget::change_message(Msg *msg){
    changeName mname;
    mname.oldName = msg->name;
    for(int i = 0;i <  4;++i)
    {
        if(i < 2)
        {
            *(&msg->name + i) = input_lineedit[i]->text();
        }
        else
        {
            *(&msg->name + i) = input_box[i]->currentText();
        }
    }
    mname.newName = msg->name;
    item->item_name = msg->name;
    item->update();

    sendMessageItem(mname);

    MyTreeItem *temp = (MyTreeItem *)(item->item_node);
    temp->setText(0,msg->name);
}

void MyDockWidget::change_connection(SubSoftwareConnection *conn){
    for(int i = 0;i < 3;++i)
    {
        *(&conn->device_id + i) = input_lineedit[i]->text();
    }

}

void MyDockWidget::change_dds(){

}
