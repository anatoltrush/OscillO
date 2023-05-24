#include "mainwindow.h"

#include <QApplication>

/**
 * @mainpage ---=== OscillO ===---
 * How it works:...\n
 *         __                      ______ \n
 *        |UI|                  <-|Player|\n
 *            \                /          \n
 *             \   __________ /           \n
 *              <-|MainWindow|            \n
 *             /              \           \n
 * __________ /                \   ______ \n
 *|TechLogger|                  <-|Server|\n
 */

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
