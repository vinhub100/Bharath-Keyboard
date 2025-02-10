
#include "service.h"

#define LANG 1
#define QkC 2
#define N1 3
#define N2 4
#define N3 5
#define N4 6
#define N5 7

//

Service::Service(QObject *parent) : QObject(parent)
{
    initCtrlV();


    new ServiceAdaptor(this);
    QDBusConnection con = QDBusConnection::systemBus();
    con.registerService("in.kanet.kbdemon");
    con.registerObject("/Service", this);

    qc = new in::kanet::kb::QC("in.kanet.kb","/QC",QDBusConnection::systemBus(),this);

    USB_Init();

    if(USB_Configed == 1){
        usbProc = new USBThread(this);
        connect(usbProc,&USBThread::CodeFromKB,this,&Service::USBSignalSlot);
        usbProc->start();
    }

}

Service::~Service(){

    closeCtrlV();
    if(USB_Configed == 1){
        USB_Exit();
    }
}

void Service::callCtrlV(int msec){
    CtrlV(msec);
}


void Service::USBSignalSlot(uint8_t code){
    switch(code){
        case LANG:
            qc->toggleLang();
//            qDebug() << "In Service Lang";
            break;
        case QkC:
            qc->toggleQCView();
//            qDebug() << "In Service QC";
            break;
        case N1:
            qc->nKeyAction("+1");
//            qDebug() << "In Service +1";
            break;
        case N2:
            qc->nKeyAction("+2");
//            qDebug() << "In Service +2";
            break;
        case N3:
            qc->nKeyAction("+3");
//            qDebug() << "In Service +3";
            break;
        case N4:
            qc->nKeyAction("+4");
//            qDebug() << "In Service +4";
            break;
        case N5:
            qc->nKeyAction("+5");
//            qDebug() << "In Service +5";
            break;
        default:
            return;

    }

    return;
}
