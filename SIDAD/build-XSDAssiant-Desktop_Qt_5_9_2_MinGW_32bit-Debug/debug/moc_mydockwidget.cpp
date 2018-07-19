/****************************************************************************
** Meta object code from reading C++ file 'mydockwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../XSDAssiant/mydockwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mydockwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyDockWidget_t {
    QByteArrayData data[14];
    char stringdata0[176];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyDockWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyDockWidget_t qt_meta_stringdata_MyDockWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MyDockWidget"
QT_MOC_LITERAL(1, 13, 16), // "sendSoftwareItem"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10), // "changeName"
QT_MOC_LITERAL(4, 42, 8), // "software"
QT_MOC_LITERAL(5, 51, 15), // "sendMessageItem"
QT_MOC_LITERAL(6, 67, 3), // "msg"
QT_MOC_LITERAL(7, 71, 10), // "changeItem"
QT_MOC_LITERAL(8, 82, 16), // "show_struct_info"
QT_MOC_LITERAL(9, 99, 14), // "show_interface"
QT_MOC_LITERAL(10, 114, 15), // "add_struct_info"
QT_MOC_LITERAL(11, 130, 16), // "save_struct_info"
QT_MOC_LITERAL(12, 147, 13), // "add_interface"
QT_MOC_LITERAL(13, 161, 14) // "save_interface"

    },
    "MyDockWidget\0sendSoftwareItem\0\0"
    "changeName\0software\0sendMessageItem\0"
    "msg\0changeItem\0show_struct_info\0"
    "show_interface\0add_struct_info\0"
    "save_struct_info\0add_interface\0"
    "save_interface"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyDockWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    1,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   65,    2, 0x0a /* Public */,
       8,    0,   66,    2, 0x0a /* Public */,
       9,    0,   67,    2, 0x0a /* Public */,
      10,    0,   68,    2, 0x0a /* Public */,
      11,    0,   69,    2, 0x0a /* Public */,
      12,    0,   70,    2, 0x0a /* Public */,
      13,    0,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MyDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyDockWidget *_t = static_cast<MyDockWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendSoftwareItem((*reinterpret_cast< changeName(*)>(_a[1]))); break;
        case 1: _t->sendMessageItem((*reinterpret_cast< changeName(*)>(_a[1]))); break;
        case 2: _t->changeItem(); break;
        case 3: _t->show_struct_info(); break;
        case 4: _t->show_interface(); break;
        case 5: _t->add_struct_info(); break;
        case 6: _t->save_struct_info(); break;
        case 7: _t->add_interface(); break;
        case 8: _t->save_interface(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyDockWidget::*_t)(changeName );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyDockWidget::sendSoftwareItem)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MyDockWidget::*_t)(changeName );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyDockWidget::sendMessageItem)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MyDockWidget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_MyDockWidget.data,
      qt_meta_data_MyDockWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MyDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyDockWidget.stringdata0))
        return static_cast<void*>(this);
    return QDockWidget::qt_metacast(_clname);
}

int MyDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void MyDockWidget::sendSoftwareItem(changeName _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyDockWidget::sendMessageItem(changeName _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
