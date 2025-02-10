#include "nkeywizard.h"

NKeyWizard::NKeyWizard(QString nKey, QWidget *parent)
    : QWizard(parent),NKEY(nKey)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(true);


    setPage(TypePage,new ChooseTypePage(NKEY,parent));
    setPage(CharPage,new CharTypePage(NKEY,parent));
    setPage(ActionPage,new ActionTypePage(NKEY,parent));
    setPage(FinalPage,new ConclusionPage(NKEY,parent));

    setStartId(TypePage);

}

void NKeyWizard::accept(){

    QJsonObject newKeyConfig;
    QString type = field("type").toString();
    qDebug() << type;
    newKeyConfig.insert("type",type);
    if(type == "NONE"){

    } else if (type == "TEXT"){
        QString text = field("text").toString();
        newKeyConfig.insert("text",text);
    } else if (type == "ACTION"){
        QString executable = field("executable").toString();
        QString parameter = field("parameter").toString();
        newKeyConfig.insert("exe",executable);
        newKeyConfig.insert("param",parameter);
    } else {

    }

    QJsonObject nKeyStoreCopy = NKeyObject["NKeysConfig"].toObject();

    nKeyStoreCopy.remove(NKEY);
    nKeyStoreCopy.insert(NKEY,newKeyConfig);

    NKeyObject.remove("NKeysConfig");
    NKeyObject.insert("NKeysConfig",nKeyStoreCopy);
    SaveNKeyConfig();

    emit sendReRender();

}



ChooseTypePage::ChooseTypePage(QString nKey, QWidget *parent)
    : QWizardPage (parent),NKEY(nKey)
{
    layout = new QVBoxLayout(this);
    layout->setMargin(30);
    layout->setSpacing(10);
    info = new QLabel("Choose 'TYPE' of '" + NKEY + "' Key :",this);
    info->setStyleSheet("font-size:18px;");
    keyType = new QComboBox(this);
    keyType->addItems({"NONE","TEXT","ACTION"});
    keyType->setCurrentIndex(0);
    keyType->setStyleSheet("background-color:#cccccc;");
    layout->addWidget(info);
    layout->addWidget(keyType);
    registerField("type",keyType,"currentText", "currentTextChanged");
}

int ChooseTypePage::nextId() const{
    auto type = keyType->currentText();
   if(type == "TEXT"){
       return NKeyWizard::CharPage;
   }
   else if(type == "ACTION"){
       return NKeyWizard::ActionPage;
   }
   else if(type == "NONE"){
       return NKeyWizard::FinalPage;
   }
   else {
       return NKeyWizard::FinalPage;
   }

}

CharTypePage::CharTypePage(QString nKey,QWidget *parent)
    : QWizardPage (parent),NKEY(nKey)
{
    layout = new QVBoxLayout(this);
    layout->setMargin(30);
    layout->setSpacing(10);
    info = new QLabel("Enter text of '" + NKEY + "' Key :",this);
    info->setStyleSheet("font-size:18px;");
    keyText = new QLineEdit(this);
    keyText->setStyleSheet("background-color:#cccccc;");
    layout->addWidget(info);
    layout->addWidget(keyText);
    registerField("text*",keyText);
}


int CharTypePage::nextId() const{

    return NKeyWizard::FinalPage;
}

ActionTypePage::ActionTypePage(QString nKey,QWidget *parent)
    : QWizardPage (parent),NKEY(nKey)
{
    layout = new QVBoxLayout(this);
    layout->setMargin(30);
    layout->setSpacing(10);
    info = new QLabel("Select Executable File and Parameter for '" + NKEY + "' Key :",this);
    info->setStyleSheet("font-size:18px;");

    exLabel = new QLabel("Executable File :",this);
    executable = new QLineEdit(this);
    executable->setStyleSheet("background-color:#cccccc;");
    fileWizBtn = new QPushButton(QIcon("../data/file.png"),"",this);
    fileWizBtn->setFixedSize(25,25);
    fileWizBtn->setStyleSheet("background-color:#cccccc;");
    connect(fileWizBtn,&QPushButton::clicked,this,&ActionTypePage::showFileWizard);
    fileInputLayout = new QHBoxLayout();
    fileInputLayout->setSpacing(0);
    fileInputLayout->addWidget(executable);
    fileInputLayout->addWidget(fileWizBtn);

    paramLabel = new QLabel("Parameters :",this);
    parameter = new QLineEdit(this);
    parameter->setStyleSheet("background-color:#cccccc;");

    layout->addWidget(info);
    layout->addWidget(exLabel);
    layout->addLayout(fileInputLayout);
    layout->addWidget(paramLabel);
    layout->addWidget(parameter);

    registerField("executable*",executable);
    registerField("parameter",parameter);
}

int ActionTypePage::nextId() const{

    return NKeyWizard::FinalPage;
}

void ActionTypePage::showFileWizard(){
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (dialog.exec()){
          fileNames = dialog.selectedFiles();
          executable->setText(fileNames[0]);
    }

}

ConclusionPage::ConclusionPage(QString nKey,QWidget *parent)
    : QWizardPage (parent),NKEY(nKey)
{
    layout = new QVBoxLayout(this);
    layout->setMargin(30);
    layout->setSpacing(10);
    info = new QLabel("Configuring '" + NKEY + "' Key as :",this);
    info->setStyleSheet("font-size:18px;");
    layout->addWidget(info);

}

void ConclusionPage::initializePage(){


    QString type = field("type").toString();
    typeLabel =  new QLabel("    Type : " + type ,this);
    typeLabel->setStyleSheet("font-size:14px;");
    layout->addWidget(typeLabel);
    if(type == "NONE"){

    } else if (type == "TEXT"){
        QString text = field("text").toString();
        textLabel =  new QLabel("    Text : " + text ,this);
        textLabel->setStyleSheet("font-size:14px;");
        layout->addWidget(textLabel);
    } else if (type == "ACTION"){
        QString executable = field("executable").toString();
        QString parameter = field("parameter").toString();
        executableLabel =  new QLabel("    Executable File : " + executable ,this);
        executableLabel->setStyleSheet("font-size:14px;");
        layout->addWidget(executableLabel);
        parameterLabel =  new QLabel("    Parameters : " + parameter ,this);
        parameterLabel->setStyleSheet("font-size:14px;");
        layout->addWidget(parameterLabel);
    } else {

    }

}

void ConclusionPage::cleanupPage(){


    QString type = field("type").toString();
    layout->removeWidget(typeLabel);
    delete typeLabel;
    if(type == "NONE"){

    } else if (type == "TEXT"){
        layout->removeWidget(textLabel);
        delete textLabel;
    } else if (type == "ACTION"){
        layout->removeWidget(executableLabel);
        layout->removeWidget(parameterLabel);
        delete executableLabel;
        delete parameterLabel;
    } else {

    }

}
