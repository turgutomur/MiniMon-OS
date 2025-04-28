#ifndef COLLECTOR_H
#define COLLECTOR_H
using namespace std;

#include <string>

struct CPUUsageStats {
    float total;
    float user;
    float system;
    float idle;
};

struct RAMUsageStats {
    float total;      // Toplam RAM
    float used;       // Kullanılan RAM
    float swapUsed;   // Kullanılan Swap
    float swapTotal;  // Toplam Swap
};

float getCPUUsage();  // CPU kullanımını döndüren fonksiyonun bildirimi
CPUUsageStats getCPUUsageDetailed();
float getRAMUsage();   // RAM kullanımını döndüren fonksiyonun bildirimi
RAMUsageStats getRAMUsageDetailed();
float getDiskUsage(const string& path); // Disk kullanımını döndüren fonksiyonun bildirimi
void getNetworkUsage(); // Ağ kullanımını döndüren fonksiyonun bildirimi
void handleUserCommand(const string& command); // Kullanıcı komutlarını işleyen fonksiyonun bildirimi
void showTopProcesses();

#endif // COLLECTOR_H


