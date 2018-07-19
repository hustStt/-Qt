#include "mainwindow.h"
#include "ui_mainwindow.h"


extern int id_count;
extern MainWindow *w;

int element_pos = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(1344,768);
    setWindowTitle(tr("系统接口数据辅助设计"));
    //xmlsystem = new XSDSystem();
    rightClickedMenu = new RightClickedMenu();
    elements_item_vec.clear();
    op_item_vec.clear();
    //connection_pair_vec.clear();

    //set menubar
    set_menubar();

    //set toolbar
    set_toolbar();

    //set main frame
    main_frame = new QFrame();
    setCentralWidget(main_frame);
    main_layout = new QHBoxLayout(main_frame);

    op_frame = new QFrame();
    tree_frame = new QFrame();
    elements_frame = new QFrame();
    op_frame->setFrameStyle(QFrame::Box);
    tree_frame->setFrameStyle(QFrame::Box);
    elements_frame->setFrameStyle(QFrame::Box);
    //ShowItem_frame::get_instance()->setFrameStyle(QFrame::Box);

    main_layout->addWidget(tree_frame,6);
    main_layout->addWidget(elements_frame,2);
    main_layout->addWidget(op_frame,16);
    //main_layout->addWidget(ShowItem_frame::get_instance(),3);


    //set tree frame
    set_tree_frame();

    //set element frame
    set_elements_frame();

    //set op frame
    set_op_frame();

    mydockwidget = MyDockWidget::get_instance();
    addDockWidget(Qt::RightDockWidgetArea,mydockwidget);

    connect(mydockwidget,SIGNAL(sendSoftwareItem(changeName)),this,SLOT(changeSoftwareItem(changeName)));
    connect(mydockwidget,SIGNAL(sendMessageItem(changeName)),this,SLOT(changeMessageItem(changeName)));
    mydockwidget->hide();

    //刷新
    connect(update_ui::get_instance(),SIGNAL(send_update_signal()),this,SLOT(update_view()));
    //startTimer(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event){
    qDebug()<<"u";
    op_arch_view->viewport()->update();
}

void MainWindow::update_view(){
    qDebug()<<"this is update_view";
    op_arch_view->viewport()->update();
}

//返回界面3 table_view 的大小
QVBoxLayout *MainWindow::adjust_layout_size(QVBoxLayout *layout,int width,int height)
{
    QFrame *frame_h[3];
    QFrame *frame_w[3];
    QVBoxLayout *layout0 = layout;
    QHBoxLayout *layout1;

    for(int i = 0;i < 3;++i)
    {
        frame_h[i] = new QFrame();
        if(i == 1)
            layout0->addWidget(frame_h[i],height);
        else
            layout0->addWidget(frame_h[i],1);
    }
    layout1 = new QHBoxLayout(frame_h[1]);
    for(int i = 0;i < 3;++i)
    {
        frame_w[i] = new QFrame();
        if(i == 1)
            layout1->addWidget(frame_w[i],width);
        else
            layout1->addWidget(frame_w[i],1);
    }
    frame_w[1]->setFrameStyle(QFrame::Box);
    layout = new QVBoxLayout(frame_w[1]);
   return layout;
}

//设置状态栏
void MainWindow::set_menubar()
{
    menu[0] = new QMenu(tr("开始"));
    menu[1] = new QMenu(tr("帮助"));
    menu_action[0][0] = menu[0]->addAction(tr("新建XML文件"));
    menu_action[0][1] = menu[0]->addAction(tr("打开XML文件"));
    menu[0]->addSeparator();
    menu_action[0][2] = menu[0]->addAction(tr("保存"));
    menu_action[0][3] = menu[0]->addAction(tr("另存为"));
    menu_action[1][0] = menu[1]->addAction(tr("关于"));
    ui->menuBar->addMenu(menu[0]);
    ui->menuBar->addMenu(menu[1]);
    connect(menu_action[0][0],SIGNAL(triggered(bool)),this,SLOT(create_file()));
    connect(menu_action[0][1],SIGNAL(triggered(bool)),this,SLOT(open_file()));
    connect(menu_action[0][2],SIGNAL(triggered(bool)),this,SLOT(save_file()));
    connect(menu_action[0][3],SIGNAL(triggered(bool)),this,SLOT(save_as_file()));
    connect(menu_action[1][0],SIGNAL(triggered(bool)),this,SLOT(about_this()));
}

//设置菜单栏
void MainWindow::set_toolbar()
{
    tool_action[0] = ui->mainToolBar->addAction(QIcon(IMAGE_PATH + "create_file_icon.jpg"),tr("新建XML文件"));
    tool_action[1] = ui->mainToolBar->addAction(QIcon(IMAGE_PATH + "open_file_icon.jpg"),tr("打开XML文件"));
    tool_action[2] = ui->mainToolBar->addAction(QIcon(IMAGE_PATH + "save_icon.jpg"),tr("保存"));
    tool_action[3] = ui->mainToolBar->addAction(QIcon(IMAGE_PATH + "save_as_icon.jpg"),tr("另存为"));
    connect(tool_action[0],SIGNAL(triggered(bool)),this,SLOT(create_file()));
    connect(tool_action[1],SIGNAL(triggered(bool)),this,SLOT(open_file()));
    connect(tool_action[2],SIGNAL(triggered(bool)),this,SLOT(save_file()));
    connect(tool_action[3],SIGNAL(triggered(bool)),this,SLOT(save_as_file()));

}

//设置tree_view
void MainWindow::set_tree_frame()
{
    tree_layout = new QHBoxLayout(tree_frame);
    tree_tab = new QTabWidget();
    tree_layout->addWidget(tree_tab);
    for(int i = 0;i < MAX_TAB_NUM;++i)tree_tab_frame[i] = new QFrame();
    tree_tab->addTab(tree_tab_frame[0],tr("结构"));
    tree_tab->addTab(tree_tab_frame[1],tr("接口关系"));
    tree_tab->addTab(tree_tab_frame[2],tr("基本元素"));
    //set tab position
    tree_tab->setTabPosition(QTabWidget::West);
    tree_tab->tabBar()->setStyle(new CustomTabStyle());
    for(int i = 0;i < MAX_TAB_NUM;++i){
        tree_tab_layout[i] = new QHBoxLayout(tree_tab_frame[i]);
        tree_widget[i] = new QTreeWidget();
        tree_tab_layout[i]->addWidget(tree_widget[i]);
        tree_widget[i]->setColumnCount(1);
        tree_widget[i]->setHeaderLabels({"系统"});
        tree_widget[i]->headerItem()->setIcon(0,QIcon(IMAGE_PATH + "system_icon.jpg"));
        tree_widget[i]->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tree_widget[i],SIGNAL(customContextMenuRequested(const QPoint)),this,SLOT(tree_widget_rightclicked(const QPoint)));
    }

    //tab1
    tree_item[0].push_back(new MyTreeItem(tree_widget[0],QStringList(tr("节点"))));
    tree_item[0][0]->setData(0,5,ROOT_NODE);
    //tab2
    tree_item[1].push_back(new MyTreeItem(tree_widget[1],QStringList(tr("接口关系"))));
    tree_item[1][0]->setData(0,5,DEFAULT_NODE);

    tree_item[1].push_back(new MyTreeItem(tree_item[1][0],QStringList(tr("DDS接口关系"))));
    tree_item[1][1]->setData(0,5,DDS_NODE);
    tree_item[1][1]->setCheckState(0,Qt::Checked);
    //tab3
    tree_item[2].push_back(new MyTreeItem(tree_widget[2],QStringList(tr("基本元素"))));
    tree_item[2][0]->setData(0,5,DEFAULT_NODE);
    tree_item[2][0]->setExpanded(true);
    tree_item[2].push_back(new MyTreeItem(tree_item[2][0],QStringList(tr("硬件"))));
    tree_item[2][1]->setData(0,5,DEFAULT_NODE);
    tree_item[2].push_back(new MyTreeItem(tree_item[2][0],QStringList(tr("软件"))));
    tree_item[2][2]->setData(0,5,SOFTWARE_NODE);
    tree_item[2].push_back(new MyTreeItem(tree_item[2][0],QStringList(tr("消息"))));
    tree_item[2][3]->setData(0,5,MESSAGE);
    tree_item[2].push_back(new MyTreeItem(tree_item[2][1],QStringList(tr("子系统"))));
    tree_item[2][4]->setData(0,5,DEFAULT_NODE);
    tree_item[2][4]->setIcon(0,QIcon(IMAGE_PATH + "system_icon.jpg"));
    tree_item[2].push_back(new MyTreeItem(tree_item[2][1],QStringList(tr("交换机"))));
    tree_item[2][5]->setData(0,5,DEFAULT_NODE);
    tree_item[2][5]->setIcon(0,QIcon(IMAGE_PATH + "switch_icon.jpg"));
    tree_item[2].push_back(new MyTreeItem(tree_item[2][1],QStringList(tr("设备"))));
    tree_item[2][6]->setData(0,5,DEFAULT_NODE);
    tree_item[2][6]->setIcon(0,QIcon(IMAGE_PATH + "device_icon.jpg"));
}

