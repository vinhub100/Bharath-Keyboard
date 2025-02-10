#include "nkeycomponent.h"

NKeyComponent::NKeyComponent(QString nKey,QWidget *parent) : QWidget(parent),NKEY(nKey)
{
    setFixedSize(540,80);

    compLayout = new QHBoxLayout();
    setLayout(compLayout);
    compLayout->setSpacing(0);
    compLayout->setMargin(0);

    QLabel *label = new QLabel(NKEY,this);
    label->setStyleSheet("font-size:18px");
    label->setAlignment(Qt::AlignCenter);

    wijet = new QWidget(this);
    subLayout = new QGridLayout(wijet);

    QPushButton *gear = new QPushButton(QIcon("../data/gear.png"),"",this);
    gear->setFixedSize(20,20);
    gear->setStyleSheet("border-radius:50%;");
    connect(gear,&QPushButton::clicked,this,&NKeyComponent::configWizard);

    QLabel *keytype,*line2,*line3;
    QString type = NKeyObject["NKeysConfig"].toObject()[NKEY].toObject()["type"].toString();

    keytype = new QLabel();
    keytype->setText("Type : " + type);

    subWijitLayout = new QVBoxLayout();
    subWijitLayout->addWidget(keytype);

    if(type == "NONE"){

    } else if(type == "TEXT"){
        QString text = NKeyObject["NKeysConfig"].toObject()[NKEY].toObject()["text"].toString();
        line2 = new QLabel("Text : " + text);
        line2->setStyleSheet("font-size:12px");
        subWijitLayout->addWidget(line2);
    } else if(type == "ACTION"){
        QString exe = NKeyObject["NKeysConfig"].toObject()[NKEY].toObject()["exe"].toString();
        QString param = NKeyObject["NKeysConfig"].toObject()[NKEY].toObject()["param"].toString();
        line2 = new QLabel("Run : " + exe);
        line3 = new QLabel("Parameter : " + param);
        line2->setStyleSheet("font-size:12px");
        line3->setStyleSheet("font-size:12px");
        subWijitLayout->addWidget(line2);
        subWijitLayout->addWidget(line3);
    } else {

    }

    subLayout->addLayout(subWijitLayout,1,1);
    subLayout->addWidget(gear,1,1,Qt::AlignRight | Qt::AlignTop);
    compLayout->addWidget(label,2);
    compLayout->addWidget(wijet,8);


}

void NKeyComponent::configWizard(){
    wizard = new NKeyWizard(NKEY,this);
    connect(wizard,&NKeyWizard::sendReRender,this,&NKeyComponent::passRender);
    wizard->show();
}

void NKeyComponent::passRender(void){
    emit rePassRender();
}
