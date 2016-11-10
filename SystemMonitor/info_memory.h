#ifndef INFOMEMORY_H
#define INFOMEMORY_H

#include <QFile>
#include <QHash>
#include <QVector>

class InfoMemory {
 public:
    InfoMemory() {}
    void calculate();
    double getRAMPercentage();
    double getSwapPercentage();

 private:
    bool openFile();

    QString fileData;
    int totRAM = 0;
    int totSwap = 0;
    int usageRAM = 0;
    int usageSwap = 0;
};

#endif // INFOMEMORY_H
