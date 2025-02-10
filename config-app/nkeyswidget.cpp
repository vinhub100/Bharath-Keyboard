#include "nkeyswidget.h"

nKeysWidget::nKeysWidget(QWidget *parent) : QWidget(parent)
{
    layout = new FlowLayout(this);
    layout->setMargin(25);

    setFixedSize(600,468);
    setStyleSheet("background-color:#ffffff;");

//    label = new QLabel("+N Keys Widget",this);
//    layout->addWidget(label);

    NKeyComponent *keyComponent = new NKeyComponent("+1",this);
    connect(keyComponent,&NKeyComponent::rePassRender,this,&nKeysWidget::passRender);
    layout->addWidget(keyComponent);
    NKeyComponent *keyComponent2 = new NKeyComponent("+2",this);
    connect(keyComponent2,&NKeyComponent::rePassRender,this,&nKeysWidget::passRender);
    layout->addWidget(keyComponent2);
    NKeyComponent *keyComponent3 = new NKeyComponent("+3",this);
    connect(keyComponent3,&NKeyComponent::rePassRender,this,&nKeysWidget::passRender);
    layout->addWidget(keyComponent3);
    NKeyComponent *keyComponent4 = new NKeyComponent("+4",this);
    connect(keyComponent4,&NKeyComponent::rePassRender,this,&nKeysWidget::passRender);
    layout->addWidget(keyComponent4);
    NKeyComponent *keyComponent5 = new NKeyComponent("+5",this);
    connect(keyComponent5,&NKeyComponent::rePassRender,this,&nKeysWidget::passRender);
    layout->addWidget(keyComponent5);

}

//    setGeometry(0,30,600,420);
//    move(0,30);
void nKeysWidget::passRender(void){
    qDebug() << "2 will be reRendered";
    emit reRender(2);
}
