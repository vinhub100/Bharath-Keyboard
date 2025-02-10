#ifndef SERVICE_H
#define SERVICE_H

#include <QtDBus>
#include <QObject>
#include <QDebug>
#include "usbutil.h"
#include "usbthread.h"
#include "servicedemon_adaptor.h"
#include "qc_interface.h"
#include "common.h"




class Service : public QObject
{
    Q_OBJECT
    USBThread *usbProc;
    in::kanet::kb::QC *qc;
public:
    explicit Service(QObject *parent = nullptr);
    ~Service();
signals:

public slots:
    void callCtrlV(int msec);
    void USBSignalSlot(uint8_t code);
};


#endif // SERVICE_H