//设置元件栏
void MainWindow::set_elements_frame()
{
    elements_layout = new QHBoxLayout(elements_frame);

    elements_scene = new QGraphicsScene();
    elements_view = new QGraphicsView(elements_scene);
    elements_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    elements_layout->addWidget(elements_view);
    elements_view->show();

    for(int i = 0;i < 3;++i)
    {
        elements_item_vec.push_back(new ClickedItem(elements_view->x() - IMAGE_W/3,\
                                                    elements_view->y() - IMAGE_H*(0 - 2*elements_item_vec.size()),SUB_SYS + i));
        elements_scene->addItem(elements_item_vec[elements_item_vec.size() - 1]);
        elements_item_vec[elements_item_vec.size() - 1]->item_name = _ELEMENT_NAME[i];
    }
    element_pos = elements_view->x() - IMAGE_W/3;
    //自动更新scene大小
    elements_scene->setSceneRect(0,0,elements_view->x() + IMAGE_W,elements_view->y()-IMAGE_H*(0 - 2*elements_item_vec.size()));
    //设置scroll到最顶部
    qDebug()<<elements_view->x()<<elements_view->y();
    elements_view->verticalScrollBar()->setRange(0,90);
    elements_view->verticalScrollBar()->setValue(elements_view->verticalScrollBar()->maximum());
}

//初始化操作界面
void MainWindow::set_op_frame()
{
    //set op stack
    op_stack = new QStackedWidget();
    op_layout = new QHBoxLayout(op_frame);
    op_layout->addWidget(op_stack);
    for(int i = 0;i < MAX_TAB_NUM;++i)
    {
        op_stack_frame[i] = new QFrame();
        op_stack_layout[i] = new QVBoxLayout(op_stack_frame[i]);
        op_stack->addWidget(op_stack_frame[i]);
    }
    connect(tree_tab,SIGNAL(currentChanged(int)),op_stack,SLOT(setCurrentIndex(int)));
    tree_tab->setCurrentIndex(0);

    //op arch
    op_arch_scene = new QGraphicsScene();
    //op_scene->setSceneRect(0,0,op_layout->sizeHint().width(),op_layout->sizeHint().height());
    op_arch_view = new QGraphicsView(op_arch_scene);
    op_arch_view->show();
    op_stack_layout[0]->addWidget(op_arch_view);

    //op interface connect
    interface_stack =  new QStackedWidget();
    op_stack_layout[1]->addWidget(interface_stack);
    for(int i = 0;i < MAX_TAB_NUM;++i)
    {
        interface_stack_frame[i] = new QFrame();
        interface_stack_layout[i] = new QVBoxLayout(interface_stack_frame[i]);
        interface_stack->addWidget(interface_stack_frame[i]);
    }
    test_textedit = new QTextEdit("test");
    interface_stack_layout[0]->addWidget(test_textedit);
    op_hardware_interface_scene = new QGraphicsScene();
    op_hardware_interface_view = new QGraphicsView(op_hardware_interface_scene);
    op_hardware_interface_view->show();
    interface_stack_layout[1]->addWidget(op_hardware_interface_view);
    interface_stack->setCurrentIndex(0);

    analysisxml =  new XmlAnalysis();
    connect(analysisxml,SIGNAL(sendString(QString)),this,SLOT(showString(QString)));
    createxml = new XmlCreate();

    //op elements
    op_stack_layout[2] = adjust_layout_size(op_stack_layout[2],2,7);
    for(int i = 0;i < 3;++i)
    {
        op_elements_table[i] = new QTableWidget(0,2);
        op_elements_table[i]->setColumnCount(2);
        op_elements_table[i]->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        //op_elements_table[i]->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightgray}"); //设置表头背景色
        op_elements_table[i]->setShowGrid(true);
        op_elements_table[i]->setHorizontalHeaderLabels(QStringList(_TABLE_HEAD[i]));
        op_stack_layout[2]->addWidget(op_elements_table[i]);
    }
    /*
    for(int i = 1;i < 3;++i)
    {
        op_elements_table[i]->setCellWidget(0,0,new QPushButton("添加"));
    }*/
    //op input
    op_stack_layout[3] = adjust_layout_size(op_stack_layout[3],2,2);

}

void MainWindow::set_software_element(XSDSystem *sys)
{
    for(int i = 0;sys->softwareset && i < sys->softwareset->software_vec.size();++i)
    {
        elements_item_vec.push_back(new ClickedItem(element_pos,\
                                                    elements_view->y() - IMAGE_H*(0 - 2*elements_item_vec.size()),SOFTWARE));
        elements_scene->addItem(elements_item_vec[elements_item_vec.size() - 1]);
        elements_item_vec[elements_item_vec.size() - 1]->nodeptr = sys->softwareset->software_vec[i];
        elements_item_vec[elements_item_vec.size() - 1]->item_name = sys->softwareset->software_vec[i]->name;
        //table
        QTableWidgetItem *item = new QTableWidgetItem(sys->softwareset->software_vec[i]->name);
        QTableWidgetItem *item_id = new QTableWidgetItem(sys->softwareset->software_vec[i]->softid);
        op_elements_table[1]->setRowCount(op_elements_table[1]->rowCount() + 1);
        //(op_elements_table[2]->itemAt(op_elements_table[2]->rowCount() - 1,0));
        op_elements_table[1]->setItem(op_elements_table[1]->rowCount() - 1,0,item);
        op_elements_table[1]->setItem(op_elements_table[1]->rowCount() - 1,1,item_id);
        //tree
        tree_item[2].push_back(new MyTreeItem(tree_item[2][2],QStringList(sys->softwareset->software_vec[i]->name)));
        elements_item_vec[elements_item_vec.size() - 1]->item_node = tree_item[2][tree_item[2].size() - 1];
        tree_item[2][tree_item[2].size() - 1]->item_ptr = elements_item_vec[elements_item_vec.size() - 1];
        tree_item[2][tree_item[2].size() - 1]->setData(0,5,SOFTWARE);
        tree_item[2][tree_item[2].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "software_icon.jpg"));
    }
}

void MainWindow::set_message_element(XSDSystem *sys)
{
    for(int i = 0;sys->msgset && i < sys->msgset->msg_vec.size();++i)
    {
        elements_item_vec.push_back(new ClickedItem(element_pos,\
                                                    elements_view->y() - IMAGE_H*(0 - 2*elements_item_vec.size()),MESSAGE));
        elements_scene->addItem(elements_item_vec[elements_item_vec.size() - 1]);
        elements_item_vec[elements_item_vec.size() - 1]->nodeptr = sys->msgset->msg_vec[i];
        elements_item_vec[elements_item_vec.size() - 1]->item_name = sys->msgset->msg_vec[i]->name;
        //table
        QTableWidgetItem *item = new QTableWidgetItem(sys->msgset->msg_vec[i]->name);
        QTableWidgetItem *item_id = new QTableWidgetItem(sys->msgset->msg_vec[i]->id);
        op_elements_table[2]->setRowCount(op_elements_table[2]->rowCount() + 1);
        //(op_elements_table[2]->itemAt(op_elements_table[2]->rowCount() - 1,0));
        op_elements_table[2]->setItem(op_elements_table[2]->rowCount() - 1,0,item);
        op_elements_table[2]->setItem(op_elements_table[2]->rowCount() - 1,1,item_id);
        //tree
        tree_item[2].push_back(new MyTreeItem(tree_item[2][3],QStringList(sys->msgset->msg_vec[i]->name)));
        elements_item_vec[elements_item_vec.size() - 1]->item_node = tree_item[2][tree_item[2].size() - 1];
        tree_item[2][tree_item[2].size() - 1]->item_ptr = elements_item_vec[elements_item_vec.size() - 1];
        tree_item[2][tree_item[2].size() - 1]->setData(0,5,MESSAGE);
        tree_item[2][tree_item[2].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "message_icon.jpg"));
    }

}


void MainWindow::set_message_item(XSDSystem *sys)
{

}

bool compare(DDSInterfaceRelationNode *a,DDSInterfaceRelationNode *b)
{
    return a->msg->id<b->msg->id; //升序排列，如果改为return a>b，则为降序
}

