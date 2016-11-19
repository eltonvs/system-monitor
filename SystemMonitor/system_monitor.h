#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <QMainWindow>
#include <QTimer>
#include <thread>
#include <chrono>
#include "info_battery.h"
#include "info_cpu.h"
#include "info_memory.h"
#include "info_process.h"

namespace Ui {
class SystemMonitor;
}

class SystemMonitor : public QMainWindow {
    Q_OBJECT

 public:
    explicit SystemMonitor(QWidget *parent = 0);
    ~SystemMonitor();
    void run();
    void initChartCPU();
    void initChartCharge();
    void initChartDischarge();
    void initChartMemory();

 signals:
    void slotUpdateChartCPU(QVector<double>);
    void slotUpdateChartCharge(int);
    void slotUpdateChartDischarge(double);
    void slotUpdateChartMemory(double, double);
    void slotUpdateChartProcess(int);

 private slots:
    void updateChartCPU(QVector<double>);
    void updateChartCharge(int);
    void updateChartDischarge(double);
    void updateChartMemory(double, double);
    void updateChartProcess(int metric = 0);

    void on_pbUpdate_clicked();
    void on_pbKill_clicked();

private:
    void chartCPU();
    void chartCharge();
    void chartDischarge();
    void chartMemory();

    Ui::SystemMonitor *ui;

    std::thread threadCPU;
    std::thread threadCharge;
    std::thread threadDischarge;
    std::thread threadMemory;

    bool fst;
    unsigned updateTime;
    QVector<QColor> availableColors;

    InfoBattery ib;
    InfoCPU icpu;
    InfoMemory im;
    InfoProcess ip;
};

#endif // SYSTEM_MONITOR_H
