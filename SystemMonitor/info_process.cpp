#include "info_process.h"

InfoProcess::InfoProcess() {
    populate_process_list();
    PATH = "/home/elton/repo/system_monitor/SystemMonitor/";
}

void InfoProcess::update_json() {
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

void InfoProcess::populate_process_list() {
    // Clear processes map
    processes.clear();

    // Create file with all data about process
    system("ps xao pid,ppid,%mem,\%cpu > proc_list.dat");

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

        ss >> a.pid >> a.ppid >> a.mem_usage >> a.CPU_usage;
        a.name = get_proc_name(a.pid);

        // Insert on map if has a name
        if (a.name != "") {
            processes[a.ppid].push_back(a);
        }
    }

    // Close file and delete him
    file.close();
    system("rm proc_list.dat");
}

std::string InfoProcess::get_proc_name(int pid) {
    // Read file that contains the process name
    std::ifstream p("/proc/" + std::to_string(pid) + "/comm");

    std::string r = "";

    // Verify if the file was opened
    if (p.is_open()) {
        // Save the file content on return variable and close file
        std::getline(p, r);
        p.close();
    }

    return r;
}

std::string InfoProcess::json_father(std::vector<Proc> &v, bool first) {
    // Verify if the vector isn't empty
    if (v.empty()) return "";

    std::ostringstream oss;
    int i = 0;

    if (!first) oss << ",";
    oss << "{\"name\": \"" << v[0].name << "\", \"children\": [";
    for (Proc p : v) {
        oss << json_child(p, i++ == 0);
    }
    oss << "]}";

    return oss.str();
}

std::string InfoProcess::json_child(Proc &p, bool first) {
    std::ostringstream oss;
    if (!first) oss << ",";
    oss << "{\"name\": \"" << p.name << "\", \"size\": " << (int)(p.CPU_usage * 1000) + 500
        << ", \"pid\": " << p.pid << "}";

    return oss.str();
}