void MainWindow::set_message_connection_item(XSDSystem *sys)
{

    if(sys->interfacerelationset == nullptr ||sys->interfacerelationset->interfacerelation == nullptr\
            ||sys->interfacerelationset->interfacerelation->DDSinterfacerelation_vec.size() == 0)return;
    DDSInterfaceRelation *relation = new DDSInterfaceRelation();


    for(int i = 0;i < sys->interfacerelationset->interfacerelation->DDSinterfacerelation_vec.size();++i)
    {
        relation = sys->interfacerelationset->interfacerelation->DDSinterfacerelation_vec[i];
        DDSInterfaceRelationNode *node = new DDSInterfaceRelationNode();
        for(int j = 0;j < sys->msgset->msg_vec.size();++j)
        {
            if(relation->message == sys->msgset->msg_vec[j]->id)
            {
                node->msg = sys->msgset->msg_vec[j];
                break;
            }
        }

        for(int j = 0;j < relation->publishmember_vec.size();++j)
        {
            for(int k = 0;k < sys->softwareset->software_vec.size();++k)
            {
                if(relation->publishmember_vec[j]->softwareid == sys->softwareset->software_vec[k]->softid)
                {
                    node->publish_vec.push_back(sys->softwareset->software_vec[k]);
                }
            }
        }
        for(int j = 0;j < relation->subscribemember_vec.size();++j)
        {
            for(int k = 0;k < sys->softwareset->software_vec.size();++k)
            {
                if(relation->subscribemember_vec[j]->softwareid == sys->softwareset->software_vec[k]->softid)
                {
                    node->subscribe_vec.push_back(sys->softwareset->software_vec[k]);
                }
            }
        }
        DDSInterfacerelation_vec.push_back(node);
    }

    //tree
    sort(DDSInterfacerelation_vec.begin(),DDSInterfacerelation_vec.end(),compare);
    MyTreeItem *mark_item = nullptr;
    MyTreeItem *pub = nullptr;
    MyTreeItem *sub = nullptr;

    for(int i = 0;i < DDSInterfacerelation_vec.size();++i)
    {

        if(mark_item == nullptr || DDSInterfacerelation_vec[i]->msg->id != mark_item->text(0))
        {
            tree_item[1].push_back(new MyTreeItem(tree_item[1][1],QStringList(DDSInterfacerelation_vec[i]->msg->id)));
            mark_item = tree_item[1][tree_item[1].size() - 1];
            tree_item[1][tree_item[1].size() - 1]->setData(0,5,MESSAGE);
            tree_item[1][tree_item[1].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "message_icon.jpg"));
            tree_item[1].push_back(new MyTreeItem(mark_item,QStringList("发布成员")));
            pub = tree_item[1][tree_item[1].size() - 1];
            pub->setData(0,5,DEFAULT_NODE);

            tree_item[1].push_back(new MyTreeItem(mark_item,QStringList("接收成员")));
            sub = tree_item[1][tree_item[1].size() - 1];
            sub->setData(0,5,DEFAULT_NODE);
        }

        MyTreeItem *mark_item1 = nullptr;
        for(int j = 0;j < DDSInterfacerelation_vec[i]->publish_vec.size();++j)
        {
            if(mark_item1 == nullptr || DDSInterfacerelation_vec[i]->publish_vec[j]->name != mark_item->text(0))
            {
                tree_item[1].push_back(new MyTreeItem(pub,\
                                                      QStringList(DDSInterfacerelation_vec[i]->publish_vec[j]->name)));
                mark_item1 = tree_item[1][tree_item[1].size() - 1];
                tree_item[1][tree_item[1].size() - 1]->setData(0,5,SOFTWARE);
                tree_item[1][tree_item[1].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "software_icon.jpg"));
            }
        }
        for(int j = 0;j < DDSInterfacerelation_vec[i]->subscribe_vec.size();++j)
        {
            if(mark_item1 == nullptr || DDSInterfacerelation_vec[i]->subscribe_vec[j]->name != mark_item->text(0))
            {
                tree_item[1].push_back(new MyTreeItem(sub,\
                                                      QStringList(DDSInterfacerelation_vec[i]->subscribe_vec[j]->name)));
                mark_item1 = tree_item[1][tree_item[1].size() - 1];
                tree_item[1][tree_item[1].size() - 1]->setData(0,5,SOFTWARE);
                tree_item[1][tree_item[1].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "software_icon.jpg"));
            }
        }

    }
}

void* MainWindow::find_hareware(XSDSubSystem *sys, QString id, int type)
{
    if(type == SWITCH)
    {
        for(int i = 0;i < sys->switch_vec.size();++i)
            if(sys->switch_vec[i]->id == id)return (void *)sys->switch_vec[i];
        for(int i = 0;i < sys->subsystem_vec.size();++i)
            find_hareware(sys->subsystem_vec[i],id,type);
    }
    else
    {
        for(int i = 0;i < sys->device_vec.size();++i)
            if(sys->device_vec[i]->id == id)return (void *)sys->device_vec[i];
        for(int i = 0;i < sys->subsystem_vec.size();++i)
            find_hareware(sys->subsystem_vec[i],id,type);
    }
    return 0;
}

void MainWindow::set_element_info(XSDSystem *sys)
{
    op_elements_table[0]->setRowCount(3);
    //硬件
    for(int i = 0 ;i < 3;++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(_ELEMENT_NAME[i]);
        op_elements_table[0]->setItem(i,0,item);
    }
    //软件 在set_soft_element中
    //消息 类似上条
}

void MainWindow::set_sys_item(XSDSystem *sys)
{
    ClickedItem *system = new ClickedItem(-350,-200,SUB_SYS,OP_ITEM);
    system->root = 1;
    op_item_vec.push_back(system);
    if(sys)
    {
        op_item_vec[op_item_vec.size() - 1]->item_name = sys->name;
        op_item_vec[op_item_vec.size() - 1]->nodeptr = sys;
        connect(op_item_vec[op_item_vec.size() - 1],SIGNAL(pushBackItem(ClickedItem *)),this,SLOT(getItem(ClickedItem *)));
        op_arch_scene->addItem(op_item_vec[op_item_vec.size() - 1]);

        for(unsigned i = 0;i < sys->node_vec.size();++i)
        {
            ClickedItem *tail;
            for(int j = 0;j < sys->node_vec[i]->subsystem_vec.size();++j)
            {
                tail = new ClickedItem(0,0,SUB_SYS,OP_ITEM);
                tail->setParent(system);
                set_subsys_item(sys->node_vec[i]->subsystem_vec[j],tail);
            }
        }
        for(int i = 0;sys->interfacerelationset && sys->interfacerelationset->softwareconnection\
            &&i < sys->interfacerelationset->softwareconnection->connection_vec.size();++i)
        {
            SubSoftwareConnection *connection = sys->interfacerelationset->softwareconnection->connection_vec[i];
            unsigned j,k;
            for(j = 0;j < op_item_vec.size();++j)//找设备
            {
                if(op_item_vec[j]->nodetype != DEVICE)continue;
                if(connection->device_id == ((XSDDevice *)op_item_vec[j]->nodeptr)->id)
                    break;
            }
            for(k = 0;k < elements_item_vec.size();++k)//找软件
            {
                if(elements_item_vec[k]->nodetype != SOFTWARE)continue;
                if(connection->software_id == ((XSDSoftWare *)elements_item_vec[k]->nodeptr)->softid)
                    break;
            }
            if(j != op_item_vec.size() && k != elements_item_vec.size())//画
            {
                //((XSDDevice *)op_item_vec[j]->nodeptr)->software_vec.push_back(((XSDSoftWare *)elements_item_vec[j]->nodeptr));
                ClickedItem* tail = new ClickedItem(0,0,SOFTWARE,OP_ITEM);
                tail->setParent(op_item_vec[j]);
                tail->nodeptr = connection;
                tail->item_name = ((XSDSoftWare *)elements_item_vec[k]->nodeptr)->name;
                op_arch_scene->addItem(tail);

                MyTreeItem *parent_item;
                parent_item = (MyTreeItem *)op_item_vec[j]->item_node;
                tree_item[0].push_back(new MyTreeItem(parent_item,QStringList(tail->item_name)));
                tail->item_node = tree_item[0][tree_item[0].size() - 1];
                tree_item[0][tree_item[0].size() - 1]->item_ptr = tail;
                tree_item[0][tree_item[0].size() - 1]->setData(0,5,SOFTWARE);
                tree_item[0][tree_item[0].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "software_icon.jpg"));
            }
        }
        set_element_info(sys);
    }
    else
    {
        add_sys_node(system);
    }
}

