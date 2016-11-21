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

    // Start some things
    fst = true;
    updateTime = 250;

    // Set InfoProcess path
    ip.PATH = "/home/elton/repo/system_monitor/SystemMonitor/";

    qRegisterMetaType<QVector<double> >("QVector<double>");
    connect(this, SIGNAL(slotUpdateChartCPU(QVector<double>)), SLOT(updateChartCPU(QVector<double>)));
    connect(this, SIGNAL(slotUpdateChartCharge(int)), SLOT(updateChartCharge(int)));
    connect(this, SIGNAL(slotUpdateChartDischarge(double)), SLOT(updateChartDischarge(double)));
    connect(this, SIGNAL(slotUpdateChartMemory(double,double)), SLOT(updateChartMemory(double, double)));
    connect(this, SIGNAL(slotUpdateChartProcess(int)), SLOT(updateChartProcess(int)));

    // Init Charts
    initChartCPU();
    initChartMemory();
    initChartCharge();
    initChartDischarge();

    // Start Process Area
    ip.update_json();
    ui->wvProcess->load(QUrl::fromLocalFile(QString::fromStdString(ip.PATH) + "index.html"));

    run();
}

SystemMonitor::~SystemMonitor() {
    delete ui;
}

void SystemMonitor::run() {
    if (fst) {
        threadCPU       = std::thread(&SystemMonitor::chartCPU, this);
        threadCharge    = std::thread(&SystemMonitor::chartCharge, this);
        threadDischarge = std::thread(&SystemMonitor::chartDischarge, this);
        threadMemory    = std::thread(&SystemMonitor::chartMemory, this);
        fst = false;
    }
}

//
// Init Charts
//

void SystemMonitor::initChartCPU() {
    int nCPUs = icpu.getNumCPUs();

    // Create pens for each CPU and set his name
    for (int i = 0; i < nCPUs; i++) {
        ui->chartCPU->addGraph();
        ui->chartCPU->graph(i)->setPen(QPen(availableColors.at(i%availableColors.size())));
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
    ui->chartCharge->yAxis->setRange(-0.1, 100.1);

    // Legend Settings
    ui->chartCharge->legend->setVisible(true);
    ui->chartCharge->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    ui->chartCharge->legend->setRowSpacing(0);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->chartCharge->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartCharge->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->chartCharge->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartCharge->yAxis2, SLOT(setRange(QCPRange)));
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
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    ui->chartDischarge->yAxis->setTicker(timeTicker);
    ui->chartDischarge->yAxis->setRange(5, 60*60*5 + 5);
    timeTicker->setTimeFormat("%h hora(s)");

    // Legend Settings
    ui->chartDischarge->legend->setVisible(true);
    ui->chartDischarge->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    ui->chartDischarge->legend->setRowSpacing(0);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->chartDischarge->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartDischarge->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->chartDischarge->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->chartDischarge->yAxis2, SLOT(setRange(QCPRange)));
}

//
// Loops
//

void SystemMonitor::chartCPU() {
    while (true) {
        icpu.calculate();
        QVector<double> vet;
        for (int i = 0; i < icpu.getNumCPUs(); i++)
            vet.append(icpu.getAverageCPU(i));
        emit(slotUpdateChartCPU(vet));
        std::this_thread::sleep_for(std::chrono::milliseconds(updateTime));
    }
}

void SystemMonitor::chartCharge() {
    while (true) {
        ib.calculate();
        emit(slotUpdateChartCharge(ib.getCurrCharge()));
        std::this_thread::sleep_for(std::chrono::milliseconds(updateTime));
    }
}

void SystemMonitor::chartDischarge() {
    while (true) {
        emit(slotUpdateChartDischarge(ib.getDischargeTime()));
        std::this_thread::sleep_for(std::chrono::milliseconds(updateTime));
    }
}

void SystemMonitor::chartMemory() {
    while (true) {
        im.calculate();
        emit(slotUpdateChartMemory(im.getRAMPercentage(), im.getSwapPercentage()));
        std::this_thread::sleep_for(std::chrono::milliseconds(updateTime));
    }
}

//
// Update Charts
//

void SystemMonitor::updateChartCPU(QVector<double> cpus) {
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    // add data to lines:
    for (int i = 0; i < cpus.size(); i++)
        ui->chartCPU->graph(i)->addData(key, cpus.at(i));

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->chartCPU->xAxis->setRange(key, 60, Qt::AlignRight);
    ui->chartCPU->replot();
}

void SystemMonitor::updateChartMemory(double ram, double swap) {
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    // add data to lines:
    ui->chartMemory->graph(0)->addData(key, ram*100.0);
    ui->chartMemory->graph(1)->addData(key, swap*100.0);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->chartMemory->xAxis->setRange(key, 60, Qt::AlignRight);
    ui->chartMemory->replot();
}

void SystemMonitor::updateChartCharge(int charge) {
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    // add data to lines:
    ui->chartCharge->graph(0)->addData(key, charge);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->chartCharge->xAxis->setRange(key, 60, Qt::AlignRight);
    ui->chartCharge->replot();
}

void SystemMonitor::updateChartDischarge(double discharge) {
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    // add data to lines:
    ui->chartDischarge->graph(0)->addData(key, discharge);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->chartDischarge->xAxis->setRange(key, 60, Qt::AlignRight);
    ui->chartDischarge->replot();
}

void SystemMonitor::updateChartProcess(int metric) {
    ip.update_json(metric);
    ui->wvProcess->repaint();
    ui->wvProcess->reload();
    ui->wvProcess->update();
}

//
// Click Events
//

void SystemMonitor::on_pbUpdate_clicked() {
    emit(updateChartProcess(ui->cbFilter->currentIndex()));
}

void SystemMonitor::on_pbKill_clicked() {
    int pid = ui->lePID->text().toInt();

    if (pid != 0) {
        ip.kill_process(pid);
    }

    emit(updateChartProcess());
}
