
#include <QTimer>
#include <QDateTime>

#include "timestampshowdialog.h"
#include "ui_timestampshowdialog.h"

TimestampShowDialog::TimestampShowDialog(uint64_t timestamp, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TimestampShowDialog)
{
    ui->setupUi(this);

    QDateTime dt = QDateTime::currentDateTime();
    ui->dateTimeEditNow->setDateTime(dt);
    ui->lineEditNowts->setText(QString::number(dt.toSecsSinceEpoch()));
    ui->lineEditNowmts->setText(QString::number(dt.toMSecsSinceEpoch()));
    ui->lineEditNowmmts->setText(QString::number(dt.toMSecsSinceEpoch()*1000));

    QDateTime maxDt(QDate(9999,12,31), QTime(23,59,59));
    QDateTime inDt = QDateTime::fromSecsSinceEpoch(timestamp);
    if(inDt.isValid() && inDt <= maxDt) {
        ui->lineEditInts->setText(QString::number(timestamp));
        ui->dateTimeEditIndt->setDateTime(inDt);
    } else {
        ui->lineEditInts->setText("0");
    }

    QDateTime inmDt = QDateTime::fromMSecsSinceEpoch(timestamp);
    if(inmDt.isValid() && inmDt <= maxDt) {
        ui->lineEditInmts->setText(QString::number(timestamp));
        ui->dateTimeEditInmdt->setDateTime(inmDt);
    } else {
        ui->lineEditInmts->setText("0");
    }

    QDateTime inmmDt = QDateTime::fromMSecsSinceEpoch(timestamp/1000);
    if(inmmDt.isValid() && inmmDt <= maxDt) {
        ui->lineEditInmmts->setText(QString::number(timestamp));
        ui->dateTimeEditInmmdt->setDateTime(inmmDt);
    } else {
        ui->lineEditInmmts->setText("0");
    }

    connect(ui->pushButtonClose, &QPushButton::clicked, this, [&](){
        accept();
    });

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, [&](){
        QDateTime dt = QDateTime::currentDateTime();
        ui->dateTimeEditNow->setDateTime(dt);
        ui->lineEditNowts->setText(QString::number(dt.toSecsSinceEpoch()));
        ui->lineEditNowmts->setText(QString::number(dt.toMSecsSinceEpoch()));
        ui->lineEditNowmmts->setText(QString::number(dt.toMSecsSinceEpoch()*1000));
    });
    m_timer->start();
}

TimestampShowDialog::~TimestampShowDialog()
{
    delete m_timer;
    delete ui;
}
