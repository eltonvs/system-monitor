#include "system_monitor.h"
#include "ui_system_monitor.h"

SystemMonitor::SystemMonitor(QWidget *parent) : QMainWindow(parent), ui(new Ui::SystemMonitor) {
    ui->setupUi(this);
}

SystemMonitor::~SystemMonitor() {
    delete ui;
}
