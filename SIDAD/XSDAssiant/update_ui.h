#ifndef UPDATE_UI_H
#define UPDATE_UI_H
#include <QObject>

class update_ui :public QObject
{
    Q_OBJECT
public:
   static update_ui *get_instance();
private:
    update_ui();
    static update_ui * _instance;
signals:
    void send_update_signal();
};

#endif // UPDATE_UI_H
