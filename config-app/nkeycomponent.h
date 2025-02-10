#ifndef NKEYCOMPONENT_H
#define NKEYCOMPONENT_H

#include <QDebug>
#include <QWidget>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonObject>
#include "flowlayout.h"
#include "common.h"
#include "nkeywizard.h"

class NKeyComponent : public QWidget
{
    Q_OBJECT
//    QJsonObject NKeyObject;
    QString NKEY;
    QHBoxLayout *compLayout;
    QVBoxLayout *subWijitLayout;
    QGridLayout *subLayout;
    QWidget *wijet,*subWijit;
    NKeyWizard *wizard;
public:
    explicit NKeyComponent(QString nKey, QWidget *parent = nullptr);

signals:
    void rePassRender(void);
public slots:
    void configWizard(void);
    void passRender(void);
};

#endif // NKEYCOMPONENT_H
