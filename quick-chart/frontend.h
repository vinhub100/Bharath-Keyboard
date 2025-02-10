#ifndef FRONTEND_H
#define FRONTEND_H

#include <QObject>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QClipboard>
#include <QtDBus>


#include "common.h"
#include "qc_frame.h"
#include "configapp_adaptor.h"
#include "qc_adaptor.h"
#include "servicedemon_interface.h"

class FrontEnd : public QObject
{
    Q_OBJECT

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QActionGroup *actionGroup;
    QC_Frame *QC;
    in::kanet::kbdemon::Service *serviceDemon;

    int64_t timeStamp,prevTimeStamp = 0;
    QString selectedLang;
    int optionListLen,currentID = 0,prevID = 100; // 100 is magic number

    QProcess *process;

public:
    explicit FrontEnd(QObject *parent = nullptr);
    void createTrayMenu(void);
    void setSudoKbCharMap(void);
    void setLang(QString lang);
    void prev(void);
    void next(void);

    ~FrontEnd();

signals:

public slots:
    void toggleQCView(void);
    void toggleLang(void);
    void nKeyAction(QString key);

    void toggleqcview(void);
    void callCtrlV(void);
    void selectLangFromTrey(QAction *action);
    void reloadconf();
};

#endif // FRONTEND_H
