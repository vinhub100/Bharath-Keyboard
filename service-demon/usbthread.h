#ifndef USBTHREAD_H
#define USBTHREAD_H

#include <QThread>
#include "usbutil.h"
#include "libusb.h"




class USBThread : public QThread
{
    Q_OBJECT
public:
    USBThread(QObject *parent);
//    ~USBThread();
    void run();

signals:
    void CodeFromKB(unsigned char code);

};

#endif // USBTHREAD_H
