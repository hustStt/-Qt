#include "update_ui.h"

update_ui* update_ui::_instance = NULL;

update_ui::update_ui()
{

}

update_ui * update_ui::get_instance(){
    if(_instance == NULL)
    {
        _instance = new update_ui();
    }
    return _instance;
}
