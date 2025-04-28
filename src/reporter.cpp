#include "reporter.h"
#include "../include/collector.h"
#include <fstream>
#include <iostream>

void saveReportAsJSON(const std::string& filename) {
    float cpuUsage = getCPUUsage();
    float ramUsage = getRAMUsage();
    float diskUsage = getDiskUsage("/");

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Dosya oluşturulamadı: " << filename << std::endl;
        return;
    }

    file << "{" << std::endl;
    file << "  \"cpu\": " << cpuUsage << "," << std::endl;
    file << "  \"ram\": " << ramUsage << "," << std::endl;
    file << "  \"disk\": " << diskUsage << std::endl;
    file << "}" << std::endl;

    file.close();
    std::cout << "JSON raporu başarıyla oluşturuldu: " << filename << std::endl;
}

void saveReportAsCSV(const std::string& filename) {
    float cpuUsage = getCPUUsage();
    float ramUsage = getRAMUsage();
    float diskUsage = getDiskUsage("/");

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Dosya oluşturulamadı: " << filename << std::endl;
        return;
    }

    file << "Type,Usage" << std::endl;
    file << "CPU," << cpuUsage << std::endl;
    file << "RAM," << ramUsage << std::endl;
    file << "Disk," << diskUsage << std::endl;

    file.close();
    std::cout << "CSV raporu başarıyla oluşturuldu: " << filename << std::endl;
}

