#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <vector>
#include <map>
#include "dbConnection.h" // Assuming you already have a dbConnection class

class Report
{
public:
    // Constructor and Destructor
    Report();
    ~Report();

    // Generate an annual report
    void generateAnnualReport(const std::string& year);
    void reportingMenu();
    void saveToCSV(const std::vector<std::map<std::string, int>>& data, const std::string& year);

private:
    // Helper methods
    void fetchReportData(dbConnection* db, const std::string& year, std::vector<std::map<std::string, int>>& data);
    void displayReportTable(const std::vector<std::map<std::string, int>>& data);
    void generateGraph(const std::vector<std::map<std::string, int>>& data, const std::string& year);
};

#endif // REPORT_H
