#include "langline.h"

LangLine::LangLine(int id, QString lang, QWidget *parent, bool disabled) :
    QWidget(parent),id(id),language(lang),butDisabled(disabled)
{
    layout = new QHBoxLayout();
    layout->setSpacing(0);
    idLabel = new QLabel(QString::number(id+2),this);
    idLabel->setAlignment(Qt::AlignCenter);
    idLabel->setStyleSheet("font-size:16px;");
    idLabel->setFixedHeight(25);
    languageLabel = new QLabel(language,this);
    languageLabel->setStyleSheet("font-size:16px;");
    languageLabel->setAlignment(Qt::AlignCenter);
    languageLabel->setFixedHeight(25);
    edit = new QPushButton(this);
    edit->setIcon(QIcon("../data/gear.png"));
    edit->setFixedSize(25,25);
    connect(edit,SIGNAL(clicked()),this,SLOT(gearClicked()));
    remove = new QPushButton("❌",this);
    remove->setFixedSize(25,25);
    connect(remove,SIGNAL(clicked()),this,SLOT(exClicked()));
    if(butDisabled){
        edit->setEnabled(false);
        remove->setEnabled(false);
    }

    layout->addWidget(idLabel,1);
    layout->addWidget(languageLabel,7);
    layout->addWidget(edit,1);
    layout->addWidget(remove,1);

    setFixedSize(450,40);
    setStyleSheet("background-color:#ffffff;");
//    setFixedHeight(30);
    setLayout(layout);
}

void LangLine::gearClicked(){
    emit langClicked(this->id);
}

void LangLine::exClicked(){
    emit langRemove(this->id);
}
