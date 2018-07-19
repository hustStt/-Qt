#include "showitem_frame.h"

ShowItem_frame * ShowItem_frame::_instance = NULL;

ShowItem_frame * ShowItem_frame::get_instance()
{
    if(_instance == NULL)
    {
        _instance = new ShowItem_frame();
    }
    return _instance;
}


ShowItem_frame::ShowItem_frame()
{

}

void ShowItem_frame::ShowItem(ClickedItem *Item){

}

void ShowItem_frame::ClearShowItem(){

}

void ShowItem_frame::changeItem(){

}
