/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/qt_plot/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ROS2_QT5_t {
    QByteArrayData data[13];
    char stringdata0[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ROS2_QT5_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ROS2_QT5_t qt_meta_stringdata_ROS2_QT5 = {
    {
QT_MOC_LITERAL(0, 0, 8), // "ROS2_QT5"
QT_MOC_LITERAL(1, 9, 16), // "sendMessage2Plot"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "vector<float>"
QT_MOC_LITERAL(4, 41, 13), // "speed_command"
QT_MOC_LITERAL(5, 55, 7), // "error_x"
QT_MOC_LITERAL(6, 63, 7), // "error_y"
QT_MOC_LITERAL(7, 71, 13), // "error_squmean"
QT_MOC_LITERAL(8, 85, 16), // "sendMessageImage"
QT_MOC_LITERAL(9, 102, 7), // "cv::Mat"
QT_MOC_LITERAL(10, 110, 11), // "color_image"
QT_MOC_LITERAL(11, 122, 11), // "depth_image"
QT_MOC_LITERAL(12, 134, 8) // "ros2_run"

    },
    "ROS2_QT5\0sendMessage2Plot\0\0vector<float>\0"
    "speed_command\0error_x\0error_y\0"
    "error_squmean\0sendMessageImage\0cv::Mat\0"
    "color_image\0depth_image\0ros2_run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ROS2_QT5[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   29,    2, 0x06 /* Public */,
       8,    2,   38,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, QMetaType::Float,    4,    5,    6,    7,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9,   10,   11,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ROS2_QT5::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ROS2_QT5 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMessage2Plot((*reinterpret_cast< vector<float>(*)>(_a[1])),(*reinterpret_cast< vector<float>(*)>(_a[2])),(*reinterpret_cast< vector<float>(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 1: _t->sendMessageImage((*reinterpret_cast< cv::Mat(*)>(_a[1])),(*reinterpret_cast< cv::Mat(*)>(_a[2]))); break;
        case 2: _t->ros2_run(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ROS2_QT5::*)(vector<float> , vector<float> , vector<float> , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ROS2_QT5::sendMessage2Plot)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ROS2_QT5::*)(cv::Mat , cv::Mat );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ROS2_QT5::sendMessageImage)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ROS2_QT5::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ROS2_QT5.data,
    qt_meta_data_ROS2_QT5,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ROS2_QT5::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ROS2_QT5::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ROS2_QT5.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ROS2_QT5::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ROS2_QT5::sendMessage2Plot(vector<float> _t1, vector<float> _t2, vector<float> _t3, float _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ROS2_QT5::sendMessageImage(cv::Mat _t1, cv::Mat _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[209];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 15), // "ros2_run_signal"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 16), // "realtimeDataSlot"
QT_MOC_LITERAL(4, 45, 18), // "addQCustomPlotData"
QT_MOC_LITERAL(5, 64, 13), // "vector<float>"
QT_MOC_LITERAL(6, 78, 13), // "speed_command"
QT_MOC_LITERAL(7, 92, 7), // "error_x"
QT_MOC_LITERAL(8, 100, 7), // "error_y"
QT_MOC_LITERAL(9, 108, 13), // "error_squmean"
QT_MOC_LITERAL(10, 122, 13), // "saveCurseData"
QT_MOC_LITERAL(11, 136, 14), // "saveCurseImage"
QT_MOC_LITERAL(12, 151, 13), // "clearPlotData"
QT_MOC_LITERAL(13, 165, 11), // "importFrame"
QT_MOC_LITERAL(14, 177, 7), // "cv::Mat"
QT_MOC_LITERAL(15, 185, 11), // "color_image"
QT_MOC_LITERAL(16, 197, 11) // "depth_image"

    },
    "MainWindow\0ros2_run_signal\0\0"
    "realtimeDataSlot\0addQCustomPlotData\0"
    "vector<float>\0speed_command\0error_x\0"
    "error_y\0error_squmean\0saveCurseData\0"
    "saveCurseImage\0clearPlotData\0importFrame\0"
    "cv::Mat\0color_image\0depth_image"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x08 /* Private */,
       4,    4,   51,    2, 0x08 /* Private */,
      10,    0,   60,    2, 0x08 /* Private */,
      11,    0,   61,    2, 0x08 /* Private */,
      12,    0,   62,    2, 0x08 /* Private */,
      13,    2,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 5, QMetaType::Float,    6,    7,    8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 14,   15,   16,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ros2_run_signal(); break;
        case 1: _t->realtimeDataSlot(); break;
        case 2: _t->addQCustomPlotData((*reinterpret_cast< vector<float>(*)>(_a[1])),(*reinterpret_cast< vector<float>(*)>(_a[2])),(*reinterpret_cast< vector<float>(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 3: _t->saveCurseData(); break;
        case 4: _t->saveCurseImage(); break;
        case 5: _t->clearPlotData(); break;
        case 6: _t->importFrame((*reinterpret_cast< cv::Mat(*)>(_a[1])),(*reinterpret_cast< cv::Mat(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::ros2_run_signal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::ros2_run_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
