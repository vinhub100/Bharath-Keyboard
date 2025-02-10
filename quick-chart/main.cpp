#include <stdio.h>
#include <QApplication>
#include "common.h"
#include "frontend.h"

#include <QStringList>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QClipboard>

#include <QDebug>


QStringList rList;
QStringList fList;
QJsonObject chartobject;

QJsonObject LangConfObject;
QStringList selectedLangs;
QJsonObject NKeyObject;

QClipboard *cb;

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

    LangConfObject = langOptDoc.object();

    return 1;
}

int saveStore(QString storeType)
{
    QFile dFile(QDir::homePath() + "/.kanetkb/" + storeType + ".store");
    if (!dFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "File not Open for Write";
        return 0;
    }
    QJsonObject json_obj;
    QJsonArray dArray;
    if(storeType == "recent"){
        dArray = QJsonArray::fromStringList(rList);
    } else {
        dArray = QJsonArray::fromStringList(fList);
    }
    json_obj[storeType] = dArray;
    QJsonDocument json_doc(json_obj);
    dFile.write(json_doc.toJson());
    dFile.close();
    return 1;
}

int loadStore(QString storeType)
{
    QFile dFile(QDir::homePath() + "/.kanetkb/" + storeType + ".store");
    if (!dFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "File not Open for Read";
        return 0;
    }
    QByteArray filedata = dFile.readAll();
    QJsonDocument rec_doc(QJsonDocument::fromJson(filedata));
    QJsonObject jobject = rec_doc.object();
    QJsonArray jarray = jobject[storeType].toArray();

    if(storeType == "recent"){
        for (int i = 0; i < jarray.size(); i++)
        {
            rList << jarray[i].toString();
        }
//        qDebug() << "loadStore Recent" << rList;
    }
    else{
        for (int i = 0; i < jarray.size(); i++)
        {
            fList << jarray[i].toString();
        }
//        qDebug() << "loadStore Favourite" << fList;
    }

    return 1;
}

int saveRecent(QString charcter){
    if(rList.contains(charcter)){
        rList.removeAll(charcter);
    }
    rList.prepend(charcter);
    rList = rList.mid(0,20);
    qDebug() << "rList";
    saveStore("recent");
    return 1;
}

int loadRecent(void){
    loadStore("recent");
    return 1;
}

int saveFavourite(QString charcter){
    if(fList.contains(charcter)){
        fList.removeAll(charcter);
    }
    fList.prepend(charcter);
    saveStore("favourite");
    return 1;
}

int loadFavourite(void){
    loadStore("favourite");
    return 1;
}

int initCharTable(void){
    QFile file("../data/charlist.store");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "File(initCharTable) not Open for Read";
        return 0;
    }
    QByteArray data = file.readAll();
    QJsonDocument chartdoc(QJsonDocument::fromJson(data));
    chartobject = chartdoc.object();
//    qDebug() << chartobject;
    return 1;
}

int start_front_end(int argc, char *argv[]){

    QApplication app(argc, argv);
    cb = QGuiApplication::clipboard();
    FrontEnd frontend;
    return app.exec();
}

void prepareUser(void){
    QString HomePath = QDir::homePath();

    QDir confDir(HomePath + "/.kanetkb");
    if(!confDir.exists()){
        if(!confDir.mkpath(".")){
            qDebug() << ".kanetkb not created at Home Folder";
        }
    }
    if(!QFile::exists(HomePath + "/.kanetkb/favourite.store")){
        if(!QFile::copy("../data/favourite.store", HomePath + "/.kanetkb/favourite.store")){
            qDebug() << "favourite.store not copied";
        }
    }
    if(!QFile::exists(HomePath + "/.kanetkb/langconfig.store")){
        if(!QFile::copy("../data/langconfig.store", HomePath + "/.kanetkb/langconfig.store")){
            qDebug() << "langconfig.store not copied";
        }
    }
    if(!QFile::exists(HomePath + "/.kanetkb/nKeysConfig.store")){
        if(!QFile::copy("../data/nKeysConfig.store", HomePath + "/.kanetkb/nKeysConfig.store")){
            qDebug() << "nKeysConfig.store not copied";
        }
    }
    if(!QFile::exists(HomePath + "/.kanetkb/recent.store")){
        if(!QFile::copy("../data/recent.store", HomePath + "/.kanetkb/recent.store")){
            qDebug() << "recent.store not copied ";
        }
    }
}

int main(int argc, char *argv[])
{
    prepareUser();
    loadRecent();
    loadFavourite();
    initCharTable();

    LoadLangConfig();
    LoadLangOptions();
    LoadNKeyConfig();

    qDebug() << rList;
//    qDebug() << selectedLangs;
//    qDebug() << NKeyObject;

    start_front_end(argc, argv);
    return 0;
}