void MainWindow::set_subsys_item(XSDSubSystem *subsys, ClickedItem *item)
{
    item->nodeptr = subsys;
    item->item_name = subsys->name;
    op_item_vec.push_back(item);
    MyTreeItem *parent_item;

    if(item->parent->depth == 0)
        parent_item = tree_item[0][0];
    else
        parent_item = (MyTreeItem *)item->parent->item_node;
    tree_item[0].push_back(new MyTreeItem(parent_item,QStringList(subsys->name)));
    item->item_node = tree_item[0][tree_item[0].size() - 1];
    tree_item[0][tree_item[0].size() - 1]->item_ptr = item;
    tree_item[0][tree_item[0].size() - 1]->setData(0,5,SUB_SYS);
    tree_item[0][tree_item[0].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "system_icon.jpg"));

    connect(op_item_vec[op_item_vec.size() - 1],SIGNAL(pushBackItem(ClickedItem *)),this,SLOT(getItem(ClickedItem *)));
    op_arch_scene->addItem(op_item_vec[op_item_vec.size() - 1]);
    ClickedItem *tail;

    for(int i = 0;i < subsys->subsystem_vec.size();++i)
    {
        tail = new ClickedItem(0,0,SUB_SYS,OP_ITEM);
        tail->setParent(item);
        set_subsys_item(subsys->subsystem_vec[i],tail);
    }
    for(int i = 0;i < subsys->device_vec.size();++i)
    {
        tail = new ClickedItem(0,0,DEVICE,OP_ITEM);
        tail->setParent(item);
        set_dev_item(subsys->device_vec[i],tail);
    }
    for(int i = 0;i < subsys->switch_vec.size();++i)
    {
        tail = new ClickedItem(0,0,SWITCH,OP_ITEM);
        tail->setParent(item);
        set_switch_item(subsys->switch_vec[i],tail);
    }
}

void MainWindow::set_dev_item(XSDDevice *dev, ClickedItem *item)
{
    device_collection_vec.push_back(dev);
    item->nodeptr = dev;
    item->item_name = dev->name;
    op_item_vec.push_back(item);

    MyTreeItem *parent_item;
    parent_item = (MyTreeItem *)item->parent->item_node;
    tree_item[0].push_back(new MyTreeItem(parent_item,QStringList(dev->name)));
    item->item_node = tree_item[0][tree_item[0].size() - 1];
    tree_item[0][tree_item[0].size() - 1]->item_ptr = item;
    tree_item[0][tree_item[0].size() - 1]->setData(0,5,DEVICE);
    tree_item[0][tree_item[0].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "device_icon.jpg"));

    connect(op_item_vec[op_item_vec.size() - 1],SIGNAL(pushBackItem(ClickedItem *)),this,SLOT(getItem(ClickedItem *)));
    op_arch_scene->addItem(op_item_vec[op_item_vec.size() - 1]);

}

void MainWindow::set_switch_item(XSDSwitch *swi, ClickedItem *item)
{
    switch_collection_vec.push_back(swi);
    item->nodeptr = swi;
    item->item_name = swi->name;
    op_item_vec.push_back(item);

    MyTreeItem *parent_item;
    parent_item = (MyTreeItem *)item->parent->item_node;
    tree_item[0].push_back(new MyTreeItem(parent_item,QStringList(swi->name)));
    item->item_node = tree_item[0][tree_item[0].size() - 1];
    tree_item[0][tree_item[0].size() - 1]->item_ptr = item;
    tree_item[0][tree_item[0].size() - 1]->setData(0,5,SWITCH);
    tree_item[0][tree_item[0].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "switch_icon.jpg"));

    connect(op_item_vec[op_item_vec.size() - 1],SIGNAL(pushBackItem(ClickedItem *)),this,SLOT(getItem(ClickedItem *)));
    op_arch_scene->addItem(op_item_vec[op_item_vec.size() - 1]);
}

//------------------------  以下为菜单栏部分 ----------------------------

void MainWindow::create_file()
{
    QString file_name = QFileDialog::getSaveFileName(NULL,"新建文件","../untitled.xml","*.xml;;*.*");
    if(file_name == nullptr)return;
    xmlsystem = nullptr;
    myxmlfile = new QFile(file_name);
    //createxml->setfilename(file_name);
    set_sys_item(xmlsystem);
    qDebug()<<"create_file";
}

void MainWindow::open_file()
{
    QString file_name = QFileDialog::getOpenFileName(NULL,"打开文件","..","*.xml;;*.*");
    myxmlfile = new QFile(file_name);
    if(file_name == nullptr)return;
    xmlsystem = new XSDSystem();
    analysisxml->setfilename(file_name);
    analysisxml->getSystemInfo(xmlsystem);
    //createxml->setfilename(file_name);
    myxmlfile->copy(myxmlfile->fileName(),createxml->file_name);

    set_software_element(xmlsystem);
    set_message_element(xmlsystem);
    elements_scene->setSceneRect(0,0,elements_view->x() + IMAGE_W,elements_view->y()-IMAGE_H*(0 - 2*elements_item_vec.size()));
    set_sys_item(xmlsystem);
    set_message_connection_item(xmlsystem);
    for(int i = 0;i < 3;i++){
        if(tree_widget[i])
            tree_widget[i]->expandAll();
    }
    qDebug()<<"open_file";
}

void MainWindow::save_file()
{
    if(myxmlfile == nullptr) return ;
    myxmlfile->remove();
    qDebug()<<createxml->file_name;
    createxml->setSystemInfo(xmlsystem);
    qDebug()<<xmlsystem->name;
    myxmlfile->copy(createxml->file_name,myxmlfile->fileName());
    qDebug()<<"save_file";
}

void MainWindow::save_as_file()
{
    if(myxmlfile == nullptr) return ;
    QString file_name = QFileDialog::getSaveFileName(NULL,"另存为文件","../untitled.xml","*.xml;;*.*");
    if(file_name == nullptr)return;
    myxmlfile->setFileName(file_name);
    myxmlfile->remove();
    createxml->setSystemInfo(xmlsystem);
    myxmlfile->copy(createxml->file_name,myxmlfile->fileName());
    qDebug()<<"save_as_file";
}

void MainWindow::about_this()
{
    qDebug()<<"about_this";
}

//tree_widget右键菜单
void MainWindow::tree_widget_rightclicked(const QPoint &pos)
{
    if(!tree_widget[tree_tab->currentIndex()]->itemAt(pos))return;

    switch (tree_widget[tree_tab->currentIndex()]->itemAt(pos)->data(0,5).toInt())
    {
        case ROOT_NODE:
        {
            //rightClickedMenu->addAction(0,"添加子系统");
            return;
        }
        case SUB_SYS:
        {
            //rightClickedMenu->addAction(0,"添加子系统");
            //rightClickedMenu->addAction(1,"添加设备");
            //rightClickedMenu->addAction(2,"添加交换机");
            return;
        }
        case DEVICE:
        {
            //rightClickedMenu->addAction(0,"添加软件");
            return;
        }
        case SWITCH:
        {
            //rightClickedMenu->addAction(0,"添加硬件连接");
            return;
        }
        case SOFTWARE:
        {
            return;
        }
        case DDS_NODE:
        {
            rightclicked_action[2] = (rightClickedMenu->addAction(0,"添加DDS连接"));
            disconnect(rightclicked_action[2],0,0,0);
            connect(rightclicked_action[2],SIGNAL(triggered(bool)),this,SLOT(add_ddsconnection()));
            break;
        }
        case SOFTWARE_NODE:
        {
            rightclicked_action[2] = (rightClickedMenu->addAction(0,"添加软件元件"));
            disconnect(rightclicked_action[2],0,0,0);
            connect(rightclicked_action[2],SIGNAL(triggered(bool)),this,SLOT(add_software_element()));

            break;
        }
        case MESSAGE:
        {
            rightclicked_action[2] = (rightClickedMenu->addAction(0,"添加消息元件"));
            disconnect(rightclicked_action[2],0,0,0);
            connect(rightclicked_action[2],SIGNAL(triggered(bool)),this,SLOT(add_message_element()));
            break;
        }
        default:
        {
            return;
        }
    }
    rightClickedMenu->ShowMenu();

}

void MainWindow::showString(QString str)
{
    //if(str == "")return;
    test_textedit->append(str);
}


//------------------------  以下为添加系统结构部分 ----------------------------



//------------------------- 以下为添加系统结构部分 ----------------------------

void MainWindow::getItem(ClickedItem *item)
{
    op_item_vec.push_back(item);
    connect(item,SIGNAL(pushBackItem(ClickedItem *)),this,SLOT(getItem(ClickedItem *)));
    switch(item->nodetype)
    {
        case SUB_SYS:add_subsystem_node(item);break;
        case DEVICE:add_device_node(item);break;
        case SWITCH:add_switch_node(item);break;
        case SOFTWARE:add_software_node(item);break;
        default:break;
    }
}

void MainWindow::cancel_add_node(){
    op_stack->setCurrentIndex(tree_tab->currentIndex());
}

