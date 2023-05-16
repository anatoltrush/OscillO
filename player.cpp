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
    pathFileLog = QFileDialog::getOpenFileName(this, tr("Choose log file"), "", tr("Log files (*.log)"));
    if(!pathFileLog.isEmpty())
        ui->lEInpFile->setText(pathFileLog);
}

void Player::slotLoadFile(){
    if(ui->lEInpFile->text().isEmpty()){
        QMessageBox::information(this, "Input from log file","Empty input data");
        return;
    }
    // --- --- ---

    QFile file(pathFileLog);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->pBLoadFile->setStyleSheet("background-color: red");
        QMessageBox::critical(this, "Error","File not found");
        ui->pBarLoad->setValue(0);
        return;
    }
    else{
        std::ifstream fileStream(pathFileLog.toLocal8Bit()); // + cyrillic
        int stringCount = std::distance(std::istream_iterator<std::string>(fileStream),
                                        std::istream_iterator<std::string>()) / 3; // 3 words in line
        readData(file, stringCount); // ...loading...
        ui->pBLoadFile->setStyleSheet("background-color: green");
    }
}

void Player::slotOneBack(){
    //int a = 5;
}

void Player::slotPause(){
    //int a = 5;
}

void Player::slotPlay(){
    //int a = 5;
}

void Player::slotOneForw(){
    //int a = 5;
}

void Player::readData(QFile &file, int linesAmount){
    frames.clear();
    ui->pBarLoad->setValue(0);
    int percent = 0;
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        //QStringList strList1 = line.split(u' ', Qt::SkipEmptyParts);
        //if(strList1.size() < 3) break;

        /*Frame frame;
        QString tmStmp = strList1[0];
        tmStmp.remove(0, 1);
        tmStmp.chop(1);
        tmStmp.remove(QChar('.'));
        frame.timeStamp = tmStmp.toDouble();
        canLine.canNum = strList1[1];

        QStringList strList2 = strList1[2].split(u'#', Qt::SkipEmptyParts);
        if(strList2.size() < 2) break;
        frame.messId = strList2[0];
        frame.messData = strList2[1];
        canLines.push_back(frame);*/

        //percent = (frames.size() / (float)linesAmount) * 100;
        //ui->pBarLoad->setValue(percent);
    }
}
