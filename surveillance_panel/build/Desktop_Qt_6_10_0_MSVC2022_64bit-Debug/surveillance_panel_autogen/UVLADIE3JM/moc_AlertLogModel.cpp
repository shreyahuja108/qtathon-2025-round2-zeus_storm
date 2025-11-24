/****************************************************************************
** Meta object code from reading C++ file 'AlertLogModel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AlertLogModel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AlertLogModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13AlertLogModelE_t {};
} // unnamed namespace

template <> constexpr inline auto AlertLogModel::qt_create_metaobjectdata<qt_meta_tag_ZN13AlertLogModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AlertLogModel",
        "countChanged",
        "",
        "alertAdded",
        "Alert",
        "alert",
        "addSnapshotAlert",
        "cameraName",
        "QImage",
        "image",
        "addMotionAlert",
        "message",
        "snapshotPath",
        "addRoiMotionAlert",
        "addTripwireAlert",
        "direction",
        "addLoiteringAlert",
        "clear",
        "exportToCsv",
        "filePath",
        "exportToJson",
        "exportSelectedToCsv",
        "QVariantList",
        "indices",
        "exportSelectedToJson",
        "exportSnapshotAsPng",
        "index",
        "removeAlerts",
        "removeAlert",
        "getSuggestedPngFilename",
        "count"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'alertAdded'
        QtMocHelpers::SignalData<void(const Alert &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Method 'addSnapshotAlert'
        QtMocHelpers::MethodData<void(const QString &, const QImage &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Method 'addMotionAlert'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 },
        }}),
        // Method 'addMotionAlert'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(10, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 11 },
        }}),
        // Method 'addMotionAlert'
        QtMocHelpers::MethodData<void(const QString &)>(10, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Method 'addRoiMotionAlert'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 },
        }}),
        // Method 'addRoiMotionAlert'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(13, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 11 },
        }}),
        // Method 'addTripwireAlert'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &, int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 }, { QMetaType::Int, 15 },
        }}),
        // Method 'addTripwireAlert'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &)>(14, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 },
        }}),
        // Method 'addTripwireAlert'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(14, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 11 },
        }}),
        // Method 'addLoiteringAlert'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 11 }, { QMetaType::QString, 12 },
        }}),
        // Method 'addLoiteringAlert'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(16, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 11 },
        }}),
        // Method 'clear'
        QtMocHelpers::MethodData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'exportToCsv'
        QtMocHelpers::MethodData<bool(const QString &)>(18, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 19 },
        }}),
        // Method 'exportToJson'
        QtMocHelpers::MethodData<bool(const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 19 },
        }}),
        // Method 'exportSelectedToCsv'
        QtMocHelpers::MethodData<bool(const QString &, const QVariantList &)>(21, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 19 }, { 0x80000000 | 22, 23 },
        }}),
        // Method 'exportSelectedToJson'
        QtMocHelpers::MethodData<bool(const QString &, const QVariantList &)>(24, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 19 }, { 0x80000000 | 22, 23 },
        }}),
        // Method 'exportSnapshotAsPng'
        QtMocHelpers::MethodData<bool(int, const QString &)>(25, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 26 }, { QMetaType::QString, 19 },
        }}),
        // Method 'removeAlerts'
        QtMocHelpers::MethodData<void(const QVariantList &)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 23 },
        }}),
        // Method 'removeAlert'
        QtMocHelpers::MethodData<void(int)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 },
        }}),
        // Method 'getSuggestedPngFilename'
        QtMocHelpers::MethodData<QString(int) const>(29, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 26 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'count'
        QtMocHelpers::PropertyData<int>(30, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AlertLogModel, qt_meta_tag_ZN13AlertLogModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AlertLogModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AlertLogModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AlertLogModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13AlertLogModelE_t>.metaTypes,
    nullptr
} };

void AlertLogModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AlertLogModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->countChanged(); break;
        case 1: _t->alertAdded((*reinterpret_cast<std::add_pointer_t<Alert>>(_a[1]))); break;
        case 2: _t->addSnapshotAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QImage>>(_a[2]))); break;
        case 3: _t->addMotionAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 4: _t->addMotionAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->addMotionAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->addRoiMotionAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 7: _t->addRoiMotionAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 8: _t->addTripwireAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 9: _t->addTripwireAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 10: _t->addTripwireAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->addLoiteringAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 12: _t->addLoiteringAlert((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->clear(); break;
        case 14: { bool _r = _t->exportToCsv((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { bool _r = _t->exportToJson((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 16: { bool _r = _t->exportSelectedToCsv((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 17: { bool _r = _t->exportSelectedToJson((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 18: { bool _r = _t->exportSnapshotAsPng((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 19: _t->removeAlerts((*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[1]))); break;
        case 20: _t->removeAlert((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 21: { QString _r = _t->getSuggestedPngFilename((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AlertLogModel::*)()>(_a, &AlertLogModel::countChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AlertLogModel::*)(const Alert & )>(_a, &AlertLogModel::alertAdded, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->rowCount(); break;
        default: break;
        }
    }
}

const QMetaObject *AlertLogModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AlertLogModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AlertLogModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int AlertLogModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 22;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void AlertLogModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AlertLogModel::alertAdded(const Alert & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
