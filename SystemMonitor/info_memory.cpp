#include "info_memory.h"

#include <iostream>
#include <fstream>
#include <QHash>
#include <QTextStream>

void InfoMemory::calculate() {
    this->openFile();

    QStringList el, dataList = fileData.split("\n");
    QHash<QString, int> data;

    for (int i = 0; i < dataList.size() - 1; i++) {
        el = dataList.at(i).split(":");
        data.insert(el.at(0).simplified(), el.at(1).simplified().remove("kB").toInt());
    }

    totRAM = data.value("MemTotal");
    totSwap = data.value("SwapTotal");
    usageRAM = totRAM  - data.value("MemFree");
    usageSwap = totSwap - data.value("SwapFree");

}

bool InfoMemory::openFile() {
    QFile memoryData("/proc/meminfo");

    if (memoryData.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fileData = memoryData.readAll();
        memoryData.close();

        return true;
    }

    return false;
}

double InfoMemory::getRAMPercentage() {
    return (double)usageRAM/totRAM;
}

double InfoMemory::getSwapPercentage() {
    return (double)usageSwap/totSwap;
}
