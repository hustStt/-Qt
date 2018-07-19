#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include <QMenu>
#include "type.h"
#include <QFrame>
#include <QLayout>
#include <QTextEdit>
#include <QDockWidget>
#include <QDebug>
#include <QTabWidget>
#include <QTreeWidget>
//#include <QStandardItem>
#include <QPoint>
#include "rightclickedmenu.h"
#include "clickeditem.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <vector>
#include <QScrollBar>
#include <QStackedWidget>
#include <QTableWidget>
#include <QPushButton>
#include "xsd_type.h"
#include "xml_analysis.h"
#include "xml_create.h"
#include <QFile>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QSet>
#include <QComboBox>
#include <showitem_frame.h>
#include <QScrollArea>
#include "mydockwidget.h"
#include "mytreeitem.h"
#include <QTimer>
#include "update_ui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QFile *myxmlfile=nullptr;
    XmlAnalysis *analysisxml;
    XSDSystem *xmlsystem = nullptr;
    XmlCreate *createxml;

    //menubar
    QMenu *menu[MAX_MENU_NUM];
    QAction *menu_action[MAX_MENU_NUM][MAX_MENU_NUM];

    //toolbar
    QToolBar *tool[MAX_MENU_NUM];
    QAction *tool_action[MAX_MENU_NUM];


    //frame & layout
    QFrame *main_frame;
    QHBoxLayout *main_layout;
    QFrame *tree_frame;
    QFrame *op_frame;
    QFrame *elements_frame;
    QHBoxLayout *tree_layout;
    QHBoxLayout *op_layout;
    QFrame *tree_tab_frame[MAX_TAB_NUM];
    QHBoxLayout *tree_tab_layout[MAX_TAB_NUM];
    QHBoxLayout *elements_layout;
    QFrame *op_stack_frame[MAX_TAB_NUM];
    QVBoxLayout *op_stack_layout[MAX_TAB_NUM];

    QFrame *interface_stack_frame[MAX_TAB_NUM];
    QVBoxLayout *interface_stack_layout[MAX_TAB_NUM];


    //tree
    QTabWidget *tree_tab;
    QTreeWidget *tree_widget[MAX_TAB_NUM];
    RightClickedMenu *rightClickedMenu;
    //MyTreeItem *tree_item[MAX_TAB_NUM][MAX_ITEM_NUM];
    std::vector <MyTreeItem *> tree_item[MAX_TAB_NUM];
    QAction * rightclicked_action[MAX_TAB_NUM];
    QTableWidget *op_elements_table[3];

    QGraphicsScene *elements_scene;
    QGraphicsView *elements_view;
    QGraphicsScene *op_arch_scene;
    QGraphicsView *op_arch_view;
    QGraphicsScene *op_hardware_interface_scene;
    QGraphicsView *op_hardware_interface_view;
    std::vector <ClickedItem *> elements_item_vec;
    std::vector <ClickedItem *> op_item_vec;

    QStackedWidget *op_stack;
    QStackedWidget *interface_stack;

    //测试用
    QTextEdit *test_textedit;
    //输入用
    QLabel *input_label[MAX_INPUT_NUM];
    QLineEdit *input_lineedit[MAX_INPUT_NUM];
    QComboBox *input_box[MAX_INPUT_NUM];
    QStringList box_string[MAX_INPUT_NUM];

    QLineEdit *add_input_lineedit[MAX_INPUT_NUM];
    QPushButton *enter_button;
    QPushButton *cancel_button;
    QPushButton *next_button;
    QPushButton *clear_button;
    ClickedItem* submit_item;
    ShowItem_frame* showitemframe;

    int mark[2] = {0,0};//标记硬件
    std::vector <DDSInterfaceRelationNode *> DDSInterfacerelation_vec;
    std::vector <XSDSwitch *> switch_collection_vec;
    std::vector <XSDDevice *> device_collection_vec;

    QVBoxLayout *adjust_layout_size(QVBoxLayout *layout, int width, int height);
    void delete_layout(QLayout * p_layout);

    void set_menubar();//
    void set_toolbar();//
    void set_tree_frame();//
    void set_elements_frame();//
    void set_op_frame();//

    void set_software_element(XSDSystem *sys);
    void set_message_element(XSDSystem *sys);

    void set_message_item(XSDSystem *sys);
    void set_message_connection_item(XSDSystem *sys);
    //
    void *find_hareware(XSDSubSystem *sys, QString id, int type);
    void set_element_info(XSDSystem *sys);

    void set_sys_item(XSDSystem *sys);
    void set_subsys_item(XSDSubSystem *subsys,ClickedItem *item);
    void set_dev_item(XSDDevice *dev,ClickedItem *item);
    void set_switch_item(XSDSwitch *swi,ClickedItem *item);
    //void set_software_item(XSDSoftWare *software,ClickedItem *item);

    void add_sys_node(ClickedItem *item);//
    void add_subsystem_node(ClickedItem *item);//
    void add_switch_node(ClickedItem *item);
    void add_device_node(ClickedItem *item);
    void add_software_node(ClickedItem *item);

    void timerEvent(QTimerEvent *event);



private slots:
    //menubar & toolbar
    void create_file();
    void open_file();
    void save_file();
    void save_as_file();
    void about_this();
    void tree_widget_rightclicked(const QPoint &pos);
    void showString(QString str);
    void getItem(ClickedItem *item);
    void submit_subsystem();
    void submit_device();
    void submit_switch();
    void add_networkinterface();
    void submit_software();
    void submit_system();

    void cancel_add_node();
    //tab2
    void add_ddsconnection();
    void submit_ddsconnection();
    //tab3
    void add_software_element();
    void sumbit_software_element();

    void add_message_element();
    void submit_message_element();
    void add_struct_info();

    void changeSoftwareItem(changeName software);
    void changeMessageItem(changeName msg);

    void update_view();

private:
    MyDockWidget *mydockwidget;
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
