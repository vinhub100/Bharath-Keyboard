#ifndef LANGDIALOG_H
#define LANGDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QButtonGroup>
#include <QStringList>
#include "flowlayout.h"

class LangDialog : public QDialog
{
    Q_OBJECT
    FlowLayout *layout;
    QButtonGroup *btnGroup;

public:
    explicit LangDialog(QWidget *parent = nullptr);
    void createList(QStringList langList);
protected:
    void focusOutEvent(QFocusEvent* event);

signals:
    void langSelected(QString lang);
    void dialogExit(void);

public slots:
    void langButtonClicked(QAbstractButton *button);

};

#endif // LANGDIALOG_H
