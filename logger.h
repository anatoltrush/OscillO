#ifndef LOGGER_H
#define LOGGER_H

#include <QDialog>

namespace Ui {class Logger;}

class Logger : public QDialog{
    Q_OBJECT

public:
    explicit Logger(QWidget *parent = nullptr);
    ~Logger();

private:
    Ui::Logger *ui;
};

#endif // LOGGER_H
