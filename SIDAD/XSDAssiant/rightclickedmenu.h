#ifndef RIGHTCLICKEDMENU_H
#define RIGHTCLICKEDMENU_H

#include <QObject>
#include <QMenu>
#include <QWidget>
#include <QDebug>

using namespace std;

class RightClickedMenu : public QObject
{
    Q_OBJECT
public:
    RightClickedMenu(QObject *parent = nullptr);
    QMenu *right_clicked_menu;
    QAction* menu_action[5];
    void ShowMenu(const QPoint &pos = QCursor::pos(),QWidget *parent = nullptr)
    {
        right_clicked_menu->exec(pos);
    }
    QAction* addAction(int i,QString qstr)
    {
        if(menu_action[i] != nullptr)
        {
            qDebug()<<qstr;
            menu_action[i]->setText(qstr);
        }
        else
            menu_action[i] = right_clicked_menu->addAction(qstr);
        return menu_action[i];

    }
};

#endif // RIGHTCLICKEDMENU_H
