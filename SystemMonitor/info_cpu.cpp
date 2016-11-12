#include "info_cpu.h"
#include <iostream>
#include <unistd.h>

InfoCPU::InfoCPU() {
    numCPUs = 0;

    if (openFile()) {
        QStringList el, dataList = fileData.split("\n");
        for (int i = 1; i < dataList.size() - 1; i++) {
            el = dataList.at(i).split(" ");
            if (el.at(0).contains("cpu")) {
                numCPUs++;
                prevUsageCPU.append(el.at(1).toInt() + el.at(2).toInt() + el.at(3).toInt());
                prevTotCPU.append(el.at(1).toInt() + el.at(2).toInt() + el.at(3).toInt() + el.at(4).toInt() + el.at(5).toInt());
            } else {
                break;
            }
        }

        usageCPU.resize(numCPUs);
        totCPU.resize(numCPUs);
    }
}

void InfoCPU::calculate() {
    if (openFile()) {
        QStringList el, dataList = fileData.split("\n");
        for (int i = 1; i < dataList.size() - 1; i++) {
            if (i > numCPUs) break;
            el = dataList.at(i).split(" ");
            prevUsageCPU[i - 1] = usageCPU[i - 1];
            prevTotCPU[i - 1] = totCPU[i - 1];
            usageCPU[i - 1] = el.at(1).toInt() + el.at(2).toInt() + el.at(3).toInt();
            totCPU[i - 1] = usageCPU.at(i - 1) + el.at(4).toInt() + el.at(5).toInt();
        }
    }
}

int InfoCPU::getNumCPUs() {
    return numCPUs;
}

double InfoCPU::getAverageCPU(int i) {
    return (usageCPU.at(i) - prevUsageCPU.at(i))*100.0/(totCPU.at(i) - prevTotCPU.at(i));
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
