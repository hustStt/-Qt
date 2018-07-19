#include "rightclickedmenu.h"

//QMenu RightClickedMenu::right_clicked_menu;

RightClickedMenu::RightClickedMenu(QObject *parent)
{
    right_clicked_menu = new QMenu();
    for(int i = 0;i < 5;++i)menu_action[i] = nullptr;
}


