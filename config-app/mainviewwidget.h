#ifndef MAINVIEWWIDGET_H
#define MAINVIEWWIDGET_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QScrollArea>
#include <QtDBus>
#include <QDebug>

#include "langconfigwidget.h"
#include "nkeyswidget.h"
#include "langdialog.h"
#include "configapp_interface.h"

class MainViewWidget : public QWidget
{
    Q_OBJECT

    int prev_action_pane_no;

    QHBoxLayout *tabBarLayout;
    QVBoxLayout *appLayout;
    QBoxLayout *actionContainer;
    QWidget *navBar,*actionWidget;
    QPushButton *nBtnConfig, *langConfig;
    QButtonGroup *btnGroup;
    LangConfigWidget *langWidget;
    LangDialog *dialog;
    nKeysWidget *nkWidget;

    in::kanet::kbconfig::Config *QC;

public:
    explicit MainViewWidget(QWidget *parent = nullptr);
    void createLangWidget(void);
    void createNBtnWidget(void);

protected:
    void closeEvent(QCloseEvent *event) override;

signals:

public slots:
    void navButtonClicked(int id);

};

#endif // MAINVIEWWIDGET_H