//填写系统信息
void MainWindow::add_sys_node(ClickedItem *item)
{
    delete_layout(op_stack_layout[3]);
    op_stack->setCurrentIndex(3);
    for(int i = 0;i < 2;++i)
    {
        op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_XSDSystem[i] + ":"));
        op_stack_layout[3]->addWidget(input_lineedit[i] = new QLineEdit());
    }
    op_stack_layout[3]->addWidget(input_label[2] = new QLabel());
    op_stack_layout[3]->addWidget(enter_button = new QPushButton("确定"));
    submit_item = item;
    connect(enter_button,SIGNAL(clicked(bool)),this,SLOT(submit_system()));
}

//提交系统信息 更新到内存
void MainWindow::submit_system()
{
    XSDSystem *sys = new XSDSystem();
    for(int i = 0;i < 2;i++)
    {
        *(&sys->name +i) = input_lineedit[i]->text();
        delete(input_label[i]);
        delete(input_lineedit[i]);
    }
    enter_button->disconnect();
    delete(enter_button);
    enter_button = nullptr;
    delete(input_label[2]);
    input_label[2] = nullptr;
    submit_item->item_name = sys->name;
    submit_item->nodeptr = sys;

    op_arch_scene->addItem(submit_item);
    for(int i = 0;i < MAX_TAB_NUM;++i)
    {
        tree_widget[i]->setHeaderLabels(QStringList(sys->name));
    }
    xmlsystem = sys;
    connect(submit_item,SIGNAL(pushBackItem(ClickedItem *)),this,SLOT(getItem(ClickedItem *)));
    op_stack->setCurrentIndex(0);
    createxml->setSystemInfo(xmlsystem);
    set_element_info(sys);
    //system("gvim " + createxml->file_name.toLatin1());
}

//填写子系统信息
void MainWindow::add_subsystem_node(ClickedItem *item)
{
    delete_layout(op_stack_layout[3]);
    op_stack->setCurrentIndex(3);
    for(int i = 0;i < 3;++i)
    {
        op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_XSDSubSystem[i] + ":"));
        op_stack_layout[3]->addWidget(input_lineedit[i] = new QLineEdit());
    }
    op_stack_layout[3]->addWidget(input_label[3] = new QLabel());
    op_stack_layout[3]->addWidget(enter_button = new QPushButton("确定"));
    op_stack_layout[3]->addWidget(cancel_button = new QPushButton("取消"));
    submit_item = item;

    connect(enter_button,SIGNAL(clicked(bool)),this,SLOT(submit_subsystem()));
    connect(cancel_button,SIGNAL(clicked(bool)),this,SLOT(cancel_add_node()));
}

//提交子系统信息
void MainWindow::submit_subsystem()
{
    XSDSubSystem *subsys = new XSDSubSystem();
    for(int i = 0;i < 3;i++)
    {
        *(&subsys->id +i) = input_lineedit[i]->text();
        delete(input_label[i]);
        delete(input_lineedit[i]);
    }
    enter_button->disconnect();
    delete(enter_button);
    enter_button = nullptr;
    delete(input_label[3]);
    input_label[3] = nullptr;
    submit_item->item_name = subsys->name;
    submit_item->nodeptr = subsys;
    if(submit_item->parent->parent)//如果该节点的父亲节点也是子系统（该节点的父亲的父亲是node）
    {
        ((XSDSubSystem *)submit_item->parent->nodeptr)->subsystem_vec.push_back(subsys);
    }
    else
    {
        //该节点是最上层子系统
        if(((XSDSystem *)submit_item->parent->nodeptr)->node_vec.size() == 0)
            ((XSDSystem *)submit_item->parent->nodeptr)->node_vec.push_back(new Node());
        ((XSDSystem *)submit_item->parent->nodeptr)->node_vec[0]->subsystem_vec.push_back(subsys);
    }
    op_arch_scene->addItem(submit_item);

    MyTreeItem *parent_item;
    if(submit_item->parent->depth == 0)
        parent_item = tree_item[0][0];//根节点
    else
        parent_item = (MyTreeItem *)submit_item->parent->item_node;//父亲节点
    tree_item[0].push_back(new MyTreeItem(parent_item,QStringList(subsys->name)));
    submit_item->item_node = tree_item[0][tree_item[0].size() - 1];

    tree_item[0][tree_item[0].size() - 1]->item_ptr = submit_item;
    tree_item[0][tree_item[0].size() - 1]->setData(0,5,SUB_SYS);
    tree_item[0][tree_item[0].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "system_icon.jpg"));
    tree_widget[0]->expandAll();
    op_stack->setCurrentIndex(0);
    createxml->setSystemInfo(xmlsystem);
    //system("gvim " + createxml->file_name.toLatin1());
}

//填写设备信息
void MainWindow::add_device_node(ClickedItem *item)
{
    delete_layout(op_stack_layout[3]);
    op_stack->setCurrentIndex(3);
    for(int i = 0;i < 3;++i)
    {
        op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_XSDDevice[i] + ":"));
        if(i != 2)
            op_stack_layout[3]->addWidget(input_lineedit[i] = new QLineEdit());
        else
        {
            op_stack_layout[3]->addWidget(input_box[i] = new QComboBox());
            for(int j = 0;j < 5;++j)
            {
                input_box[i]->addItem(_XSDNodeType[j]);
            }
        }
    }
    op_stack_layout[3]->addWidget(input_label[3] = new QLabel());
    next_button = new QPushButton("添加" + CHS_XSDDevice[3]);
    op_stack_layout[3]->addWidget(next_button);
    submit_item = item;
    connect(next_button,SIGNAL(clicked(bool)),this,SLOT(add_networkinterface()));
}

//添加网络接口信息
void MainWindow::add_networkinterface()
{
    //首先获取设备基本信息 释放内存
    qDebug()<<"添加网络接口信息";
    if(input_lineedit[0] != nullptr)
    {
        XSDDevice *dev = new XSDDevice();
        for(int i = 0;i <  3;++i)
        {
            if(i == 2)
            {
                *(&dev->id + i) = input_box[i]->currentText();
                delete(input_box[i]);
                input_box[i] = nullptr;
            }
            else
            {
                *(&dev->id + i) = input_lineedit[i]->text();
                delete(input_lineedit[i]);
                input_lineedit[i] = nullptr;
            }
            delete(input_label[i]);
            input_label[i] = nullptr;
        }
        delete(input_label[3]);
        input_label[3] = nullptr;
        delete(next_button);
        next_button = nullptr;


        submit_item->item_name = dev->name;
        ((XSDSubSystem *)submit_item->parent->nodeptr)->device_vec.push_back(dev);

        for(int i = 0;i < 4;++i)
        {
            op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_XSDNetworkInterface[i] + ":"));
            op_stack_layout[3]->addWidget(add_input_lineedit[i] = new QLineEdit());
        }
        op_stack_layout[3]->addWidget(input_label[4] = new QLabel());
        op_stack_layout[3]->addWidget(next_button = new QPushButton("添加" + CHS_XSDDevice[3]));
        op_stack_layout[3]->addWidget(enter_button = new QPushButton("确定"));
        connect(enter_button,SIGNAL(clicked(bool)),this,SLOT(submit_device()));
        connect(next_button,SIGNAL(clicked(bool)),this,SLOT(add_networkinterface()));
    }
    else
    {
        XSDNetworkInterface *interface = new XSDNetworkInterface();
        for(int i = 0;i < 4;++i)
        {
            *(&interface->MACadress + i) = add_input_lineedit[i]->text();
        }
        ((XSDSubSystem *)submit_item->parent->nodeptr)->device_vec[((XSDSubSystem *)\
                         submit_item->parent->nodeptr)->device_vec.size() - 1]->networkinterface_vec.push_back(interface);
        for(int i = 0;i < 4;++i){
            //qDebug()<<"清零";
            add_input_lineedit[i]->setText("");
        }
    }
}


