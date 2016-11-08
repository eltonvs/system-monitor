#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <QMainWindow>
#include <QTimer>
#include <thread>

namespace Ui {
class SystemMonitor;
}

class SystemMonitor : public QMainWindow {
    Q_OBJECT

 public:
    explicit SystemMonitor(QWidget *parent = 0);
    ~SystemMonitor();
    void initChartCPU();
    void initChartMemory();
    void initChartCharge();
    void initChartDischarge();

 private slots:
    void updateChartCPU();
    void updateChartMemory();
    void updateChartCharge();
    void updateChartDischarge();

 private:
    Ui::SystemMonitor *ui;
    QTimer timerCPU;
    QTimer timerMemory;
    QTimer timerCharge;
    QTimer timerDischarge;
    QVector<QColor> availableColors;
};

#endif // SYSTEM_MONITOR_H
