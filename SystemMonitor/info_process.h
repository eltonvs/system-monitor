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
    double mem_usage;
    double CPU_usage;
    std::string name;
};

class InfoProcess {
 public:
    InfoProcess();
    void update_json();
    // void kill_process(int pid);

 private:
    void populate_process_list();
    std::string get_proc_name(int pid);
    std::string json_father(std::vector<Proc> &v);
    std::string json_child(Proc &p);
    std::map<int, std::vector<Proc>> processes;
};

#endif // INFOPROCESS_H