//提交设备信息
void MainWindow::submit_device()
{
    XSDNetworkInterface *interface = new XSDNetworkInterface();
    for(int i = 0;i < 4;++i)
    {
        *(&interface->MACadress + i) = add_input_lineedit[i]->text();
        delete(input_label[i]);
        input_label[i] = nullptr;
        delete(add_input_lineedit[i]);
        add_input_lineedit[i] = nullptr;
    }
    delete(input_label[4]);
    input_label[4] = nullptr;
    enter_button->disconnect();
    next_button->disconnect();
    delete(enter_button);
    enter_button = nullptr;
    delete(next_button);
    next_button = nullptr;
    submit_item->nodeptr = ((XSDSubSystem *)submit_item->parent->nodeptr)->device_vec[((XSDSubSystem *)\
                                                   submit_item->parent->nodeptr)->device_vec.size() - 1];

    ((XSDSubSystem *)submit_item->parent->nodeptr)->device_vec[((XSDSubSystem *)\
                     submit_item->parent->nodeptr)->device_vec.size() - 1]->networkinterface_vec.push_back(interface);
    op_arch_scene->addItem(submit_item);

    MyTreeItem *parent_item;
    parent_item = (MyTreeItem *)submit_item->parent->item_node;
    tree_item[0].push_back(new MyTreeItem(parent_item,QStringList(submit_item->item_name)));
    submit_item->item_node = tree_item[0][tree_item[0].size() - 1];
    tree_item[0][tree_item[0].size() - 1]->item_ptr = submit_item;
    tree_item[0][tree_item[0].size() - 1]->setData(0,5,DEVICE);
    tree_item[0][tree_item[0].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "device_icon.jpg"));
    tree_widget[0]->expandAll();

    device_collection_vec.push_back((XSDDevice *)submit_item->nodeptr);
    op_stack->setCurrentIndex(0);
    createxml->setSystemInfo(xmlsystem);
    //system("gvim " + createxml->file_name.toLatin1());
}

//添加交换机信息
void MainWindow::add_switch_node(ClickedItem *item)
{
    delete_layout(op_stack_layout[3]);
    op_stack->setCurrentIndex(3);
    for(int i = 0;i < 4;++i)
    {
        op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_XSDSwitch[i] + ":"));
        op_stack_layout[3]->addWidget(input_lineedit[i] = new QLineEdit());
    }
    input_label[3]->setText(CHS_XSDDevice[3] + " (如有多个请用空格隔开):");
    op_stack_layout[3]->addWidget(input_label[4] = new QLabel());
    op_stack_layout[3]->addWidget(enter_button = new QPushButton("确定"));
    submit_item = item;
    connect(enter_button,SIGNAL(clicked(bool)),this,SLOT(submit_switch()));
}

//提交交换机信息
void MainWindow::submit_switch()
{
    XSDSwitch *swi = new XSDSwitch();
    for(int i = 0;i < 2;i++)
    {
        *(&swi->id +i) = input_lineedit[i]->text();
    }
    //ip
    swi->ip_vec.push_back(input_lineedit[2]->text());
    //port
    QStringList list = input_lineedit[3]->text().split(' ');
    for(int i = 0;i < list.size();++i)
    {
        swi->portnumber_vec.push_back(list.value(i).toInt());
    }

    for(int i = 0;i < 4;i++)
    {
        delete(input_label[i]);
        delete(input_lineedit[i]);
    }
    enter_button->disconnect();
    delete(enter_button);
    enter_button = nullptr;
    delete(input_label[4]);
    input_label[3] = nullptr;
    submit_item->item_name = swi->name;
    submit_item->nodeptr = swi;

    ((XSDSubSystem *)submit_item->parent->nodeptr)->switch_vec.push_back(swi);

    MyTreeItem *parent_item;
    parent_item = (MyTreeItem *)submit_item->parent->item_node;
    tree_item[0].push_back(new MyTreeItem(parent_item,QStringList(swi->name)));
    submit_item->item_node = tree_item[0][tree_item[0].size() - 1];
    tree_item[0][tree_item[0].size() - 1]->item_ptr = submit_item;
    tree_item[0][tree_item[0].size() - 1]->setData(0,5,SWITCH);
    tree_item[0][tree_item[0].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "switch_icon.jpg"));
    tree_widget[0]->expandAll();

    switch_collection_vec.push_back(swi);
    op_arch_scene->addItem(submit_item);
    op_stack->setCurrentIndex(0);
    createxml->setSystemInfo(xmlsystem);
    //system("gvim " + createxml->file_name.toLatin1());
}

void MainWindow::add_software_node(ClickedItem *item)
{
    delete_layout(op_stack_layout[3]);
    op_stack->setCurrentIndex(3);
    /*
    for(int i = 0;i < 5;++i)
    {
        op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_XSDSoftWare[i] + ":"));
        op_stack_layout[3]->addWidget(input_lineedit[i] = new QLineEdit());
    }*/
    op_stack_layout[3]->addWidget(input_label[5] = new QLabel("软件部署信息:"));
    op_stack_layout[3]->addWidget(input_lineedit[5] = new QLineEdit());
    op_stack_layout[3]->addWidget(input_label[6] = new QLabel());
    op_stack_layout[3]->addWidget(enter_button = new QPushButton("确定"));
    submit_item = item;
    connect(enter_button,SIGNAL(clicked(bool)),this,SLOT(submit_software()));
}

void MainWindow::submit_software()
{
    xmlsystem->softwareset->software_vec.size();
    XSDSoftWare *software = new XSDSoftWare();
    XSDSoftWare *ele = new XSDSoftWare();
    //找到软件
    for(int j = 0;j < xmlsystem->softwareset->software_vec.size();++j)
    {
        ele = xmlsystem->softwareset->software_vec[j];

        if(ele->name == submit_item->item_name)
            for(int i = 0;i < 5;i++)
            {
                *(&software->softid + i) = *(&ele->softid + i);
            }

    }
    enter_button->disconnect();
    delete(enter_button);
    enter_button = nullptr;
    delete(input_label[6]);
    input_label[6] = nullptr;
    submit_item->item_name = software->name;
    //submit_item->nodeptr = software;
    /*
    //添加软件节点
    if(xmlsystem->softwareset == nullptr)
        xmlsystem->softwareset = new SoftwareSet();
    xmlsystem->softwareset->software_vec.push_back(software);
    */
    //添加软件部署关系
    if(xmlsystem->interfacerelationset == nullptr)
        xmlsystem->interfacerelationset = new InterfaceRelationSet();
    if(xmlsystem->interfacerelationset->softwareconnection == nullptr)
        xmlsystem->interfacerelationset->softwareconnection = new SoftwareConnection();
    SubSoftwareConnection *conn = new SubSoftwareConnection();
    conn->device_id = ((XSDDevice *)submit_item->parent->nodeptr)->id;
    conn->software_id = software->softid;
    conn->info = input_lineedit[5]->text();
    submit_item->nodeptr = conn;
    xmlsystem->interfacerelationset->softwareconnection->connection_vec.push_back(conn);
    delete(input_label[5]);
    delete(input_lineedit[5]);

    MyTreeItem *parent_item;
    parent_item = (MyTreeItem *)submit_item->parent->item_node;
    tree_item[0].push_back(new MyTreeItem(parent_item,QStringList(software->name)));
    submit_item->item_node = tree_item[0][tree_item[0].size() - 1];
    tree_item[0][tree_item[0].size() - 1]->item_ptr = submit_item;
    tree_item[0][tree_item[0].size() - 1]->setData(0,5,SOFTWARE);
    tree_item[0][tree_item[0].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "software_icon.jpg"));
    tree_widget[0]->expandAll();


    op_arch_scene->addItem(submit_item);
    op_stack->setCurrentIndex(0);
    createxml->setSystemInfo(xmlsystem);
    //system("gvim " + createxml->file_name.toLatin1());
}


//------------------------  以下为添加系统元素部分 ----------------------------



//------------------------- 以下为添加系统元素部分 ----------------------------

//添加应用软件信息
void MainWindow::add_software_element()
{
    QLayoutItem *item;
    if(xmlsystem == nullptr)
    {
        return;
    }
    delete_layout(op_stack_layout[3]);
    op_stack->setCurrentIndex(3);
    for(int i = 0;i < 5;++i)
    {
        op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_XSDSoftWare[i] + ":"));
        if(i != 2)
            op_stack_layout[3]->addWidget(input_lineedit[i] = new QLineEdit());
        else
        {
            op_stack_layout[3]->addWidget(input_box[i] = new QComboBox());
            for(int j = 0;j < 2;++j)
                input_box[i]->addItem(_XSDSoftwareType[j]);
        }
    }
    op_stack_layout[3]->addWidget(input_label[5] = new QLabel());
    op_stack_layout[3]->addWidget(enter_button = new QPushButton("确定"));
    connect(enter_button,SIGNAL(clicked(bool)),this,SLOT(sumbit_software_element()));
}

