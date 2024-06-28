
#include "timestamp.h"

#include <QApplication>
#include <QMap>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QRegularExpression>
#include <QTranslator>
#include <QLocale>
#include <QDebug>

#include "timestampshowdialog.h"

int Timestamp::init(QMap<QString, QString> params, QWidget *parent)
{
    Q_UNUSED(params);

    m_action = new QAction(tr("Timestamp"), parent);
    connect(m_action, &QAction::triggered, [&,parent](){
        QMessageBox::information(parent, tr("Timestamp"), 
            tr(
                "Timestamp\nThis is a plugin to convert timestamps."
            ));
    });

    return 0;
}

QList<QAction *> Timestamp::terminalContextAction(QString selectedText, QString workingDirectory, QMenu *parentMenu)
{
    Q_UNUSED(workingDirectory);
    if(selectedText.isEmpty()) {
        return QList<QAction *>();
    }

    QList<QAction *> actions;
    QLocale locale;
    bool isUInt64Number = false;
    uint64_t number = 0;
    if(selectedText.startsWith("0x")) {
        QString testText = selectedText.mid(2);
        number = testText.toULongLong(&isUInt64Number,16);
    } else {
        number = selectedText.toULongLong(&isUInt64Number,10);
        if(!isUInt64Number) {
            number = locale.toULongLong(selectedText,&isUInt64Number);
        }
    }
    if(isUInt64Number) {
        QAction *textStatistics = new QAction(tr("Show Timestamp"), parentMenu);
        actions.append(textStatistics);
        connect(textStatistics, &QAction::triggered, this, [=](){
            TimestampShowDialog dialog(number,parentMenu);
            dialog.setWindowTitle(tr("Show Timestamp"));
            dialog.exec();
        });
    }

    return actions;
}

void Timestamp::setLanguage(const QLocale &language,QApplication *app) {
    static QTranslator *qtTranslator = nullptr;
    if(qtTranslator == nullptr) {
        qtTranslator = new QTranslator(app);
    } else {
        app->removeTranslator(qtTranslator);
        delete qtTranslator;
        qtTranslator = new QTranslator(app);
    }
    switch(language.language()) {
    case QLocale::Chinese:
        if(qtTranslator->load(":/lang/timestamp_zh_CN.qm"))
            app->installTranslator(qtTranslator);
        break;
    default:
    case QLocale::English:
        if(qtTranslator->load(":/lang/timestamp_en_US.qm"))
            app->installTranslator(qtTranslator);
        break;
    }
}

void Timestamp::retranslateUi() {
    m_action->setText(tr("Timestamp"));
}
