#include <QStringList>
#include <QJsonObject>

extern QStringList langList;
extern QStringList selectedLangs;

extern QJsonObject NKeyObject;

extern int SaveNKeyConfig(void);
extern int SaveLangConfig(void);
extern int LoadNKeyConfig(void);
extern int LoadLangConfig(void);
extern int LoadLangOptions(void);