//提交软件信息
void MainWindow::sumbit_software_element()
{

    XSDSoftWare *software = new XSDSoftWare();
    for(int i = 0;i < 5;i++)
    {
        if(i != 2)
        {
            *(&software->softid + i) = input_lineedit[i]->text();
            delete(input_label[i]);
            delete(input_lineedit[i]);
        }
        else
        {
            *(&software->softid + i) = input_box[i]->currentText();
            delete(input_box[i]);
            delete(input_label[i]);
        }
    }
    enter_button->disconnect();
    delete(enter_button);
    enter_button = nullptr;
    delete(input_label[5]);
    input_label[5] = nullptr;
    //添加软件节点
    if(xmlsystem->softwareset == nullptr)
        xmlsystem->softwareset = new SoftwareSet();
    xmlsystem->softwareset->software_vec.push_back(software);
    tree_item[2].push_back(new MyTreeItem(tree_item[2][2],QStringList(software->name)));
    tree_item[2][tree_item[2].size() - 1]->setData(0,5,SOFTWARE);
    tree_item[2][tree_item[2].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "software_icon.jpg"));
    tree_widget[2]->expandAll();

    //添加到元件栏
    elements_item_vec.push_back(new ClickedItem(element_pos,\
                                                elements_view->y() - IMAGE_H*(0 - 2*elements_item_vec.size()),SOFTWARE));
    elements_scene->addItem(elements_item_vec[elements_item_vec.size() - 1]);
    elements_item_vec[elements_item_vec.size() - 1]->nodeptr = \
            xmlsystem->softwareset->software_vec[xmlsystem->softwareset->software_vec.size() - 1];
    elements_item_vec[elements_item_vec.size() - 1]->item_name = software->name;
    elements_scene->setSceneRect(0,0,elements_view->x() + IMAGE_W,elements_view->y()-IMAGE_H*(0 - 2*elements_item_vec.size()));
    elements_item_vec[elements_item_vec.size() - 1]->item_node = tree_item[2][tree_item[2].size() - 1];

    //添加到表格
    QTableWidgetItem *item = new QTableWidgetItem(software->name);
    QTableWidgetItem *item_id = new QTableWidgetItem(software->softid);
    op_elements_table[1]->setRowCount(op_elements_table[1]->rowCount() + 1);
    op_elements_table[1]->setItem(op_elements_table[1]->rowCount() - 1,0,item);
    op_elements_table[1]->setItem(op_elements_table[1]->rowCount() - 1,1,item_id);
    op_stack->setCurrentIndex(2);
    createxml->setSystemInfo(xmlsystem);

}

void MainWindow::add_message_element()
{
    QLayoutItem *item;
    if(xmlsystem == nullptr)
    {
        return;
    }
    delete_layout(op_stack_layout[3]);
    op_stack->setCurrentIndex(3);
    for(int i = 0;i < 2;++i)
    {
        op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_Msg[i] + ":"));
        op_stack_layout[3]->addWidget(input_lineedit[i] = new QLineEdit());
    }
    for(int i = 2;i < 4;++i)
    {
        op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_Msg[i] + ":"));
        op_stack_layout[3]->addWidget(input_box[i] = new QComboBox());
    }
    for(int i = 0;i < 3;++i)
        input_box[2]->addItem(_XSDMsgType[i]);
    for(int i = 0;i < 2;++i)
        input_box[3]->addItem(_XSDTransportType[i]);

    op_stack_layout[3]->addWidget(input_label[4] = new QLabel());
    next_button = new QPushButton("添加" + CHS_Msg[4]);
    op_stack_layout[3]->addWidget(next_button);
    connect(next_button,SIGNAL(clicked(bool)),this,SLOT(add_struct_info()));
}

void MainWindow::submit_message_element()
{
    MsgElement *ele = new MsgElement();
    for(int i = 0;i < 4;++i)
    {
        *(&ele->name + i) = add_input_lineedit[i]->text();
        delete(input_label[i]);
        input_label[i] = nullptr;
        delete(add_input_lineedit[i]);
        add_input_lineedit[i] = nullptr;
    }
    ele->maxlength = add_input_lineedit[4]->text().toInt();
    for(int i = 5;i < 9;++i)
    {
        *(&ele->name + i) = add_input_lineedit[i]->text();
        delete(input_label[i]);
        input_label[i] = nullptr;
        delete(add_input_lineedit[i]);
        add_input_lineedit[i] = nullptr;
    }
    //添加软件节点
    if(xmlsystem->msgset == nullptr)
        xmlsystem->msgset = new MsgSet();
    xmlsystem->msgset->msg_vec[xmlsystem->msgset->msg_vec.size() - 1]->msgstruct->element_vec.push_back(ele);

    delete(input_label[9]);
    input_label[9] = nullptr;
    enter_button->disconnect();
    next_button->disconnect();
    delete(enter_button);
    enter_button = nullptr;
    delete(next_button);
    next_button = nullptr;


    tree_item[2].push_back(new MyTreeItem(tree_item[2][3],QStringList(xmlsystem->msgset->msg_vec[xmlsystem->msgset->msg_vec.size() - 1]->name)));
    tree_item[2][tree_item[2].size() - 1]->setData(0,5,MESSAGE);
    tree_item[2][tree_item[2].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "message_icon.jpg"));
    tree_widget[2]->expandAll();

    elements_item_vec.push_back(new ClickedItem(element_pos,\
                                                elements_view->y() - IMAGE_H*(0 - 2*elements_item_vec.size()),MESSAGE));
    elements_scene->addItem(elements_item_vec[elements_item_vec.size() - 1]);
    elements_item_vec[elements_item_vec.size() - 1]->nodeptr = \
            xmlsystem->msgset->msg_vec[xmlsystem->msgset->msg_vec.size() - 1];
    elements_item_vec[elements_item_vec.size() - 1]->item_name = xmlsystem->msgset->msg_vec[xmlsystem->msgset->msg_vec.size() - 1]->name;

    elements_scene->setSceneRect(0,0,elements_view->x() + IMAGE_W,elements_view->y()-IMAGE_H*(0 - 2*elements_item_vec.size()));
    QTableWidgetItem *item = new QTableWidgetItem(xmlsystem->msgset->msg_vec[xmlsystem->msgset->msg_vec.size() - 1]->name);
    QTableWidgetItem *item_id = new QTableWidgetItem(xmlsystem->msgset->msg_vec[xmlsystem->msgset->msg_vec.size() - 1]->id);
    op_elements_table[2]->setRowCount(op_elements_table[2]->rowCount() + 1);
    op_elements_table[2]->setItem(op_elements_table[2]->rowCount() - 1,0,item);
    op_elements_table[2]->setItem(op_elements_table[2]->rowCount() - 1,1,item_id);

    op_stack->setCurrentIndex(2);
    createxml->setSystemInfo(xmlsystem);
}

void MainWindow::add_struct_info()
{
    if(input_lineedit[0] != nullptr)
    {
        Msg *msg = new Msg();
        for(int i = 0;i <  4;++i)
        {
            if(i < 2)
            {
                *(&msg->name + i) = input_lineedit[i]->text();
                delete(input_lineedit[i]);
                input_lineedit[i] = nullptr;
            }
            else
            {
                *(&msg->name + i) = input_box[i]->currentText();
                delete(input_box[i]);
                input_box[i] = nullptr;
            }
            delete(input_label[i]);
            input_label[i] = nullptr;
        }
        delete(input_label[3]);
        input_label[4] = nullptr;
        delete(next_button);
        next_button = nullptr;
        msg->msgstruct = new MsgStruct();
        xmlsystem->msgset->msg_vec.push_back(msg);
        for(int i = 0;i < 9;++i)
        {
            if(i == 1 || i == 2)
                op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_MsgElement[i] + "(*):"));
            else
                op_stack_layout[3]->addWidget(input_label[i] = new QLabel(CHS_MsgElement[i] + ":"));
            op_stack_layout[3]->addWidget(add_input_lineedit[i] = new QLineEdit());
        }
        op_stack_layout[3]->addWidget(input_label[9] = new QLabel());
        op_stack_layout[3]->addWidget(next_button = new QPushButton("添加" + CHS_Msg[4]));
        op_stack_layout[3]->addWidget(enter_button = new QPushButton("确定"));
        next_button->disconnect();
        connect(next_button,SIGNAL(clicked(bool)),this,SLOT(add_struct_info()));
        connect(enter_button,SIGNAL(clicked(bool)),this,SLOT(submit_message_element()));
    }
    else
    {
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
        xmlsystem->msgset->msg_vec[xmlsystem->msgset->msg_vec.size() - 1]->msgstruct->element_vec.push_back(ele);
        for(int i = 0;i < 9;++i)
            add_input_lineedit[i]->setText("");
    }
}

void MainWindow::delete_layout(QLayout * p_layout)
{
    QLayoutItem *child;
    qDebug()<<"test1";
    while((child = p_layout->takeAt(0)) != 0)
    {
        qDebug()<<"test2";
        if(child->widget() != NULL)
        {
            QWidget* p_widget= child->widget();
            p_layout->removeWidget(p_widget);
            qDebug()<<"test3";
            delete p_widget;
        }
        p_layout->removeItem(child);
        delete child;
    }
}

