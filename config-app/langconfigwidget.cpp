#include "langconfigwidget.h"

LangConfigWidget::LangConfigWidget(QWidget *parent) : QWidget(parent)
{
//    setGeometry(0,30,600,468);
    layout = new FlowLayout(this);
    layout->setSpacing(0);
    layout->setMargin(70);

//    QStringList langConf = {"","","Telugu","Devanagari","",""};

    LangLine *line = new LangLine(-1,"QWERTY",this,true);
    layout->addWidget(line);

    int listLength = (selectedLangs.length());
    for(int i = 0;i<listLength;i++){
        LangLine *line = new LangLine(i,selectedLangs[i],this);
        connect(line,SIGNAL(langClicked(int)),this,SLOT(editLanguage(int)));
        connect(line,SIGNAL(langRemove(int)),this,SLOT(removeLanguage(int)));
        layout->addWidget(line);
    }

    QPushButton *addButton = new QPushButton("+",this);
    addButton->setStyleSheet("font-size:16px;padding:3px;margin-left:8px;margin-right:8px;");
    addButton->setFixedSize(450,25);
    connect(addButton,&QAbstractButton::clicked,this,&LangConfigWidget::addLanguage);

    if(listLength >= 4){
        addButton->setEnabled(false);
    }
    layout->addWidget(addButton);
//    setFixedSize(600,468);
//    this->setStyleSheet("");
//    label = new QLabel("Language Config Widget",this);
//    layout->addWidget(label);
    langDialog = new LangDialog(this);
    TARGET = -1;

}

void LangConfigWidget::showLangDialog(QStringList langListParam){
    delete langDialog;
    langDialog = new LangDialog(this);
    connect(langDialog,&LangDialog::langSelected,this,&LangConfigWidget::langSelectedHandler);
    connect(langDialog,&LangDialog::dialogExit,this,&LangConfigWidget::dialogExitHandler);
    langDialog->createList(langListParam);
    langDialog->show();
    langDialog->setFocus();
}

void LangConfigWidget::addLanguage(void){
    TARGET = 100;
    QStringList lanList;
    int len = langList.length();
    for(int i = 0; i< len; i++){
        if(!selectedLangs.contains(langList[i])){
            lanList.append(langList[i]);
        }
    }
    showLangDialog(lanList);
}

void LangConfigWidget::editLanguage(int index){
    TARGET = index;
    QStringList lanList;
    int len = langList.length();
    for(int i = 0; i< len; i++){
        if(!selectedLangs.contains(langList[i])){
            lanList.append(langList[i]);
        }
    }
    lanList.prepend(selectedLangs[index]);
    showLangDialog(lanList);
}

void LangConfigWidget::langSelectedHandler(QString lang){
    qDebug() << TARGET;
    if(TARGET == 100){
        selectedLangs.append(lang);
    } else {
        selectedLangs[TARGET] = lang;
    }

    SaveLangConfig();
    TARGET = -1;
    emit reRender(1);
    qDebug() << "Lang Selected" << lang;
}

void LangConfigWidget::removeLanguage(int index){
    selectedLangs.removeAll(selectedLangs[index]);
    SaveLangConfig();
    TARGET = -1;
    emit reRender(1);
}

void LangConfigWidget::dialogExitHandler(void){

    qDebug() << "Lang Dialog Exit";

}
