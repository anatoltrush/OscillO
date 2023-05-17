#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent) : QDialog(parent), ui(new Ui::Player){
    ui->setupUi(this);

    connect(ui->pBChooseFile, &QPushButton::clicked, this, &Player::slotChooseFile);
    connect(ui->pBLoadFile, &QPushButton::clicked, this, &Player::slotLoadFile);
    connect(ui->lEInpFile, &QLineEdit::textChanged, this, &Player::slotPathChanged);
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
        QMessageBox::critical(this, "Error", "'" + pathFileLog + "'\nFile not found");
        ui->pBarLoad->setValue(0);
        return;
    }
    else{
        std::ifstream fileStream(pathFileLog.toLocal8Bit()); // + cyrillic
        int stringCount = std::count(std::istreambuf_iterator<char>(fileStream),
                                     std::istreambuf_iterator<char>(), '\n');
        std::vector<QString> dataStrings = readStrings(file, stringCount); // ...loading...
        makeFrames(dataStrings);
    }
}

void Player::slotOneBack(){
    //int a = 5;
}

void Player::slotPlay(){
    //int a = 5;
}

void Player::slotOneForw(){
    //int a = 5;
}

std::vector<QString> Player::readStrings(QFile &file, int linesAmount){
    std::vector<QString> vecStrings;
    ui->pBarLoad->setValue(0);
    uint8_t percent = 0;
    uint8_t tempPer = 0;
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        vecStrings.push_back(line);
        percent = (vecStrings.size() / (float)linesAmount) * 100;
        if(percent != tempPer){
            ui->pBarLoad->setValue(percent);
            tempPer = percent;
        }
    }

    return vecStrings;
}

void Player::makeFrames(const std::vector<QString> &strings){
    if(strings.size() % 2 != 0){
        ui->pBLoadFile->setStyleSheet("background-color: red");
        QMessageBox::critical(this, "Error","Bad file data (%2 size != 0)");
        return;
    }
    // --- programm state ---
    QJsonDocument jDoc = QJsonDocument::fromJson(strings.at(0).toUtf8());
    QJsonObject jObj;
    if(!jDoc.isNull()){
        if(jDoc.isObject()) jObj = jDoc.object();
        else{
            ui->pBLoadFile->setStyleSheet("background-color: red");
            QMessageBox::critical(this, "Error","Json document is not an object");
            return;
        }
    }
    else{
        ui->pBLoadFile->setStyleSheet("background-color: red");
        QMessageBox::critical(this, "Error","Invalid JSON (json is null)");
        return;
    }
    // TODO: send to MainWindow

    // --- all data ---
    frames.clear();
    uint8_t gap = 2;
    std::vector<QString> vecTemp(strings.size() - gap);
    std::copy(strings.begin() + gap, strings.end(), vecTemp.begin());
    // ---
    Frame locFrame;
    for (size_t i = 0; i < vecTemp.size(); i++) {
        QList listData = vecTemp[i].split("\t");
        if(i == 0 || i % 2 == 0){
            if(listData.size() != 3){ // wrong data
                ui->pBLoadFile->setStyleSheet("background-color: red");
                QMessageBox::critical(this, "Error","Bad file data (header of data)");
                return;
            }
            locFrame.timeStamp = listData.at(0).toDouble();
            locFrame.channelNum = listData.at(1).toUShort();
            locFrame.deviceIndex = listData.at(2).toUShort();
        }
        else{
            locFrame.payload.clear();
            for (const auto& val : qAsConst(listData))
                if(!val.isEmpty())
                    locFrame.payload.push_back(val.toUInt());
            frames.push_back(locFrame);
        }
    }

    // ---
    ui->pBLoadFile->setStyleSheet("background-color: green");
}
