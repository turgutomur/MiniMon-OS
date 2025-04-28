#ifndef REPORTER_H
#define REPORTER_H

#include <string>


void saveReportAsJSON(const std::string& filename);
void saveReportAsCSV(const std::string& filename);

#endif // REPORTER_H