void MainWindow::add_ddsconnection()
{
    if(xmlsystem == nullptr || xmlsystem->softwareset->software_vec.size()==0 || xmlsystem->msgset->msg_vec.size()==0)
    {
        return;
    }
    delete_layout(op_stack_layout[3]);
    op_stack->setCurrentIndex(3);
    op_stack_layout[3]->addWidget(input_label[0] = new QLabel(CHS_DDSInterfaceRelation[0] + ":"));
    op_stack_layout[3]->addWidget(input_lineedit[0] = new QLineEdit());


    for(int i = 0;i < 3;++i)
    {
        op_stack_layout[3]->addWidget(input_label[1 + i] = new QLabel(CHS_DDSInterfaceRelation[1] + CHS_PublichMember[i] + ":"));
        if(i != 0)
            op_stack_layout[3]->addWidget(input_lineedit[1 + i] = new QLineEdit());
        else
            op_stack_layout[3]->addWidget(input_box[1 + i] = new QComboBox());
    }
    for(int j = 0;j < xmlsystem->softwareset->software_vec.size();++j)
        input_box[1]->addItem(xmlsystem->softwareset->software_vec[j]->softid);

    for(int i = 0;i < 2;++i)
    {
        op_stack_layout[3]->addWidget(input_label[4 + i] = new QLabel(CHS_DDSInterfaceRelation[2] + CHS_SubscribeMember[i] + ":"));
        if(i != 0)
            op_stack_layout[3]->addWidget(input_lineedit[4 + i] = new QLineEdit());
        else
            op_stack_layout[3]->addWidget(input_box[4 + i] = new QComboBox());
    }
    for(int j = 0;j < xmlsystem->softwareset->software_vec.size();++j)
        input_box[4]->addItem(xmlsystem->softwareset->software_vec[j]->softid);

    op_stack_layout[3]->addWidget(input_label[6] = new QLabel(CHS_DDSInterfaceRelation[3] + ":"));
    op_stack_layout[3]->addWidget(input_box[6] = new QComboBox());
    for(int j = 0;j < xmlsystem->msgset->msg_vec.size();++j)
        input_box[6]->addItem(xmlsystem->msgset->msg_vec[j]->id);

    op_stack_layout[3]->addWidget(input_label[7] = new QLabel());
    enter_button = new QPushButton("确定");
    op_stack_layout[3]->addWidget(enter_button);
    connect(enter_button,SIGNAL(clicked(bool)),this,SLOT(submit_ddsconnection()));
}

void MainWindow::submit_ddsconnection()
{
    DDSInterfaceRelation *relation = new DDSInterfaceRelation();
    relation->domainid = input_lineedit[0]->text();
    PublishMember *pub = new PublishMember();
    for(int i = 0;i < 3;++i)
    {
        if(i != 0)
            *(&pub->softwareid + i) = input_lineedit[i + 1]->text();
        else
            *(&pub->softwareid + i) = input_box[i + 1]->currentText();
    }
    relation->publishmember_vec.push_back(pub);

    SubscribeMember *sub = new SubscribeMember();
    for(int i = 0;i < 2;++i)
    {
        if(i != 0)
            *(&sub->softwareid + i) = input_lineedit[i + 4]->text();
        else
            *(&sub->softwareid + i) = input_box[i + 4]->currentText();
    }

    relation->subscribemember_vec.push_back(sub);

    relation->message = input_box[6]->currentText();

    for(int i = 0;i < 7;++i)
    {
        delete(input_label[i]);
        input_label[i] = nullptr;
        if(i != 1 && i != 4 && i != 6)
        {
            delete(input_lineedit[i]);
            input_lineedit[i] = nullptr;
        }
        else
        {
            delete(input_box[i]);
            input_box[i] = nullptr;
        }
    }

    delete(input_label[7]);
    input_label[7] = nullptr;
    enter_button->disconnect();
    delete(enter_button);
    enter_button = nullptr;

    if(xmlsystem->interfacerelationset == nullptr)
        xmlsystem->interfacerelationset = new InterfaceRelationSet();
    if(xmlsystem->interfacerelationset->interfacerelation == nullptr)
        xmlsystem->interfacerelationset->interfacerelation = new InterfaceRelation();
    xmlsystem->interfacerelationset->interfacerelation->DDSinterfacerelation_vec.push_back(relation);

    //更新树
    MyTreeItem *parent = nullptr;
    MyTreeItem *pub_item = nullptr;
    MyTreeItem *sub_item = nullptr;
    int k;
    for(int i = 0;i < tree_item[1].size();++i)
    {
        qDebug()<<tree_item[1][i]->text(0)<<relation->message;
        if(tree_item[1][i]->text(0) == relation->message)
        {
            parent = tree_item[1][i];
            k = i;
            break;
        }
    }
    if(parent == nullptr)
    {
        parent = new MyTreeItem(tree_item[1][1],QStringList(relation->message));
        tree_item[1].push_back(parent);
        parent->setData(0,5,MESSAGE);
        parent->setIcon(0,QIcon(IMAGE_PATH + "message_icon.jpg"));

        tree_item[1].push_back(new MyTreeItem(parent,QStringList("发布成员")));
        pub_item = tree_item[1][tree_item[1].size() - 1];
        pub_item->setData(0,5,DEFAULT_NODE);

        tree_item[1].push_back(new MyTreeItem(parent,QStringList("接收成员")));
        sub_item = tree_item[1][tree_item[1].size() - 1];
        sub_item->setData(0,5,DEFAULT_NODE);
    } else {
        pub_item = tree_item[1][k+1];
        sub_item = tree_item[1][k+2];
    }

    XSDSoftWare *pub_soft = nullptr;
    XSDSoftWare *sub_soft = nullptr;
    for(int i = 0;i < xmlsystem->softwareset->software_vec.size();++i){
        if(xmlsystem->softwareset->software_vec[i]->softid == relation->publishmember_vec[0]->softwareid){
            pub_soft = xmlsystem->softwareset->software_vec[i];
        }
        if(xmlsystem->softwareset->software_vec[i]->softid == relation->subscribemember_vec[0]->softwareid){
            sub_soft = xmlsystem->softwareset->software_vec[i];
        }
    }

    for(int i = 0;i < pub_item->childCount();i++){
        if(pub_item->child(i)->text(0) == pub_soft->name){
            pub_soft = nullptr;
        }
    }
    for(int i = 0;i < sub_item->childCount();i++){
        if(sub_item->child(i)->text(0) == sub_soft->name){
            sub_soft = nullptr;
        }
    }

    if(pub_soft){
        tree_item[1].push_back(new MyTreeItem(pub_item,QStringList(pub_soft->name)));
        tree_item[1][tree_item[1].size() - 1]->setData(0,5,SOFTWARE);
        tree_item[1][tree_item[1].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "software_icon.jpg"));
    }

    if(sub_soft){
        tree_item[1].push_back(new MyTreeItem(sub_item,QStringList(sub_soft->name)));
        tree_item[1][tree_item[1].size() - 1]->setData(0,5,SOFTWARE);
        tree_item[1][tree_item[1].size() - 1]->setIcon(0,QIcon(IMAGE_PATH + "software_icon.jpg"));
    }
    tree_widget[1]->expandAll();

    op_stack->setCurrentIndex(1);
    createxml->setSystemInfo(xmlsystem);
}


//同步修改
void MainWindow::changeSoftwareItem(changeName software){
    qDebug()<<software.oldName<<software.newName;
    if(software.oldName == software.newName){
        return;
    }
    //修改相同的item
    for(int i = 0;i < tree_item[0].size();i++){
        if(tree_item[0][i]->text(0) == software.oldName){
            //tree_item[0][i]->setText(0,software.newName);
            if(tree_item[0][i]->item_ptr != nullptr){
                tree_item[0][i]->item_ptr->item_name = software.newName;
                tree_item[0][i]->item_ptr->update();
            }
        }
    }
    //修改tree
    for(int j = 0;j<3;j++){
        for(int i = 0;i < tree_item[j].size();i++){
            if(tree_item[j][i]->text(0) == software.oldName){
                tree_item[j][i]->setText(0,software.newName);
            }
        }
    }
    //修改table
    for(int i = 0;i < op_elements_table[1]->rowCount();i++){
        if(op_elements_table[1]->item(i,0)->text() == software.oldName){
            op_elements_table[1]->item(i,0)->setText(software.newName);
        }
        //qDebug()<<op_elements_table[1]->item(i,0)->text();
    }
}


void MainWindow::changeMessageItem(changeName msg){
    qDebug()<<msg.newName<<msg.oldName;
    //修改table
    for(int i = 0;i < op_elements_table[2]->rowCount();i++){
        if(op_elements_table[2]->item(i,0)->text() == msg.oldName){
            op_elements_table[2]->item(i,0)->setText(msg.newName);
        }
        //qDebug()<<op_elements_table[1]->item(i,0)->text();
    }
}
