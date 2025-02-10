#include "langdialog.h"

LangDialog::LangDialog(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setFocusPolicy(Qt::ClickFocus);
//    setAttribute(Qt::WA_DeleteOnClose);
//    setFixedSize(300,250);
    setFixedWidth(300);
    layout = new FlowLayout(this);
    btnGroup = new QButtonGroup(this);
    connect(btnGroup,QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),this,&LangDialog::langButtonClicked);


}

void LangDialog::createList(QStringList langList){
    int listlen = langList.length();
    for(int i =0;i<listlen;i++){
        QPushButton *btn = new QPushButton(langList[i],this);
        btn->setFixedWidth(275);
        btn->setStyleSheet("font-size:16px;");
        btnGroup->addButton(btn);
        layout->addWidget(btn);
    }
}


void LangDialog::focusOutEvent(QFocusEvent* event){
    close();
    emit dialogExit();
}

void LangDialog::langButtonClicked(QAbstractButton *button){
    emit langSelected(button->text());
}
