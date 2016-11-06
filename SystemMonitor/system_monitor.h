#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <QMainWindow>

namespace Ui {
class SystemMonitor;
}

class SystemMonitor : public QMainWindow {
    Q_OBJECT

 public:
    explicit SystemMonitor(QWidget *parent = 0);
    ~SystemMonitor();

 private:
    Ui::SystemMonitor *ui;
};

#endif // SYSTEM_MONITOR_H
