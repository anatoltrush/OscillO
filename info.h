#ifndef INFO_H
#define INFO_H

#include <QDialog>

namespace Ui {class Info;}

class Info : public QDialog{
    Q_OBJECT

public:
    explicit Info(QWidget *parent = nullptr);
    ~Info();

private:
    Ui::Info *ui;

public slots:
    void slotGetString(QString mess);

private slots:
    void slotClearList();
};

#endif // INFO_H
