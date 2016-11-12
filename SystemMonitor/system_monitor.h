#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <QMainWindow>
#include <QTimer>
#include <thread>
#include <chrono>
#include "info_battery.h"
#include "info_cpu.h"
#include "info_memory.h"

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

 private slots:
    void updateChartCPU(QVector<double>);
    void updateChartCharge(int);
    void updateChartDischarge(double);
    void updateChartMemory(double, double);

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
    QVector<QColor> availableColors;

    InfoBattery ib;
    InfoCPU icpu;
    InfoMemory im;
};

#endif // SYSTEM_MONITOR_H
