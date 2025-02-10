#ifndef LANGLINE_H
#define LANGLINE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QDebug>

class LangLine : public QWidget
{
    Q_OBJECT
    int id;
    QString language;
    bool butDisabled;

    QHBoxLayout *layout;
    QLabel *idLabel,*languageLabel;
    QPushButton *edit,*remove;

public:
    explicit LangLine(int id, QString lang, QWidget *parent = nullptr,bool disabled = false);

signals:
    void langClicked(int index);
    void langRemove(int index);

public slots:
    void gearClicked(void);
    void exClicked(void);
};

#endif // LANGLINE_H
