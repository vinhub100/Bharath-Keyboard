#ifndef LANGCONFIGWIDGET_H
#define LANGCONFIGWIDGET_H

#include <QWidget>
//#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QDebug>
#include <QStringList>
#include <common.h>
#include "flowlayout.h"
#include "langline.h"
#include "langdialog.h"


class LangConfigWidget : public QWidget
{
    Q_OBJECT
    QStringList labls = {"","Default","2<sup>nd</sup> Language","3<sup>rd</sup> Language","4<sup>th</sup> Language","5<sup>th</sup> Language"};
    signed int TARGET;
//    QLabel *label;
    FlowLayout *layout;
    LangDialog *langDialog;

public:
    explicit LangConfigWidget(QWidget *parent = nullptr);
    void showLangDialog(QStringList langListParam);

signals:
    void reRender(int id);

public slots:
    void editLanguage(int index);
    void addLanguage(void);
    void removeLanguage(int index);
    void langSelectedHandler(QString lang);
    void dialogExitHandler(void);

};

#endif // LANGCONFIGWIDGET_H
