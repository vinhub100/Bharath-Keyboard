#include "usbthread.h"
#include <QDebug>



USBThread::USBThread(QObject *parent):QThread (parent)
{

}

void USBThread::run(){
    while(1){
        USB_Loop();
        if(dirtyFlag){
            dirtyFlag = false;
            emit CodeFromKB(buffer[0]);
            qDebug() << "In Thread: " << buffer[0];
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

