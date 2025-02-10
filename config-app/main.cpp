#include <QApplication>

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "mainviewwidget.h"
#include "common.h"

QStringList langList;
QStringList selectedLangs = {};

QJsonObject NKeyObject;

int SaveNKeyConfig(void){
    QFile nkFile(QDir::homePath() + "/.kanetkb/nKeysConfig.store");
    if (!nkFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "SaveNKeyConfig - File not Open for Write";
        return 0;
    }
    QJsonDocument json_doc(NKeyObject);
    nkFile.write(json_doc.toJson());
    nkFile.close();
    return 1;
}

int LoadNKeyConfig(void){
    QFile nkFile(QDir::homePath() + "/.kanetkb/nKeysConfig.store");
    if (!nkFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "LoadNKeyConfig - File not Open for Read";
        return 0;
    }
    QByteArray filedata = nkFile.readAll();
    QJsonDocument nkConfDoc(QJsonDocument::fromJson(filedata));
    NKeyObject = nkConfDoc.object();
    return 1;
}

int SaveLangConfig(void){
    QFile lFile(QDir::homePath() + "/.kanetkb/langconfig.store");
    if (!lFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "SaveLangConfig - File not Open for Write";
        return 0;
    }
    QJsonObject json_obj;
    QJsonArray lArray = QJsonArray::fromStringList(selectedLangs);
    json_obj["selectedLanguage"] = lArray;
    QJsonDocument json_doc(json_obj);
    lFile.write(json_doc.toJson());
    lFile.close();
    return 1;
}

int LoadLangConfig(void){
    QFile lFile(QDir::homePath() + "/.kanetkb/langconfig.store");
    if (!lFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "LoadLangConfig - File not Open for Read";
        return 0;
    }
    QByteArray filedata = lFile.readAll();
    QJsonDocument langConfDoc(QJsonDocument::fromJson(filedata));
    QJsonObject lobject = langConfDoc.object();
    QJsonArray larray = lobject["selectedLanguage"].toArray();
    for (int i = 0; i < larray.size(); i++)
    {
        selectedLangs << larray[i].toString();
    }
    return 1;
}

int LoadLangOptions(void){
    QFile lFile("../data/langoptions.store");
    if (!lFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "LoadLangOptions - File not Open for Read";
        return 0;
    }
    QByteArray filedata = lFile.readAll();
    QJsonDocument langOptDoc(QJsonDocument::fromJson(filedata));
    QJsonObject langObject = langOptDoc.object();

    langList = langObject["langoptions"].toObject().keys();
    return 1;
}


int main(int argc, char *argv[])
{
    LoadLangOptions();
    LoadLangConfig();
    LoadNKeyConfig();

    qDebug() << selectedLangs;

    QApplication a(argc, argv);

    MainViewWidget *view = new MainViewWidget();

    view->show();

    return a.exec();
}
