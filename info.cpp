#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) : QDialog(parent), ui(new Ui::Info){
    ui->setupUi(this);

    connect(ui->pBClear, SIGNAL(clicked()), this, SLOT(slotClearList()));
}

Info::~Info(){
    delete ui;
}

void Info::slotGetString(QString mess){
    ui->lWEvents->addItem(mess);
    // ---
    if(ui->cBScroll->isChecked())
        ui->lWEvents->scrollToBottom();
    // ---
    if(ui->lWEvents->count() > lineLimit)
        ui->lWEvents->takeItem(0);
}

void Info::slotClearList(){
    ui->lWEvents->clear();
}
