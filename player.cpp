#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent) : QDialog(parent), ui(new Ui::Player){
    ui->setupUi(this);

    connect(ui->pBChooseFile, &QPushButton::clicked, this, &Player::slotChooseFile);
    connect(ui->pBLoadFile, &QPushButton::clicked, this, &Player::slotLoadFile);
}

Player::~Player(){
    delete ui;
}

void Player::slotChooseFile(){

}

void Player::slotLoadFile(){

}
