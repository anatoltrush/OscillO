#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent) : QDialog(parent), ui(new Ui::Player){
    ui->setupUi(this);
}

Player::~Player(){
    delete ui;
}
