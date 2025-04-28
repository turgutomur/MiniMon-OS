#include "cli.h"
#include "../include/collector.h"
#include "../include/reporter.h"
#include <iostream>

void handleUserCommand(const std::string& command) {
    if (command == "cpu") {
        CPUUsageStats stats = getCPUUsageDetailed();
        std::cout << "CPU Usage: %" << stats.total 
                  << " (User: %" << stats.user
                  << ", System: %" << stats.system
                  << ", Idle: %" << stats.idle
                  << ")" << std::endl;
    }
    else if (command == "mem") {
        RAMUsageStats stats = getRAMUsageDetailed();
        std::cout << "Total RAM: " << stats.total << " GB" << std::endl;
        std::cout << "Used: " << stats.used << " GB" << std::endl;
        std::cout << "Swap: " << stats.swapUsed << " GB / " << stats.swapTotal << " GB" << std::endl;
    }
    else if (command == "disk") {
        float diskUsage = getDiskUsage("/");
        std::cout << "Disk Usage ('/'): %" << diskUsage << std::endl;
    }
    else if (command == "net") {
        getNetworkUsage();
    }
    else if (command == "top5") {
        showTopProcesses();
    }
    else if (command == "report_json") {
        saveReportAsJSON("report.json");
    }
    else if (command == "report_csv") {
        saveReportAsCSV("report.csv");
    }
    else {
        std::cout << "Unknown command! Available commands: cpu, mem, disk, net, top5, report_json, report_csv" << std::endl;
    }
}
