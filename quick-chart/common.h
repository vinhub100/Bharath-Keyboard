
#include <QStringList>
#include <QJsonObject>
#include <QClipboard>

extern QStringList rList;
extern QStringList fList;
extern QJsonObject chartobject;

extern QJsonObject LangConfObject;
extern QStringList selectedLangs;
extern QJsonObject NKeyObject;

extern int saveRecent(QString charcter);
extern int loadRecent(void);
extern int saveFavourite(void);
extern int loadFavourite(void);

extern int LoadNKeyConfig(void);
extern int LoadLangConfig(void);
extern int LoadLangOptions(void);

extern int saveStore(QString storeType);
extern int loadStore(QString storeType);

extern QClipboard *cb;
