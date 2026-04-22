
#include <QTimer>
#include <QDateTime>
#include <QClipboard>
#include <QStyle>

#include "timestampshowdialog.h"
#include "ui_timestampshowdialog.h"

TimestampShowDialog::TimestampShowDialog(uint64_t timestamp, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TimestampShowDialog)
{
    ui->setupUi(this);

    // Compute a fixed width that fits exactly 3 numeric characters in current font/style.
    auto setThreeDigitFixedWidth = [](QLineEdit *lineEdit) {
        const int textWidth = lineEdit->fontMetrics().horizontalAdvance(QStringLiteral("000"));
        const int frameWidth = lineEdit->style()->pixelMetric(QStyle::PM_DefaultFrameWidth, nullptr, lineEdit);
        const QMargins margins = lineEdit->textMargins();
        const int totalWidth = textWidth + margins.left() + margins.right() + (frameWidth * 2) + 4;
        lineEdit->setFixedWidth(totalWidth);
    };
    setThreeDigitFixedWidth(ui->lineEditInmdtMs);
    setThreeDigitFixedWidth(ui->lineEditInmmdtMs);
    setThreeDigitFixedWidth(ui->lineEditInmmdtUs);

    auto formatDateTime = [](const QDateTime &dt) {
        return dt.toString("yyyy-MM-dd hh:mm:ss");
    };

    QDateTime dt = QDateTime::currentDateTime();
    ui->dateTimeEditNow->setDateTime(dt);
    ui->lineEditNowts->setText(QString::number(dt.toSecsSinceEpoch()));
    ui->lineEditNowmts->setText(QString::number(dt.toMSecsSinceEpoch()));
    ui->lineEditNowmmts->setText(QString::number(dt.toMSecsSinceEpoch()*1000));

    uint64_t dts = 0;
    uint64_t dtms = 0;
    uint64_t dtmms = 0;
    QDateTime maxDt(QDate(9999,12,31), QTime(23,59,59));
    QDateTime inDt = QDateTime::fromSecsSinceEpoch(timestamp);
    if(inDt.isValid() && inDt <= maxDt) {
        ui->lineEditInts->setText(QString::number(timestamp));
        ui->dateTimeEditIndt->setDateTime(inDt);
        dts = timestamp;
    }

    QDateTime inmDt = QDateTime::fromMSecsSinceEpoch(timestamp);
    if(inmDt.isValid() && inmDt <= maxDt) {
        ui->lineEditInmts->setText(QString::number(timestamp));
        ui->dateTimeEditInmdt->setDateTime(inmDt);
        ui->lineEditInmdtMs->setText(QString::number(timestamp % 1000ULL));
        dtms = timestamp;
    }

    QDateTime inmmDt = QDateTime::fromMSecsSinceEpoch(timestamp/1000);
    if(inmmDt.isValid() && inmmDt <= maxDt) {
        ui->lineEditInmmts->setText(QString::number(timestamp));
        ui->dateTimeEditInmmdt->setDateTime(inmmDt);
        const uint64_t inSecondUs = timestamp % 1000000ULL;
        ui->lineEditInmmdtMs->setText(QString::number(inSecondUs / 1000ULL));
        ui->lineEditInmmdtUs->setText(QString::number(inSecondUs % 1000ULL));
        dtmms = timestamp;
    }

    connect(ui->toolButtonCPNd, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(formatDateTime(ui->dateTimeEditNow->dateTime()));
    });
    connect(ui->toolButtonCPNt, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(ui->lineEditNowts->text());
    });
    connect(ui->toolButtonCPNmt, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(ui->lineEditNowmts->text());
    });
    connect(ui->toolButtonCPNmmt, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(ui->lineEditNowmmts->text());
    });
    connect(ui->toolButtonCPtd, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(formatDateTime(ui->dateTimeEditIndt->dateTime()));
    });
    connect(ui->toolButtonCPmtd, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(formatDateTime(ui->dateTimeEditInmdt->dateTime()) + "." + ui->lineEditInmdtMs->text());
    });
    connect(ui->toolButtonCPmmtd, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(formatDateTime(ui->dateTimeEditInmmdt->dateTime()) + "." + ui->lineEditInmmdtMs->text() + "." + ui->lineEditInmmdtUs->text());
    });
    connect(ui->toolButtonCPts, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(ui->lineEditDiffCurTime->text());
    });
    connect(ui->toolButtonCPmts, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(ui->lineEditDiffCurTimem->text());
    });
    connect(ui->toolButtonCPmmts, &QToolButton::clicked, this,[&](){
        QApplication::clipboard()->setText(ui->lineEditDiffCurTimemm->text());
    });

    connect(ui->pushButtonClose, &QPushButton::clicked, this, [&](){
        accept();
    });

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, [&,dts,dtms,dtmms](){
        QDateTime cdt = QDateTime::currentDateTime();
        uint64_t cdts = cdt.toSecsSinceEpoch();
        uint64_t cdtms = cdt.toMSecsSinceEpoch();
        uint64_t cdtmms = cdt.toMSecsSinceEpoch()*1000;
        ui->dateTimeEditNow->setDateTime(cdt);
        ui->lineEditNowts->setText(QString::number(cdts));
        ui->lineEditNowmts->setText(QString::number(cdtms));
        ui->lineEditNowmmts->setText(QString::number(cdtmms));
        uint64_t diffdt = cdts>dts ? cdts-dts : dts-cdts;
        uint64_t diffdtm = cdtms>dtms ? cdtms-dtms : dtms-cdtms;
        uint64_t diffdtmm = cdtmms>dtmms ? cdtmms-dtmms : dtmms-cdtmms;
        if(dts == 0 && dtms == 0 && dtmms == 0) {
            ui->lineEditDiffCurTime->setText(QString::number(diffdt/(60ULL*60ULL*24ULL*365ULL)) + "y : " + QString::number(diffdt/(60ULL*60ULL*24ULL*30ULL)) + "m : " + QString::number(diffdt/(60ULL*60ULL*7ULL*30ULL)) + "w : " +QString::number(diffdt/(60ULL*60ULL*30ULL)) + "d : " + QString::number(diffdt/(60ULL*60ULL)) + "h : " + QString::number(diffdt/60ULL) + "m : " + QString::number(diffdt) + "s");
            ui->lineEditDiffCurTimem->setText(QString::number(diffdtm/(60ULL*60ULL*24ULL*365ULL*1000ULL)) + "y : " + QString::number(diffdtm/(60ULL*60ULL*24ULL*30ULL*1000ULL)) + "m : " + QString::number(diffdtm/(60ULL*60ULL*7ULL*30ULL*1000ULL)) + "w : " +QString::number(diffdtm/(60ULL*60ULL*30ULL*1000ULL)) + "d : " + QString::number(diffdtm/(60ULL*60ULL*1000ULL)) + "h : " + QString::number(diffdtm/(60ULL*1000ULL)) + "m : " + QString::number(diffdtm/1000ULL) + "s");
            ui->lineEditDiffCurTimemm->setText(QString::number(diffdtmm/(60ULL*60ULL*24ULL*365ULL*1000ULL*1000ULL)) + "y : " + QString::number(diffdtmm/(60ULL*60ULL*24ULL*30ULL*1000ULL*1000ULL)) + "m : " + QString::number(diffdtmm/(60ULL*60ULL*7ULL*30ULL*1000ULL*1000ULL)) + "w : " +QString::number(diffdtmm/(60ULL*60ULL*30ULL*1000ULL*1000ULL)) + "d : " + QString::number(diffdtmm/(60ULL*60ULL*1000ULL*1000ULL)) + "h : " + QString::number(diffdtmm/(60ULL*1000ULL*1000ULL)) + "m : " + QString::number(diffdtmm/1000ULL/1000ULL) + "s");
        } else {
            if(dts != 0)
                ui->lineEditDiffCurTime->setText(QString::number(diffdt/(60ULL*60ULL*24ULL*365ULL)) + "y : " + QString::number(diffdt/(60ULL*60ULL*24ULL*30ULL)) + "m : " + QString::number(diffdt/(60ULL*60ULL*7ULL*30ULL)) + "w : " +QString::number(diffdt/(60ULL*60ULL*30ULL)) + "d : " + QString::number(diffdt/(60ULL*60ULL)) + "h : " + QString::number(diffdt/60ULL) + "m : " + QString::number(diffdt) + "s");
            if(dtms != 0)
                ui->lineEditDiffCurTimem->setText(QString::number(diffdtm/(60ULL*60ULL*24ULL*365ULL*1000ULL)) + "y : " + QString::number(diffdtm/(60ULL*60ULL*24ULL*30ULL*1000ULL)) + "m : " + QString::number(diffdtm/(60ULL*60ULL*7ULL*30ULL*1000ULL)) + "w : " +QString::number(diffdtm/(60ULL*60ULL*30ULL*1000ULL)) + "d : " + QString::number(diffdtm/(60ULL*60ULL*1000ULL)) + "h : " + QString::number(diffdtm/(60ULL*1000ULL)) + "m : " + QString::number(diffdtm/1000ULL) + "s");
            if(dtmms != 0)
                ui->lineEditDiffCurTimemm->setText(QString::number(diffdtmm/(60ULL*60ULL*24ULL*365ULL*1000ULL*1000ULL)) + "y : " + QString::number(diffdtmm/(60ULL*60ULL*24ULL*30ULL*1000ULL*1000ULL)) + "m : " + QString::number(diffdtmm/(60ULL*60ULL*7ULL*30ULL*1000ULL*1000ULL)) + "w : " +QString::number(diffdtmm/(60ULL*60ULL*30ULL*1000ULL*1000ULL)) + "d : " + QString::number(diffdtmm/(60ULL*60ULL*1000ULL*1000ULL)) + "h : " + QString::number(diffdtmm/(60ULL*1000ULL*1000ULL)) + "m : " + QString::number(diffdtmm/1000ULL/1000ULL) + "s");
        }
    });
    m_timer->start();
}

TimestampShowDialog::~TimestampShowDialog()
{
    delete m_timer;
    delete ui;
}
