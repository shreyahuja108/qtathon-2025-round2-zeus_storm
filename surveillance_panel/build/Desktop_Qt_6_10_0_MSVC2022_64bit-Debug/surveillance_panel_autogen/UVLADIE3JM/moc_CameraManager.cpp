/****************************************************************************
** Meta object code from reading C++ file 'CameraManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/CameraManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CameraManager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13CameraManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto CameraManager::qt_create_metaobjectdata<qt_meta_tag_ZN13CameraManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CameraManager",
        "roiChanged",
        "",
        "index",
        "tripwireChanged",
        "cameraName",
        "cameraAvailable",
        "cameraType",
        "cameraSource",
        "roiPoints",
        "QVariantList",
        "hasRoi",
        "setRoi",
        "points",
        "clearRoi",
        "tripwire",
        "QVariantMap",
        "hasTripwire",
        "setTripwire",
        "startX",
        "startY",
        "endX",
        "endY",
        "clearTripwire",
        "camera1",
        "camera2",
        "camera3",
        "camera4"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'roiChanged'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'tripwireChanged'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'cameraName'
        QtMocHelpers::MethodData<QString(int) const>(5, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'cameraAvailable'
        QtMocHelpers::MethodData<bool(int) const>(6, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'cameraType'
        QtMocHelpers::MethodData<QString(int) const>(7, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'cameraSource'
        QtMocHelpers::MethodData<QString(int) const>(8, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'roiPoints'
        QtMocHelpers::MethodData<QVariantList(int) const>(9, 2, QMC::AccessPublic, 0x80000000 | 10, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'hasRoi'
        QtMocHelpers::MethodData<bool(int) const>(11, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'setRoi'
        QtMocHelpers::MethodData<void(int, const QVariantList &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { 0x80000000 | 10, 13 },
        }}),
        // Method 'clearRoi'
        QtMocHelpers::MethodData<void(int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'tripwire'
        QtMocHelpers::MethodData<QVariantMap(int) const>(15, 2, QMC::AccessPublic, 0x80000000 | 16, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'hasTripwire'
        QtMocHelpers::MethodData<bool(int) const>(17, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 3 },
        }}),
        // Method 'setTripwire'
        QtMocHelpers::MethodData<void(int, double, double, double, double)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Double, 19 }, { QMetaType::Double, 20 }, { QMetaType::Double, 21 },
            { QMetaType::Double, 22 },
        }}),
        // Method 'clearTripwire'
        QtMocHelpers::MethodData<void(int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'camera1'
        QtMocHelpers::PropertyData<QObject*>(24, QMetaType::QObjectStar, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'camera2'
        QtMocHelpers::PropertyData<QObject*>(25, QMetaType::QObjectStar, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'camera3'
        QtMocHelpers::PropertyData<QObject*>(26, QMetaType::QObjectStar, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'camera4'
        QtMocHelpers::PropertyData<QObject*>(27, QMetaType::QObjectStar, QMC::DefaultPropertyFlags | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CameraManager, qt_meta_tag_ZN13CameraManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CameraManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CameraManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CameraManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13CameraManagerE_t>.metaTypes,
    nullptr
} };

void CameraManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CameraManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->roiChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->tripwireChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: { QString _r = _t->cameraName((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->cameraAvailable((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->cameraType((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->cameraSource((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 6: { QVariantList _r = _t->roiPoints((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->hasRoi((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->setRoi((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[2]))); break;
        case 9: _t->clearRoi((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 10: { QVariantMap _r = _t->tripwire((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 11: { bool _r = _t->hasTripwire((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->setTripwire((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5]))); break;
        case 13: _t->clearTripwire((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CameraManager::*)(int )>(_a, &CameraManager::roiChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraManager::*)(int )>(_a, &CameraManager::tripwireChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QObject**>(_v) = _t->camera1(); break;
        case 1: *reinterpret_cast<QObject**>(_v) = _t->camera2(); break;
        case 2: *reinterpret_cast<QObject**>(_v) = _t->camera3(); break;
        case 3: *reinterpret_cast<QObject**>(_v) = _t->camera4(); break;
        default: break;
        }
    }
}

const QMetaObject *CameraManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CameraManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CameraManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CameraManager::roiChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void CameraManager::tripwireChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
