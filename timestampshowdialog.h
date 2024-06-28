#ifndef TIMESTAMPSHOWDIALOG_H
#define TIMESTAMPSHOWDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class TimestampShowDialog;
}

class TimestampShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimestampShowDialog(uint64_t timestamp, QWidget *parent = nullptr);
    ~TimestampShowDialog();

private:
    Ui::TimestampShowDialog *ui;
    QTimer *m_timer;
};

#endif // TIMESTAMPSHOWDIALOG_H
