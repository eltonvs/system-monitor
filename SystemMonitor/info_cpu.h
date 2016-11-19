#ifndef INFOCPU_H
#define INFOCPU_H

#include <QFile>
#include <QHash>
#include <QVector>
#include <QStringList>

class InfoCPU {
 public:
    InfoCPU();
    void calculate();
    int getNumCPUs();
    double getAverageCPU(int i);

 private:
    bool openFile();

    QString fileData;
    int numCPUs;
    QVector<long> usageCPU;
    QVector<long> prevUsageCPU;
    QVector<long> totCPU;
    QVector<long> prevTotCPU;
};

#endif // INFOCPU_H
