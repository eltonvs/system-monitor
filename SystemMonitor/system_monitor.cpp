#include "system_monitor.h"
#include "ui_system_monitor.h"

SystemMonitor::SystemMonitor(QWidget *parent) : QMainWindow(parent), ui(new Ui::SystemMonitor) {
    ui->setupUi(this);

    // Define available colors
    availableColors.append(Qt::blue);
    availableColors.append(Qt::red);
    availableColors.append(Qt::green);
    availableColors.append(Qt::yellow);
    availableColors.append(Qt::cyan);
    availableColors.append(Qt::magenta);

    // Init Charts
    initChartCPU();
    initChartMemory();
    initChartCharge();
    initChartDischarge();
}

SystemMonitor::~SystemMonitor() {
    delete ui;
}

//
// Init Charts
//

void SystemMonitor::initChartCPU() {
    int nCPUs = icpu.getNumCPUs();

    // Create pens for each CPU and set his name
    for (int i = 0; i < nCPUs; i++) {
        ui->chartCPU->addGraph();
        ui->chartCPU->graph(i)->setPen(QPen(availableColors.at(i)));
        ui->chartCPU->graph(i)->setName("CPU " + QString::number(i + 1));
    }

    // Set X axis
    ui->chartCPU->xAxis->setTickLabels(true);
    ui->chartCPU->axisRect()->setupFullAxesBox();

    // Set y axis
    ui->chartCPU->yAxis->setRange(-0.5, 100.5);

    // Legend Settings
    ui->chartCPU->legend->setVisible(true);
    ui->chartCPU->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    ui->chartCPU->legend->setRowSpacing(0);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->chartCPU->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartCPU->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->chartCPU->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartCPU->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls SystemMonitor::updateChartCPU
    connect(&timerCPU, SIGNAL(timeout()), this, SLOT(updateChartCPU()));
    timerCPU.start(0); // Interval 0 means to refresh as fast as possible
}

void SystemMonitor::initChartMemory() {
    // Create pens for RAM
    ui->chartMemory->addGraph();
    ui->chartMemory->graph(0)->setPen(QPen(availableColors.at(0)));
    ui->chartMemory->graph(0)->setName("RAM");

    // Create pens for Swap
    ui->chartMemory->addGraph();
    ui->chartMemory->graph(1)->setPen(QPen(availableColors.at(1)));
    ui->chartMemory->graph(1)->setName("Swap");

    // Set X axis
    ui->chartMemory->xAxis->setTickLabels(true);
    ui->chartMemory->axisRect()->setupFullAxesBox();

    // Set y axis
    ui->chartMemory->yAxis->setRange(-0.5, 100.5);

    // Legend Settings
    ui->chartMemory->legend->setVisible(true);
    ui->chartMemory->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    ui->chartMemory->legend->setRowSpacing(0);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->chartMemory->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartMemory->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->chartMemory->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartMemory->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls SystemMonitor::updatechartMemory
    connect(&timerMemory, SIGNAL(timeout()), this, SLOT(updateChartMemory()));
    timerMemory.start(0); // Interval 0 means to refresh as fast as possible
}

void SystemMonitor::initChartCharge() {
    // Create pen for Charge
    ui->chartCharge->addGraph();
    ui->chartCharge->graph(0)->setPen(QPen(availableColors.at(0)));
    ui->chartCharge->graph(0)->setName("Carga (%)");

    // Set X axis
    ui->chartCharge->xAxis->setTickLabels(true);
    ui->chartCharge->axisRect()->setupFullAxesBox();

    // Set y axis
    ui->chartCharge->yAxis->setRange(-0.5, 100.5);

    // Legend Settings
    ui->chartCharge->legend->setVisible(true);
    ui->chartCharge->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    ui->chartCharge->legend->setRowSpacing(0);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->chartCharge->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartCharge->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->chartCharge->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartCharge->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls SystemMonitor::updatechartCharge
    connect(&timerCharge, SIGNAL(timeout()), this, SLOT(updateChartCharge()));
    timerCharge.start(0); // Interval 0 means to refresh as fast as possible
}

void SystemMonitor::initChartDischarge() {
    // Create pen for Charge
    ui->chartDischarge->addGraph();
    ui->chartDischarge->graph(0)->setPen(QPen(availableColors.at(1)));
    ui->chartDischarge->graph(0)->setName("Tempo de Descarga");

    // Set X axis
    ui->chartDischarge->xAxis->setTickLabels(true);
    ui->chartDischarge->axisRect()->setupFullAxesBox();

    // Set y axis
    ui->chartDischarge->yAxis->setRange(-0.5, 600.5);

    // Legend Settings
    ui->chartDischarge->legend->setVisible(true);
    ui->chartDischarge->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    ui->chartDischarge->legend->setRowSpacing(0);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->chartDischarge->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartDischarge->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->chartDischarge->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartDischarge->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls SystemMonitor::updatechartDischarge
    connect(&timerDischarge, SIGNAL(timeout()), this, SLOT(updateChartDischarge()));
    timerDischarge.start(0); // Interval 0 means to refresh as fast as possible
}

//
// Update Charts
//

void SystemMonitor::updateChartCPU() {
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    // add data to lines:
    icpu.calculate();
    for (int i = 0; i < icpu.getNumCPUs(); i++)
        ui->chartCPU->graph(i)->addData(key, icpu.getAverageCPU(i)*100.0);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->chartCPU->xAxis->setRange(key, 60, Qt::AlignRight);
    ui->chartCPU->replot();
}

void SystemMonitor::updateChartMemory() {
    im.calculate();

    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    // add data to lines:
    ui->chartMemory->graph(0)->addData(key, im.getRAMPercentage()*100.0);
    ui->chartMemory->graph(1)->addData(key, im.getSwapPercentage()*100.0);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->chartMemory->xAxis->setRange(key, 60, Qt::AlignRight);
    ui->chartMemory->replot();
}

void SystemMonitor::updateChartCharge() {
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    ib.calculate();

    // add data to lines:
    ui->chartCharge->graph(0)->addData(key, ib.getCurrCharge());

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->chartCharge->xAxis->setRange(key, 60, Qt::AlignRight);
    ui->chartCharge->replot();
}

void SystemMonitor::updateChartDischarge() {
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    ib.calculate();

    // add data to lines:
    ui->chartDischarge->graph(0)->addData(key, ib.getDischargeTime());

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->chartDischarge->xAxis->setRange(key, 60, Qt::AlignRight);
    ui->chartDischarge->replot();
}
