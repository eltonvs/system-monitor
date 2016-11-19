#ifndef INFOBATTERY_H
#define INFOBATTERY_H

#include <QFile>
#include <QHash>
#include <QVector>
#include <QStringList>

class InfoBattery {
 public:
    InfoBattery() {}
    void calculate();
    int getCurrCharge();
    double getDischargeTime();

 private:
    bool openFile();

    QString fileData;
    int currCharge = 0;
    long dischargeTime = 0;
};

#endif // INFOBATTERY_H
