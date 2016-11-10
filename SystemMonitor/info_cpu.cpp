#include "info_cpu.h"
#include <iostream>

InfoCPU::InfoCPU() {
    numCPUs = -1;
    if (openFile()) {
        QStringList dataList = fileData.split("\n");
        for (int i = 0; i < dataList.size() - 1; i++) {
            QString line = dataList.at(i);
            if (line[0] == 'c' && line[1] == 'p' && line[2] == 'u')
                numCPUs++;
            else break;
        }

        usageCPU.resize(numCPUs);
        totCPU.resize(numCPUs);
    }
}

void InfoCPU::calculate() {
    if (openFile()) {
        QStringList el, dataList = fileData.split("\n");
        for (int i = 0; i < dataList.size() - 1; i++) {
            if (i == 0) continue;
            if (i > numCPUs) break;
            el = dataList.at(i).split(" ");
            usageCPU[i - 1] = el.at(1).toInt() + el.at(2).toInt() + el.at(3).toInt() + el.at(4).toInt();
            totCPU[i - 1] = usageCPU.at(i - 1) + el.at(5).toInt();
            std::cout << usageCPU[i - 1] << " | " << totCPU[i - 1] << "\n";
        }
    }
}

int InfoCPU::getNumCPUs() {
    return numCPUs;
}

double InfoCPU::getAverageCPU(int i) {
    return (double)usageCPU.at(i)/totCPU.at(i);
}

bool InfoCPU::openFile() {
    QFile cpuData("/proc/stat");

    if (cpuData.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fileData = cpuData.readAll();
        cpuData.close();

        return true;
    }

    return false;
}
