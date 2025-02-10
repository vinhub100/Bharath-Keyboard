#ifndef NKEYSWIDGET_H
#define NKEYSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include "flowlayout.h"
#include "nkeycomponent.h"

class nKeysWidget : public QWidget
{
    Q_OBJECT

    QLabel *label;
    FlowLayout *layout;
public:
    explicit nKeysWidget(QWidget *parent = nullptr);

signals:
    void reRender(int id);

public slots:
    void passRender(void);
};

#endif // NKEYSWIDGET_H
