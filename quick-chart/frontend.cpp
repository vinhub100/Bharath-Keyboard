#include "frontend.h"

#include <chrono>

using namespace std::chrono;

FrontEnd::FrontEnd(QObject *parent) : QObject(parent)
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon("../data/qwerty.png"));
    trayIconMenu = new QMenu();
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
    actionGroup = new QActionGroup(this);

    createTrayMenu();

    QC = new QC_Frame();
    connect(QC,&QC_Frame::sendCtlVsignal,this,&FrontEnd::callCtrlV);

    optionListLen = selectedLangs.length()+1;
    process = new QProcess(this);

    setSudoKbCharMap();

    new ConfigAdaptor(this);
    QDBusConnection con = QDBusConnection::sessionBus();
    con.registerService("in.kanet.kbconfig");
    con.registerObject("/Config", this);

    new QCAdaptor(this);
    QDBusConnection demon = QDBusConnection::systemBus();
    demon.registerService("in.kanet.kb");
    demon.registerObject("/QC",this);

    serviceDemon = new in::kanet::kbdemon::Service("in.kanet.kbdemon","/Service",QDBusConnection::systemBus(),this);
}

FrontEnd::~FrontEnd(){
    delete QC;
}

void FrontEnd::createTrayMenu(void){

    trayIconMenu->clear();
    delete actionGroup;
    actionGroup = new QActionGroup(this);

    QAction *act = new QAction("QWERTY",actionGroup);
    actionGroup->addAction(act);
    trayIconMenu->addAction(act);

    int l = selectedLangs.length();
    for(int i = 0; i<l; i++){
        QAction *act = new QAction(selectedLangs[i],actionGroup);
        actionGroup->addAction(act);
        trayIconMenu->addAction(act);

    }
    trayIconMenu->addSeparator();
    connect(actionGroup, &QActionGroup::triggered, this, &FrontEnd::selectLangFromTrey);

    QAction *toggleAction = new QAction("Show &QuickChart");
    connect(toggleAction, &QAction::triggered, this, &FrontEnd::toggleqcview);
    trayIconMenu->addAction(toggleAction);
}

void FrontEnd::toggleQCView(void){
    toggleqcview();
}

void FrontEnd::setSudoKbCharMap(){
    process->start("/bin/bash -c \"xinput list --id-only 'kanet Psudo Keyboard'\"");
    process->waitForFinished();
    QString dId = process->readAllStandardOutput();
    dId = dId.remove("\n");
    process->start("/bin/bash -c \"setxkbmap -device "+ dId +" -model evdev vitanive_vndr/in\"");
    process->waitForFinished();
//    qDebug() << "Data: " << dId;
}

void FrontEnd::setLang(QString lang){
//    qDebug() << lang << "CID: " << currentID << " PID: " << prevID;
    QString cmd,iconImg;
    cmd = "/bin/bash -c \"setxkbmap -model evdev ";
    if(lang == "QWERTY"){
        iconImg = "../data/qwerty.png";
        cmd = cmd + "vitanive_vndr/in\"";
    } else {
        QJsonObject lObj = LangConfObject["langoptions"].toObject()[lang].toObject();
        cmd = cmd + lObj["keymap"].toString() + "\"";
        iconImg = lObj["icon"].toString();
    }

    process->start(cmd);
    process->waitForFinished();
    trayIcon->setIcon(QIcon(iconImg));
    setSudoKbCharMap();
}

void FrontEnd::next(void){
    if(currentID == optionListLen - 1){
        currentID = 0;
    } else {
        currentID = currentID + 1;
    }
}

void FrontEnd::prev(void){

    int tempID;
    if(prevID != 100){
        tempID = currentID;
        currentID = prevID;
        prevID = tempID;
    } else {
        prevID = currentID;
        next();
    }
}

void FrontEnd::toggleLang(void){

    if(optionListLen > 1){
        timeStamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        int64_t delta;
        delta = timeStamp-prevTimeStamp;

        if(delta>2000){
            prev();

        } else {
            next();
        }
//        selectedLang = selectedLangs[currentID];
        qDebug() << "Current: " << currentID << " Prev: " << prevID;
        if(currentID == 0){
            setLang("QWERTY");
        } else {
            setLang(selectedLangs[currentID-1]);
        }

        prevTimeStamp = timeStamp;

    }
}

void FrontEnd::nKeyAction(QString key){
    QJsonObject keyObj = NKeyObject["NKeysConfig"].toObject()[key].toObject();
    QString type = keyObj["type"].toString();
    if(type == "NONE"){
        return;
    } else if(type == "ACTION"){
        QString exe = keyObj["exe"].toString();
        QString param = keyObj["param"].toString();
        process->setProgram(exe);
        process->setArguments(QStringList(param));
        process->startDetached();

        trayIcon->showMessage(key + " Key Running", "'" + exe + "' started with '" + param + "' as Parameter",QSystemTrayIcon::Information,3000);

        qDebug() << "Action";
    } else if(type == "TEXT"){
        QString txt = keyObj["text"].toString();
        QC->cb->setText(txt,QClipboard::Clipboard);
        serviceDemon->callCtrlV(100);
    }
}

void FrontEnd::toggleqcview(void){
        if(!QC->isVisible()){
            QC->show();
        } else {
            QC->hide();
            QC->setFocusPolicy(Qt::ClickFocus);
        }
}

void FrontEnd::selectLangFromTrey(QAction *action){
    QString langName = action->text();
    prevID = currentID;
    if(langName == "QWERTY"){
        currentID = 0;
    } else {
        currentID = selectedLangs.indexOf(langName)+1;
    }
    setLang(langName);
}

void FrontEnd::callCtrlV(void){
//    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    serviceDemon->callCtrlV(400);
//    qDebug() << "Ctrl - V";
}

void FrontEnd::reloadconf(){

    selectedLangs.clear();

    LoadLangConfig();
    LoadLangOptions();
    LoadNKeyConfig();
    optionListLen = selectedLangs.length()+1;

    this->createTrayMenu();

    qDebug() << "Reload Conf";

}
