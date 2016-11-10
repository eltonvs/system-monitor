#ifndef INFOCPU_H
#define INFOCPU_H

#include <QFile>
#include <QHash>
#include <QVector>

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
    QVector<int> usageCPU;
    QVector<int> totCPU;
};

#endif // INFOCPU_H
