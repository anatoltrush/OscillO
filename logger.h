#ifndef LOGGER_H
#define LOGGER_H

#include <QDialog>
#include <QTimer>

namespace Ui {class Logger;}

class Logger : public QDialog{
    Q_OBJECT

public:
    explicit Logger(QWidget *parent = nullptr);
    ~Logger();

private:
    Ui::Logger *ui;

    QTimer* timer = nullptr;
    uint8_t indicCount = 9;

private slots:
    void slotTimerTick();
    void slotAddSymb();
    void slotChooseDir();
};

#endif // LOGGER_H
