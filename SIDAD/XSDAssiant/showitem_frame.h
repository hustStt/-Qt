#ifndef SHOWITEM_FRAME_H
#define SHOWITEM_FRAME_H
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QLayout>
#include <QComboBox>
#include <clickeditem.h>
#include <type.h>

class ShowItem_frame:public QFrame
{
    Q_OBJECT
public slots:
    void changeItem();
public:
    static ShowItem_frame * get_instance();
    void ClearShowItem();
    void ShowItem(ClickedItem *Item);

    QLabel *input_label[MAX_INPUT_NUM];
    QLineEdit *input_lineedit[MAX_INPUT_NUM];
    QComboBox *input_box[MAX_INPUT_NUM];
    QStringList box_string[MAX_INPUT_NUM];

    QLineEdit *add_input_lineedit[MAX_INPUT_NUM];
    QPushButton *enter_button;
    QPushButton *cancel_button;
    QPushButton *next_button;
    QPushButton *clear_button;

private:
    static ShowItem_frame * _instance;
    ClickedItem* _item;
    ShowItem_frame();
};


#endif // SHOWITEM_FRAME_H
