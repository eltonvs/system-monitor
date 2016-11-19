#include "info_process.h"

InfoProcess::InfoProcess() {
    populate_process_list();
    PATH = "/home/elton/repo/system_monitor/SystemMonitor/";
    metric_type = 0;
}

void InfoProcess::update_json(int metric) {
    if (metric >= 0)
        metric_type = metric;

    populate_process_list();

    std::ofstream json_file(PATH + "processos.json", std::ofstream::trunc);

    int i = 0;
    json_file << "{\"name\": \"processos\", \"children\": [";
    for (auto proc_pair : processes) {
        json_file << json_father(proc_pair.second, (i++ == 0));
    }
    json_file << "]}";

    json_file.close();
}

void InfoProcess::kill_process(int pid) {
    std::string cmd = "kill -9 " + std::to_string(pid);
    system(cmd.c_str());
}

void InfoProcess::populate_process_list() {
    // Clear processes map
    processes.clear();

    // Create file with all data about process
    system("ps xao pid,ppid,pmem,pcpu,thcount,comm > proc_list.dat");

    // Open file
    std::ifstream file("proc_list.dat");
    if (!file.is_open()) {
        exit(1);
    }

    // Just a variable to keep the line content
    std::string line;

    // Ignore first line with header info
    std::getline(file, line);

    // Read all lines from file
    while (std::getline(file, line)) {
        Proc a;
        std::stringstream ss(line);

        ss >> a.pid >> a.ppid >> a.memory >> a.cpu >> a.threads;
        if (ss >> a.name) {
            processes[a.ppid].push_back(a);
        }
    }

    // Close file and delete him
    file.close();
    system("rm proc_list.dat");
}

std::string InfoProcess::json_father(std::vector<Proc> &v, bool first) {
    // Verify if the vector isn't empty
    if (v.empty()) return "";

    std::ostringstream oss;
    int i = 0;

    if (!first) oss << ",";
    oss << "{\"name\":\"" << v[0].name << "\",\"children\":[";
    for (Proc p : v) {
        oss << json_child(p, i++ == 0);
    }
    oss << "]}";

    return oss.str();
}

std::string InfoProcess::json_child(Proc &p, bool first) {
    int metric;

    if (metric_type == 0) {         // CPU
        metric = (p.cpu + 0.1)*1000;
    } else if (metric_type == 1) {  // Thread
        metric = p.threads;
    } else if (metric_type == 2) {  // Memory
        metric = (p.memory + 0.1)*1000;
    } else {                        // Gourmet = average between cpu, threads and memory
        metric = (p.threads*100 + (p.cpu + 0.1)*1000 + (p.memory + 0.1)*1000)/3;
    }

    std::ostringstream oss;
    if (!first) oss << ",";
    oss << "{\"name\":\"" << p.name << "\",\"size\":" << metric << ",\"pid\":" << p.pid << "}";

    return oss.str();
}
