#include "collector.h"
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <sys/statvfs.h>
#include <iostream>
#include <algorithm>
#include <dirent.h>

float getCPUUsage() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);

    std::istringstream ss(line);

    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long idleTime = idle + iowait;
    long nonIdle = user + nice + system + irq + softirq + steal;
    long total = idleTime + nonIdle;

    // İlk ölçüm değerleri
    long total1 = total;
    long idle1 = idleTime;

    // 100ms bekle
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // İkinci ölçüm
    file.clear();
    file.seekg(0);
    file.close();
    file.open("/proc/stat");
    std::getline(file, line);
    std::istringstream ss2(line);
    ss2 >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    idleTime = idle + iowait;
    nonIdle = user + nice + system + irq + softirq + steal;
    total = idleTime + nonIdle;

    long total2 = total;
    long idle2 = idleTime;

    float totald = total2 - total1;
    float idled = idle2 - idle1;

    float CPU_Percentage = (totald - idled) / totald * 100.0;

    return CPU_Percentage;

   
}

float getRAMUsage() {
    std::ifstream meminfo("/proc/meminfo");
    std::string label;
    long totalMem = 0, freeMem = 0, buffers = 0, cached = 0;

    while (meminfo >> label) {
        if (label == "MemTotal:") meminfo >> totalMem;
        else if (label == "MemFree:") meminfo >> freeMem;
        else if (label == "Buffers:") meminfo >> buffers;
        else if (label == "Cached:") meminfo >> cached;
    }
    meminfo.close();

    long usedMem = totalMem - freeMem - buffers - cached;
    return (float)usedMem / totalMem * 100.0f;
}

// RAM kullanımını döndüren fonksiyon

float getDiskUsage(const std::string& path) {
    struct statvfs stat;

    if (statvfs(path.c_str(), &stat) != 0) {
        // Hata olursa -1 döneriz
        return -1.0f;
    }

    unsigned long total = stat.f_blocks * stat.f_frsize;
    unsigned long free = stat.f_bfree * stat.f_frsize;
    unsigned long used = total - free;

    if (total == 0) return 0.0f;

    float diskUsagePercentage = (static_cast<float>(used) / total) * 100.0f;
    return diskUsagePercentage;
}
// Disk kullanımını döndüren fonksiyon

void getNetworkUsage() {
    std::ifstream file("/proc/net/dev");
    std::string line;

    // Skip the first 2 lines as they contain headers
    std::getline(file, line);
    std::getline(file, line);

    std::cout << "Network Interface Usage:" << std::endl;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string interfaceName;
        unsigned long receiveBytes, transmitBytes;
        ss >> interfaceName;

        // Remove the ":" character from the interface name
        if (interfaceName.back() == ':') {
            interfaceName.pop_back();
        }

        ss >> receiveBytes;
        
        // Skip fields to reach transmitBytes (8 fields to skip)
        for (int i = 0; i < 7; ++i) {
            ss >> transmitBytes;
        }
        ss >> transmitBytes; // 9th field is transmitBytes

        std::cout << "  - " << interfaceName 
                  << " | Received: " << receiveBytes / 1024 << " KB"
                  << " | Transmitted: " << transmitBytes / 1024 << " KB" 
                  << std::endl;
    }
}
// Ağ kullanımını döndüren fonksiyon
struct ProcessInfo {
    int pid;
    std::string name;
    long unsigned int cpuTime;
};

void showTopProcesses() {
    std::vector<ProcessInfo> processes;

    DIR* proc = opendir("/proc");
    if (proc == nullptr) {
        std::cerr << "Failed to open /proc directory!" << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(proc)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            std::string dirName = entry->d_name;
            if (std::all_of(dirName.begin(), dirName.end(), ::isdigit)) {
                int pid = std::stoi(dirName);
                std::ifstream statFile("/proc/" + dirName + "/stat");
                if (statFile.is_open()) {
                    std::string comm;
                    char dummy;
                    long unsigned int utime, stime;
                    
                    statFile >> pid >> comm;
                    for (int i = 0; i < 11; ++i) {
                        statFile >> dummy; // atla
                    }
                    statFile >> utime >> stime;
                    
                    ProcessInfo procInfo;
                    procInfo.pid = pid;
                    procInfo.name = comm;
                    procInfo.cpuTime = utime + stime;
                    
                    processes.push_back(procInfo);
                }
            }
        }
    }
    closedir(proc);

    // CPU kullanımına göre sırala
    std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.cpuTime > b.cpuTime;
    });

    std::cout << "Top 5 CPU User Processes:" << std::endl;
    for (int i = 0; i < 5 && i < processes.size(); ++i) {
        std::cout << i+1 << ". PID: " << processes[i].pid
                  << " | İsim: " << processes[i].name
                  << " | CPU Time: " << processes[i].cpuTime << std::endl;
    }
}

CPUUsageStats getCPUUsageDetailed() {
    std::ifstream statFile("/proc/stat");
    std::string line;
    CPUUsageStats stats{};

    if (std::getline(statFile, line)) {
        std::istringstream iss(line);
        std::string cpuLabel;
        unsigned long user, nice, system, idle, iowait, irq, softirq;

        iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq;

        unsigned long totalIdle = idle + iowait;
        unsigned long totalSystem = system + irq + softirq;
        unsigned long totalUser = user + nice;
        unsigned long total = totalUser + totalSystem + totalIdle;

        stats.user = (float)totalUser / total * 100.0f;
        stats.system = (float)totalSystem / total * 100.0f;
        stats.idle = (float)totalIdle / total * 100.0f;
        stats.total = 100.0f - stats.idle;
    }

    return stats;
}

RAMUsageStats getRAMUsageDetailed() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    RAMUsageStats stats{};

    if (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        unsigned long totalRam;
        iss >> key >> totalRam;
        stats.total = totalRam / 1024.0f;  // Kilobyte'dan GB'ye çevir
    }

    // Kullanılan RAM
    std::getline(meminfo, line);  // 2. satır: "MemFree"
    std::istringstream(line) >> std::ws >> line;  // RAM'in boş kısmı
    unsigned long freeRam;
    std::istringstream(line) >> freeRam;
    stats.used = stats.total - freeRam / 1024.0f;

    // Swap kullanımı
    while (std::getline(meminfo, line)) {
        if (line.find("SwapTotal") != std::string::npos) {
            std::istringstream(line) >> std::ws >> line;
            unsigned long swapTotal;
            std::istringstream(line) >> swapTotal;
            stats.swapTotal = swapTotal / 1024.0f;
        }
        if (line.find("SwapFree") != std::string::npos) {
            std::istringstream(line) >> std::ws >> line;
            unsigned long swapFree;
            std::istringstream(line) >> swapFree;
            stats.swapUsed = stats.swapTotal - swapFree / 1024.0f;
            break;
        }
    }

    return stats;
}