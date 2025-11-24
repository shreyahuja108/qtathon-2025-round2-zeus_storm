/****************************************************************************
** Meta object code from reading C++ file 'CameraStream.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/CameraStream.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CameraStream.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13CaptureWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto CaptureWorker::qt_create_metaobjectdata<qt_meta_tag_ZN13CaptureWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CaptureWorker",
        "frameCaptured",
        "",
        "QImage",
        "frame",
        "fpsUpdated",
        "fps",
        "errorOccurred",
        "error",
        "motionDetected",
        "score",
        "cv::Mat",
        "roiMotionDetected",
        "tripwireCrossed",
        "direction",
        "aiDetectionsReady",
        "std::vector<Detection>",
        "detections",
        "trackCrossedTripwire",
        "trackId",
        "label",
        "loiteringDetected",
        "durationMs",
        "start",
        "stop",
        "captureFrame",
        "setMotionEnabled",
        "enabled",
        "setMotionSensitivity",
        "sensitivity",
        "setRoiPolygon",
        "QList<QPointF>",
        "normalizedPoints",
        "clearRoi",
        "setTripwire",
        "QPointF",
        "startNorm",
        "endNorm",
        "clearTripwire",
        "setAiEnabled",
        "setAiConfidenceThreshold",
        "threshold",
        "setObjectDetector",
        "ObjectDetector*",
        "detector"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'frameCaptured'
        QtMocHelpers::SignalData<void(const QImage &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'fpsUpdated'
        QtMocHelpers::SignalData<void(double)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 6 },
        }}),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Signal 'motionDetected'
        QtMocHelpers::SignalData<void(double, const cv::Mat &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 10 }, { 0x80000000 | 11, 4 },
        }}),
        // Signal 'roiMotionDetected'
        QtMocHelpers::SignalData<void(double, const cv::Mat &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 10 }, { 0x80000000 | 11, 4 },
        }}),
        // Signal 'tripwireCrossed'
        QtMocHelpers::SignalData<void(int, const cv::Mat &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 14 }, { 0x80000000 | 11, 4 },
        }}),
        // Signal 'aiDetectionsReady'
        QtMocHelpers::SignalData<void(const std::vector<Detection> &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Signal 'trackCrossedTripwire'
        QtMocHelpers::SignalData<void(int, const QString &, const QString &, const cv::Mat &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 19 }, { QMetaType::QString, 20 }, { QMetaType::QString, 14 }, { 0x80000000 | 11, 4 },
        }}),
        // Signal 'loiteringDetected'
        QtMocHelpers::SignalData<void(int, const QString &, qint64, const cv::Mat &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 19 }, { QMetaType::QString, 20 }, { QMetaType::LongLong, 22 }, { 0x80000000 | 11, 4 },
        }}),
        // Slot 'start'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'stop'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'captureFrame'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setMotionEnabled'
        QtMocHelpers::SlotData<void(bool)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 27 },
        }}),
        // Slot 'setMotionSensitivity'
        QtMocHelpers::SlotData<void(double)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 29 },
        }}),
        // Slot 'setRoiPolygon'
        QtMocHelpers::SlotData<void(const QVector<QPointF> &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 31, 32 },
        }}),
        // Slot 'clearRoi'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setTripwire'
        QtMocHelpers::SlotData<void(const QPointF &, const QPointF &)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 35, 36 }, { 0x80000000 | 35, 37 },
        }}),
        // Slot 'clearTripwire'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setAiEnabled'
        QtMocHelpers::SlotData<void(bool)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 27 },
        }}),
        // Slot 'setAiConfidenceThreshold'
        QtMocHelpers::SlotData<void(double)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 41 },
        }}),
        // Slot 'setObjectDetector'
        QtMocHelpers::SlotData<void(ObjectDetector *)>(42, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 43, 44 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CaptureWorker, qt_meta_tag_ZN13CaptureWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CaptureWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CaptureWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CaptureWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13CaptureWorkerE_t>.metaTypes,
    nullptr
} };

void CaptureWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CaptureWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->frameCaptured((*reinterpret_cast<std::add_pointer_t<QImage>>(_a[1]))); break;
        case 1: _t->fpsUpdated((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 2: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->motionDetected((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[2]))); break;
        case 4: _t->roiMotionDetected((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[2]))); break;
        case 5: _t->tripwireCrossed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[2]))); break;
        case 6: _t->aiDetectionsReady((*reinterpret_cast<std::add_pointer_t<std::vector<Detection>>>(_a[1]))); break;
        case 7: _t->trackCrossedTripwire((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[4]))); break;
        case 8: _t->loiteringDetected((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[4]))); break;
        case 9: _t->start(); break;
        case 10: _t->stop(); break;
        case 11: _t->captureFrame(); break;
        case 12: _t->setMotionEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 13: _t->setMotionSensitivity((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 14: _t->setRoiPolygon((*reinterpret_cast<std::add_pointer_t<QList<QPointF>>>(_a[1]))); break;
        case 15: _t->clearRoi(); break;
        case 16: _t->setTripwire((*reinterpret_cast<std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QPointF>>(_a[2]))); break;
        case 17: _t->clearTripwire(); break;
        case 18: _t->setAiEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 19: _t->setAiConfidenceThreshold((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 20: _t->setObjectDetector((*reinterpret_cast<std::add_pointer_t<ObjectDetector*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CaptureWorker::*)(const QImage & )>(_a, &CaptureWorker::frameCaptured, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CaptureWorker::*)(double )>(_a, &CaptureWorker::fpsUpdated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CaptureWorker::*)(const QString & )>(_a, &CaptureWorker::errorOccurred, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (CaptureWorker::*)(double , const cv::Mat & )>(_a, &CaptureWorker::motionDetected, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (CaptureWorker::*)(double , const cv::Mat & )>(_a, &CaptureWorker::roiMotionDetected, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (CaptureWorker::*)(int , const cv::Mat & )>(_a, &CaptureWorker::tripwireCrossed, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (CaptureWorker::*)(const std::vector<Detection> & )>(_a, &CaptureWorker::aiDetectionsReady, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (CaptureWorker::*)(int , const QString & , const QString & , const cv::Mat & )>(_a, &CaptureWorker::trackCrossedTripwire, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (CaptureWorker::*)(int , const QString & , qint64 , const cv::Mat & )>(_a, &CaptureWorker::loiteringDetected, 8))
            return;
    }
}

const QMetaObject *CaptureWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CaptureWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13CaptureWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CaptureWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void CaptureWorker::frameCaptured(const QImage & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void CaptureWorker::fpsUpdated(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void CaptureWorker::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void CaptureWorker::motionDetected(double _t1, const cv::Mat & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void CaptureWorker::roiMotionDetected(double _t1, const cv::Mat & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void CaptureWorker::tripwireCrossed(int _t1, const cv::Mat & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void CaptureWorker::aiDetectionsReady(const std::vector<Detection> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void CaptureWorker::trackCrossedTripwire(int _t1, const QString & _t2, const QString & _t3, const cv::Mat & _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 8
void CaptureWorker::loiteringDetected(int _t1, const QString & _t2, qint64 _t3, const cv::Mat & _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2, _t3, _t4);
}
namespace {
struct qt_meta_tag_ZN12CameraStreamE_t {};
} // unnamed namespace

template <> constexpr inline auto CameraStream::qt_create_metaobjectdata<qt_meta_tag_ZN12CameraStreamE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CameraStream",
        "frameChanged",
        "",
        "runningChanged",
        "fpsChanged",
        "statusChanged",
        "cameraNameChanged",
        "snapshotSaved",
        "filePath",
        "snapshotFailed",
        "reason",
        "motionEnabledChanged",
        "motionSensitivityChanged",
        "motionActiveChanged",
        "motionDetected",
        "score",
        "roiMotionDetected",
        "tripwireCrossed",
        "direction",
        "trackCrossedTripwire",
        "trackId",
        "label",
        "loiteringDetected",
        "durationMs",
        "roiAlertActiveChanged",
        "tripwireAlertActiveChanged",
        "aiEnabledChanged",
        "aiConfidenceThresholdChanged",
        "detectionsChanged",
        "snapshotCaptured",
        "QImage",
        "image",
        "autoSnapshotOnMotionChanged",
        "autoSnapshotOnRoiChanged",
        "autoSnapshotOnTripwireChanged",
        "onFrameCaptured",
        "frame",
        "onFpsUpdated",
        "fps",
        "onErrorOccurred",
        "error",
        "onMotionDetected",
        "cv::Mat",
        "onRoiMotionDetected",
        "onTripwireCrossed",
        "onTrackCrossedTripwire",
        "onLoiteringDetected",
        "onAIDetectionsReady",
        "std::vector<Detection>",
        "detections",
        "resetMotionActive",
        "resetRoiAlertActive",
        "resetTripwireAlertActive",
        "start",
        "stop",
        "setSource",
        "cameraIndex",
        "takeSnapshot",
        "saveSnapshot",
        "targetDir",
        "id",
        "source",
        "sourceType",
        "running",
        "status",
        "cameraName",
        "motionEnabled",
        "motionSensitivity",
        "motionActive",
        "roiAlertActive",
        "tripwireAlertActive",
        "aiEnabled",
        "aiConfidenceThreshold",
        "QVariantList",
        "autoSnapshotOnMotion",
        "autoSnapshotOnRoi",
        "autoSnapshotOnTripwire"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'frameChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'runningChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'fpsChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'statusChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cameraNameChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'snapshotSaved'
        QtMocHelpers::SignalData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Signal 'snapshotFailed'
        QtMocHelpers::SignalData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'motionEnabledChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'motionSensitivityChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'motionActiveChanged'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'motionDetected'
        QtMocHelpers::SignalData<void(double)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 15 },
        }}),
        // Signal 'roiMotionDetected'
        QtMocHelpers::SignalData<void(double)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 15 },
        }}),
        // Signal 'tripwireCrossed'
        QtMocHelpers::SignalData<void(int)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Signal 'trackCrossedTripwire'
        QtMocHelpers::SignalData<void(int, const QString &, const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 20 }, { QMetaType::QString, 21 }, { QMetaType::QString, 18 },
        }}),
        // Signal 'loiteringDetected'
        QtMocHelpers::SignalData<void(int, const QString &, qint64)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 20 }, { QMetaType::QString, 21 }, { QMetaType::LongLong, 23 },
        }}),
        // Signal 'roiAlertActiveChanged'
        QtMocHelpers::SignalData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'tripwireAlertActiveChanged'
        QtMocHelpers::SignalData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'aiEnabledChanged'
        QtMocHelpers::SignalData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'aiConfidenceThresholdChanged'
        QtMocHelpers::SignalData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'detectionsChanged'
        QtMocHelpers::SignalData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'snapshotCaptured'
        QtMocHelpers::SignalData<void(const QImage &)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 30, 31 },
        }}),
        // Signal 'autoSnapshotOnMotionChanged'
        QtMocHelpers::SignalData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'autoSnapshotOnRoiChanged'
        QtMocHelpers::SignalData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'autoSnapshotOnTripwireChanged'
        QtMocHelpers::SignalData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onFrameCaptured'
        QtMocHelpers::SlotData<void(const QImage &)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 30, 36 },
        }}),
        // Slot 'onFpsUpdated'
        QtMocHelpers::SlotData<void(double)>(37, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 38 },
        }}),
        // Slot 'onErrorOccurred'
        QtMocHelpers::SlotData<void(const QString &)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 40 },
        }}),
        // Slot 'onMotionDetected'
        QtMocHelpers::SlotData<void(double, const cv::Mat &)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 15 }, { 0x80000000 | 42, 36 },
        }}),
        // Slot 'onRoiMotionDetected'
        QtMocHelpers::SlotData<void(double, const cv::Mat &)>(43, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 15 }, { 0x80000000 | 42, 36 },
        }}),
        // Slot 'onTripwireCrossed'
        QtMocHelpers::SlotData<void(int, const cv::Mat &)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { 0x80000000 | 42, 36 },
        }}),
        // Slot 'onTrackCrossedTripwire'
        QtMocHelpers::SlotData<void(int, const QString &, const QString &, const cv::Mat &)>(45, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 20 }, { QMetaType::QString, 21 }, { QMetaType::QString, 18 }, { 0x80000000 | 42, 36 },
        }}),
        // Slot 'onLoiteringDetected'
        QtMocHelpers::SlotData<void(int, const QString &, qint64, const cv::Mat &)>(46, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 20 }, { QMetaType::QString, 21 }, { QMetaType::LongLong, 23 }, { 0x80000000 | 42, 36 },
        }}),
        // Slot 'onAIDetectionsReady'
        QtMocHelpers::SlotData<void(const std::vector<Detection> &)>(47, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 48, 49 },
        }}),
        // Slot 'resetMotionActive'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'resetRoiAlertActive'
        QtMocHelpers::SlotData<void()>(51, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'resetTripwireAlertActive'
        QtMocHelpers::SlotData<void()>(52, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'start'
        QtMocHelpers::MethodData<void()>(53, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stop'
        QtMocHelpers::MethodData<void()>(54, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'setSource'
        QtMocHelpers::MethodData<void(int)>(55, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 56 },
        }}),
        // Method 'takeSnapshot'
        QtMocHelpers::MethodData<bool()>(57, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'saveSnapshot'
        QtMocHelpers::MethodData<bool(const QString &)>(58, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 59 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'id'
        QtMocHelpers::PropertyData<QString>(60, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'source'
        QtMocHelpers::PropertyData<QString>(61, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'sourceType'
        QtMocHelpers::PropertyData<QString>(62, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'frame'
        QtMocHelpers::PropertyData<QImage>(36, 0x80000000 | 30, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'running'
        QtMocHelpers::PropertyData<bool>(63, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'fps'
        QtMocHelpers::PropertyData<double>(38, QMetaType::Double, QMC::DefaultPropertyFlags, 2),
        // property 'status'
        QtMocHelpers::PropertyData<QString>(64, QMetaType::QString, QMC::DefaultPropertyFlags, 3),
        // property 'cameraName'
        QtMocHelpers::PropertyData<QString>(65, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'motionEnabled'
        QtMocHelpers::PropertyData<bool>(66, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
        // property 'motionSensitivity'
        QtMocHelpers::PropertyData<double>(67, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 8),
        // property 'motionActive'
        QtMocHelpers::PropertyData<bool>(68, QMetaType::Bool, QMC::DefaultPropertyFlags, 9),
        // property 'roiAlertActive'
        QtMocHelpers::PropertyData<bool>(69, QMetaType::Bool, QMC::DefaultPropertyFlags, 15),
        // property 'tripwireAlertActive'
        QtMocHelpers::PropertyData<bool>(70, QMetaType::Bool, QMC::DefaultPropertyFlags, 16),
        // property 'aiEnabled'
        QtMocHelpers::PropertyData<bool>(71, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 17),
        // property 'aiConfidenceThreshold'
        QtMocHelpers::PropertyData<double>(72, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 18),
        // property 'detections'
        QtMocHelpers::PropertyData<QVariantList>(49, 0x80000000 | 73, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 19),
        // property 'autoSnapshotOnMotion'
        QtMocHelpers::PropertyData<bool>(74, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 21),
        // property 'autoSnapshotOnRoi'
        QtMocHelpers::PropertyData<bool>(75, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 22),
        // property 'autoSnapshotOnTripwire'
        QtMocHelpers::PropertyData<bool>(76, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 23),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CameraStream, qt_meta_tag_ZN12CameraStreamE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CameraStream::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12CameraStreamE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12CameraStreamE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12CameraStreamE_t>.metaTypes,
    nullptr
} };

void CameraStream::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CameraStream *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->frameChanged(); break;
        case 1: _t->runningChanged(); break;
        case 2: _t->fpsChanged(); break;
        case 3: _t->statusChanged(); break;
        case 4: _t->cameraNameChanged(); break;
        case 5: _t->snapshotSaved((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->snapshotFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->motionEnabledChanged(); break;
        case 8: _t->motionSensitivityChanged(); break;
        case 9: _t->motionActiveChanged(); break;
        case 10: _t->motionDetected((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 11: _t->roiMotionDetected((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 12: _t->tripwireCrossed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->trackCrossedTripwire((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 14: _t->loiteringDetected((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[3]))); break;
        case 15: _t->roiAlertActiveChanged(); break;
        case 16: _t->tripwireAlertActiveChanged(); break;
        case 17: _t->aiEnabledChanged(); break;
        case 18: _t->aiConfidenceThresholdChanged(); break;
        case 19: _t->detectionsChanged(); break;
        case 20: _t->snapshotCaptured((*reinterpret_cast<std::add_pointer_t<QImage>>(_a[1]))); break;
        case 21: _t->autoSnapshotOnMotionChanged(); break;
        case 22: _t->autoSnapshotOnRoiChanged(); break;
        case 23: _t->autoSnapshotOnTripwireChanged(); break;
        case 24: _t->onFrameCaptured((*reinterpret_cast<std::add_pointer_t<QImage>>(_a[1]))); break;
        case 25: _t->onFpsUpdated((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 26: _t->onErrorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 27: _t->onMotionDetected((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[2]))); break;
        case 28: _t->onRoiMotionDetected((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[2]))); break;
        case 29: _t->onTripwireCrossed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[2]))); break;
        case 30: _t->onTrackCrossedTripwire((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[4]))); break;
        case 31: _t->onLoiteringDetected((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<cv::Mat>>(_a[4]))); break;
        case 32: _t->onAIDetectionsReady((*reinterpret_cast<std::add_pointer_t<std::vector<Detection>>>(_a[1]))); break;
        case 33: _t->resetMotionActive(); break;
        case 34: _t->resetRoiAlertActive(); break;
        case 35: _t->resetTripwireAlertActive(); break;
        case 36: _t->start(); break;
        case 37: _t->stop(); break;
        case 38: _t->setSource((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 39: { bool _r = _t->takeSnapshot();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 40: { bool _r = _t->saveSnapshot((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::frameChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::runningChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::fpsChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::statusChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::cameraNameChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)(const QString & )>(_a, &CameraStream::snapshotSaved, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)(const QString & )>(_a, &CameraStream::snapshotFailed, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::motionEnabledChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::motionSensitivityChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::motionActiveChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)(double )>(_a, &CameraStream::motionDetected, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)(double )>(_a, &CameraStream::roiMotionDetected, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)(int )>(_a, &CameraStream::tripwireCrossed, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)(int , const QString & , const QString & )>(_a, &CameraStream::trackCrossedTripwire, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)(int , const QString & , qint64 )>(_a, &CameraStream::loiteringDetected, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::roiAlertActiveChanged, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::tripwireAlertActiveChanged, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::aiEnabledChanged, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::aiConfidenceThresholdChanged, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::detectionsChanged, 19))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)(const QImage & )>(_a, &CameraStream::snapshotCaptured, 20))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::autoSnapshotOnMotionChanged, 21))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::autoSnapshotOnRoiChanged, 22))
            return;
        if (QtMocHelpers::indexOfMethod<void (CameraStream::*)()>(_a, &CameraStream::autoSnapshotOnTripwireChanged, 23))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->id(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->source(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->sourceType(); break;
        case 3: *reinterpret_cast<QImage*>(_v) = _t->frame(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->isRunning(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->fps(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->status(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->cameraName(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->motionEnabled(); break;
        case 9: *reinterpret_cast<double*>(_v) = _t->motionSensitivity(); break;
        case 10: *reinterpret_cast<bool*>(_v) = _t->motionActive(); break;
        case 11: *reinterpret_cast<bool*>(_v) = _t->roiAlertActive(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->tripwireAlertActive(); break;
        case 13: *reinterpret_cast<bool*>(_v) = _t->aiEnabled(); break;
        case 14: *reinterpret_cast<double*>(_v) = _t->aiConfidenceThreshold(); break;
        case 15: *reinterpret_cast<QVariantList*>(_v) = _t->detections(); break;
        case 16: *reinterpret_cast<bool*>(_v) = _t->autoSnapshotOnMotion(); break;
        case 17: *reinterpret_cast<bool*>(_v) = _t->autoSnapshotOnRoi(); break;
        case 18: *reinterpret_cast<bool*>(_v) = _t->autoSnapshotOnTripwire(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 7: _t->setCameraName(*reinterpret_cast<QString*>(_v)); break;
        case 8: _t->setMotionEnabled(*reinterpret_cast<bool*>(_v)); break;
        case 9: _t->setMotionSensitivity(*reinterpret_cast<double*>(_v)); break;
        case 13: _t->setAiEnabled(*reinterpret_cast<bool*>(_v)); break;
        case 14: _t->setAiConfidenceThreshold(*reinterpret_cast<double*>(_v)); break;
        case 16: _t->setAutoSnapshotOnMotion(*reinterpret_cast<bool*>(_v)); break;
        case 17: _t->setAutoSnapshotOnRoi(*reinterpret_cast<bool*>(_v)); break;
        case 18: _t->setAutoSnapshotOnTripwire(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *CameraStream::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraStream::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12CameraStreamE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CameraStream::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 41)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 41;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 41)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 41;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void CameraStream::frameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CameraStream::runningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CameraStream::fpsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CameraStream::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CameraStream::cameraNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void CameraStream::snapshotSaved(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void CameraStream::snapshotFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void CameraStream::motionEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void CameraStream::motionSensitivityChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void CameraStream::motionActiveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void CameraStream::motionDetected(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void CameraStream::roiMotionDetected(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1);
}

// SIGNAL 12
void CameraStream::tripwireCrossed(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1);
}

// SIGNAL 13
void CameraStream::trackCrossedTripwire(int _t1, const QString & _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1, _t2, _t3);
}

// SIGNAL 14
void CameraStream::loiteringDetected(int _t1, const QString & _t2, qint64 _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1, _t2, _t3);
}

// SIGNAL 15
void CameraStream::roiAlertActiveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void CameraStream::tripwireAlertActiveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void CameraStream::aiEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void CameraStream::aiConfidenceThresholdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void CameraStream::detectionsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void CameraStream::snapshotCaptured(const QImage & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 20, nullptr, _t1);
}

// SIGNAL 21
void CameraStream::autoSnapshotOnMotionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void CameraStream::autoSnapshotOnRoiChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void CameraStream::autoSnapshotOnTripwireChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 23, nullptr);
}
QT_WARNING_POP
