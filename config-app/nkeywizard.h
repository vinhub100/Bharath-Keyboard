#ifndef NKEYWIZARD_H
#define NKEYWIZARD_H

#include <QDebug>
#include <QWidget>
#include <QWizard>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QComboBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "common.h"

class NKeyWizard : public QWizard
{

    Q_OBJECT

public:
    QString NKEY;
    enum { TypePage, CharPage, ActionPage, FinalPage };

    NKeyWizard(QString nKey, QWidget *parent = nullptr);
    void accept() override;

signals:
    void sendReRender(void);
};


class ChooseTypePage : public QWizardPage
{
    Q_OBJECT

public:
    QString NKEY;
    QVBoxLayout *layout;
    QLabel *info;
    QComboBox *keyType;
    ChooseTypePage(QString nKey,QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *discription;
    QComboBox *nKeyType;

};

class CharTypePage : public QWizardPage
{
    Q_OBJECT

public:
    QString NKEY;
    QVBoxLayout *layout;
    QLabel *info;
    QLineEdit *keyText;
    CharTypePage(QString nKey,QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *discription;
    QLineEdit *keyCharEdit;

};

class ActionTypePage : public QWizardPage
{
    Q_OBJECT

public:
    QString NKEY;
    ActionTypePage(QString nKey,QWidget *parent = nullptr);

    int nextId() const override;

private:
    QLabel *info,*exLabel,*paramLabel;
    QLineEdit *executable;
    QLineEdit *parameter;
    QPushButton *fileWizBtn;
    QVBoxLayout *layout;
    QHBoxLayout *fileInputLayout;

public slots:
    void showFileWizard(void);

};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

    QVBoxLayout *layout;
    QLabel *info,*typeLabel,*textLabel,*executableLabel,*parameterLabel;

public:
    QString NKEY;
    ConclusionPage(QString nKey,QWidget *parent = nullptr);

protected:
    void initializePage() override;
    void cleanupPage() override;

private:
    QLabel *label;
};


#endif // NKEYWIZARD_H
