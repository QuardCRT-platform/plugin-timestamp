#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "plugininterface.h"

#define PLUGIN_NAME    "Timestamp"
#define PLUGIN_VERSION "0.0.3"
#define PLUGIN_WEBSITE "https://github.com/QuardCRT-platform/plugin-timestamp"

class Timestamp : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.quardCRT.PluginInterface" FILE "./plugininterface/plugininterface.json")
    Q_INTERFACES(PluginInterface)

public:
    Timestamp() : m_action(nullptr) {}
    virtual ~Timestamp() {}

    int init(QMap<QString, QString> params, QWidget *parent);

    void setLanguage(const QLocale &language,QApplication *app);
    void retranslateUi();
    QString name() { return PLUGIN_NAME; }
    QString version() { return PLUGIN_VERSION; }

    QMap<QString,void *> metaObject() {
        QMap<QString,void *> ret;
        ret.insert("QAction", (void *)m_action);
        ret.insert("website", (void *)(&m_website));
        return ret;
    }

    QMenu *terminalContextMenu(QString selectedText, QString workingDirectory, QMenu *parentMenu) {Q_UNUSED(selectedText);Q_UNUSED(workingDirectory);Q_UNUSED(parentMenu); return nullptr;}
    QList<QAction *> terminalContextAction(QString selectedText, QString workingDirectory, QMenu *parentMenu);

private:
    QAction *m_action;
    QString m_website = QString(PLUGIN_WEBSITE);
};

#endif /* TIMESTAMP_H_ */
