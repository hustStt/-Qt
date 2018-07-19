#ifndef MYDOCKWIDGET_H
#define MYDOCKWIDGET_H
#include <QDockWidget>
#include "clickeditem.h"
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <xsd_type.h>

class MyDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    static MyDockWidget * get_instance();
    void set_data(ClickedItem *item);
    void show_data();
    void clear_data();
    void del_layout();
    void Controls_initial();

    void show_sys(XSDSystem *sys);
    void show_subsys(XSDSubSystem *subsys);
    void show_device(XSDDevice *dev);
    void show_switch(XSDSwitch *swi);
    void show_software(XSDSoftWare *software);
    void show_message(Msg *msg);
    void show_connection(SubSoftwareConnection *conn);
    void show_dds();

    void change_sys(XSDSystem *sys);
    void change_subsys(XSDSubSystem *subsys);
    void change_device(XSDDevice *dev);
    void change_switch(XSDSwitch *swi);
    void change_software(XSDSoftWare *software);
    void change_message(Msg *msg);
    void change_connection(SubSoftwareConnection *conn);
    void change_dds();

    QFrame *myframe;
    QWidget *ss;
    QVBoxLayout *op_stack_layout;
    QLabel *input_label[MAX_INPUT_NUM];
    QLineEdit *input_lineedit[MAX_INPUT_NUM];
    QComboBox *input_box[MAX_INPUT_NUM];

    QLineEdit *add_input_lineedit[MAX_INPUT_NUM];
    QPushButton *save_button;
    QPushButton *next_button;
public slots:
    void changeItem();
    void show_struct_info();
    void show_interface();
    void add_struct_info();
    void save_struct_info();
    void add_interface();
    void save_interface();
signals:
    void sendSoftwareItem(changeName software);
    void sendMessageItem(changeName msg);

private:
    MyDockWidget();
    ClickedItem* item;
    static MyDockWidget * _instance;
    int count = 0;
    bool swi = false;
};

#endif // MYDOCKWIDGET_H
