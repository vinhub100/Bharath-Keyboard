#include "mainviewwidget.h"

MainViewWidget::MainViewWidget(QWidget *parent) : QWidget(parent)
{
    QC = new in::kanet::kbconfig::Config("in.kanet.kbconfig","/Config",QDBusConnection::sessionBus(),this);
    setWindowIcon(QIcon("../data/gear.png"));
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("Quick Keys Configuration");
    setFixedSize(600,500);
    QDesktopWidget *desktop = QApplication::desktop();
    int windowWidth = 600;
    int windowHeight = 500;
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();
    setGeometry((screenWidth/2)-(windowWidth/2),
                (screenHeight/2)-(windowHeight/2),
                windowWidth,
                windowHeight);
    navBar = new QWidget(this);
//    navBar->setGeometry(0,0,600,20);
    navBar->setFixedSize(598,30);
    tabBarLayout = new QHBoxLayout();
    tabBarLayout->setMargin(0);
    tabBarLayout->setSpacing(0);
    navBar->setLayout(tabBarLayout);

    nBtnConfig = new QPushButton(" +N Keys ",navBar);
    langConfig = new QPushButton(" Languages ",navBar);
    btnGroup = new QButtonGroup(navBar);
    btnGroup->addButton(langConfig,1);
    langConfig->setChecked(true);
    langConfig->setStyleSheet("color:black");
    nBtnConfig->setStyleSheet("color:grey");

    btnGroup->addButton(nBtnConfig,2);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(navButtonClicked(int)));

    tabBarLayout->addWidget(langConfig);
    tabBarLayout->addWidget(nBtnConfig);

    appLayout = new QVBoxLayout(this);
    appLayout->setMargin(1);
    appLayout->setSpacing(0);

    actionWidget = new QWidget(this);
    actionContainer = new QBoxLayout(QBoxLayout::TopToBottom,actionWidget);
    actionContainer->setMargin(0);
    actionContainer->setSpacing(0);
    actionWidget->setLayout(actionContainer);
//    actionWidget->setStyleSheet("background-color:steelblue");

    this->createLangWidget();
    this->prev_action_pane_no = 1;
    //    actionWidget->setGeometry(0,50,600,480);

    appLayout->addWidget(navBar,1);
    appLayout->addWidget(actionWidget,30);
    setLayout(appLayout);


}


void MainViewWidget::navButtonClicked(int id){

    if(this->prev_action_pane_no == 2){
        actionContainer->removeWidget(nkWidget);
        delete nkWidget;
        nBtnConfig->setStyleSheet("color:grey;");
    } else if(this->prev_action_pane_no == 1) {
        actionContainer->removeWidget(langWidget);
        delete langWidget;
        langConfig->setStyleSheet("color:grey;");
    } else {}


    if(id == 1){
        langConfig->setStyleSheet("color:black;");
        this->createLangWidget();
    }  else if(id == 2) {
        nBtnConfig->setStyleSheet("color:black;");
        this->createNBtnWidget();
    } else {}

    this->prev_action_pane_no = id;

}

void MainViewWidget::createLangWidget(void){
    langWidget = new LangConfigWidget(actionWidget);
    connect(langWidget,&LangConfigWidget::reRender,this,&MainViewWidget::navButtonClicked);
    actionContainer->addWidget(langWidget);
}


void MainViewWidget::createNBtnWidget(void){
    nkWidget = new nKeysWidget(actionWidget);
    connect(nkWidget,&nKeysWidget::reRender,this,&MainViewWidget::navButtonClicked);
    actionContainer->addWidget(nkWidget);

}

void MainViewWidget::closeEvent(QCloseEvent *event){
    QC->reloadconf();
    event->accept();

}
