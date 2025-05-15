MINIMON - System Resource Monitor
----------------------------------

Overview:
MINIMON is a lightweight system resource monitoring tool that runs on the command line. It provides real-time insights into CPU, RAM, Disk, and Network usage and can generate reports in JSON and CSV formats. Additionally, it can list the top 5 processes consuming the most CPU.

Features:
- CPU Usage (User/System/Idle breakdown)
- RAM Usage (Used/Empty/Swap)
- Disk Usage
- Network Traffic (Received/Transmitted)
- Top 5 CPU-consuming processes
- Command-line interface (CLI)
- Export reports to JSON/CSV

Installation:
1. Make sure you have g++ and CMake installed.
2. Navigate to the project directory:
   cd minimon
3. Create a build directory and build the project:
   mkdir build
   cd build
   cmake ..
   make

Usage:
Run the application with the following commands:

  ./minimon cpu      # Shows CPU usage
  ./minimon mem      # Shows RAM usage
  ./minimon disk     # Shows Disk usage
  ./minimon net      # Shows Network usage
  ./minimon top5     # Lists top 5 CPU-consuming processes
  ./minimon report json   # Saves a report in JSON format
  ./minimon report csv    # Saves a report in CSV format

Notes:
- For best results, run the application with appropriate system permissions.
- Reports will be saved in the build directory by default.

Author:
omurturgut
