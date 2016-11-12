#include "info_battery.h"

void InfoBattery::calculate() {
    if (openFile()) {
        QStringList data = fileData.split(" ");
        QString percent = data.at(3);

        currCharge = percent.remove("%,").toInt();
        QStringList time = data.at(4).split(":");
        dischargeTime = time.at(0).toInt()*60 + time.at(1).toInt();
    }
}

int InfoBattery::getCurrCharge() {
    return currCharge;
}

long InfoBattery::getDischargeTime() {
    return dischargeTime;
}

bool InfoBattery::openFile() {
    system("acpi -b > battery_info.dat");
    QFile batteryData("battery_info.dat");

    if (batteryData.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fileData = batteryData.readAll();
        batteryData.close();
        system("rm battery_info.dat");

        return true;
    }

    return false;
}
