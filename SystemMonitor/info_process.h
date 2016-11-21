#ifndef INFOPROCESS_H
#define INFOPROCESS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

struct Proc {
    int pid;
    int ppid;
    double memory;
    double cpu;
    int threads;
    std::string name;
};

class InfoProcess {
 public:
    InfoProcess();
    void update_json(int metric = -1);
    void kill_process(int pid);
    std::string PATH;

 private:
    void populate_process_list();
    std::string json_father(std::vector<Proc> &v, bool first = true);
    std::string json_child(Proc &p, bool first = true);

    std::map<int, std::vector<Proc>> processes;
    int metric_type;
};

#endif // INFOPROCESS_H
