#include "Report.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include "tc.h"
#include "time.h"
#include <tabulate/table_internal.hpp>
#include <tabulate/table.hpp>
#include "table.h"

Report::Report() {}

Report::~Report() {}

void Report::generateAnnualReport(const std::string& year)
{   

    dbConnection* db = new dbConnection();
    std::vector<std::map<std::string, int>> data(12); // Data for 12 months
    system("cls");
    try
    {
        // Fetch report data from the database
        fetchReportData(db, year, data);

        // Display table in terminal
        displayReportTable(data);

        // Generate graph using GNUPlot
        generateGraph(data, year);

        saveToCSV(data, year);

        std::string command = "start annual_report_" + year + ".png";
        system(command.c_str());

        system("pause");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error generating annual report: " << e.what() << std::endl;
    }

    delete db; // Clean up database connection
}

void Report::fetchReportData(dbConnection* db, const std::string& year, std::vector<std::map<std::string, int>>& data)
{
    const std::vector<std::string> months = { "01", "02", "03", "04", "05", "06",
                                              "07", "08", "09", "10", "11", "12" };

    for (size_t i = 0; i < months.size(); ++i)
    {
        std::string month = months[i];

        // Query to get the count of returned books for the current month
        std::string returnedQuery = "SELECT COUNT(*) AS count FROM Loan "
            "WHERE bookStatus = 'Returned' AND "
            "DATE_FORMAT(borrow_date, '%Y-%m') = '" + year + "-" + month + "'";

        std::string damagedQuery = "SELECT COUNT(*) AS count FROM Loan "
            "WHERE bookStatus = 'Damaged' AND "
            "DATE_FORMAT(borrow_date, '%Y-%m') = '" + year + "-" + month + "'";

        std::string lostQuery = "SELECT COUNT(*) AS count FROM Loan "
            "WHERE bookStatus = 'Lost' AND "
            "DATE_FORMAT(borrow_date, '%Y-%m') = '" + year + "-" + month + "'";

        // Fetch results
        int returnedCount = db->fetchSingleResult(returnedQuery);
        int damagedCount = db->fetchSingleResult(damagedQuery);
        int lostCount = db->fetchSingleResult(lostQuery);

        // Store data
        data[i]["Returned"] = returnedCount;
        data[i]["Damaged"] = damagedCount;
        data[i]["Lost"] = lostCount;
        data[i]["Total"] = returnedCount + damagedCount + lostCount;
    }
}

void Report::displayReportTable(const std::vector<std::map<std::string, int>>& data) {
    tabulate::Table table;

    // Adding headers
    table.add_row({ "Month", "Returned", "Damaged", "Lost", "Total" });

    // Adding rows for each month (this example assumes data has 12 months of data)
    const std::vector<std::string> months = { "January", "February", "March", "April", "May", "June",
                                              "July", "August", "September", "October", "November", "December" };

    for (int i = 0; i < 12; ++i) {
        table.add_row({ months[i],
                       std::to_string(data[i].at("Returned")),
                       std::to_string(data[i].at("Damaged")),
                       std::to_string(data[i].at("Lost")),
                       std::to_string(data[i].at("Total")) });
    }

    // Print the table
    pivotTableFormat(table);
}

void Report::generateGraph(const std::vector<std::map<std::string, int>>& data, const std::string& year)
{
    // Get the current date and time from getCurrentTime
    std::string generatedDate = getCurrentTime();

    // Create a data file for GNUPlot
    std::ofstream dataFile("report_data.txt");
    const std::vector<std::string> months = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12" };

    if (!dataFile)
    {
        throw std::runtime_error("Error: Unable to create data file for GNUPlot.");
    }

    dataFile << "# Month Returned Damaged Lost Total\n";
    for (int i = 0; i < 12; ++i)
    {
        dataFile << months[i] << " "
            << data[i].at("Returned") << " "
            << data[i].at("Damaged") << " "
            << data[i].at("Lost") << " "
            << data[i].at("Total") << "\n";
    }
    dataFile.close();

    // Create a GNUPlot script file
    std::ofstream scriptFile("report_plot.plt");
    if (!scriptFile)
    {
        throw std::runtime_error("Error: Unable to create GNUPlot script file.");
    }

    // Include the generated date in the graph title
    scriptFile << "set terminal png size 1024,768\n";
    scriptFile << "set output 'annual_report_" << year << ".png'\n";
    scriptFile << "set title 'Annual Report - " << year << " (Generated on: " << generatedDate << ")'\n";
    scriptFile << "set xlabel 'Month'\n";
    scriptFile << "set ylabel 'Count'\n";
    scriptFile << "set grid\n";
    scriptFile << "set key outside\n";
    scriptFile << "set xtics 1\n";
    scriptFile << "plot 'report_data.txt' using 1:2 with lines title 'Returned', \\\n"
        << "     'report_data.txt' using 1:3 with lines title 'Damaged', \\\n"
        << "     'report_data.txt' using 1:4 with lines title 'Lost', \\\n"
        << "     'report_data.txt' using 1:5 with lines title 'Total'\n";
    scriptFile.close();

    // Execute GNUPlot script
    system("gnuplot report_plot.plt");
    std::cout << "Graph generated: annual_report_" << year << ".png\n";
}


void Report::reportingMenu()
{
    int reportSelected = 0;
    bool inReportingMenu = true;

    while (inReportingMenu)
    {
        system("cls");
        int yearSelected = 0;
        bool inYearSelection = true;
        std::cout << "Reporting Menu:\n\n";
        std::cout << (reportSelected == 0 ? "-> " : "   ") << (reportSelected == 0 ? BG_YELLOW : "") << "Annual Book Borrowing Report" << RESET << std::endl;
        std::cout << (reportSelected == 1 ? "-> " : "   ") << (reportSelected == 1 ? BG_RED : "") << "Return Back" << RESET << std::endl;

        std::cout << "\nUse arrow keys to navigate, press Enter to select, Esc to return to the main menu.\n";

        char c = _getch();
        switch (c)
        {
        case KEY_UP:
            reportSelected = (reportSelected - 1 + 2) % 2;
            break;
        case KEY_DOWN:
            reportSelected = (reportSelected + 1) % 2;
            break;
        case KEY_ENTER:
            switch (reportSelected)
            {
            case 0:


                while (inYearSelection)
                {
                    system("cls");
                    std::cout << "Select Year:\n\n";
                    std::cout << (yearSelected == 0 ? "-> " : "   ") << (yearSelected == 0 ? BG_YELLOW : "") << "2024" << RESET << std::endl;
                    std::cout << (yearSelected == 1 ? "-> " : "   ") << (yearSelected == 1 ? BG_YELLOW : "") << "2025" << RESET << std::endl;
                    std::cout << "\nUse arrow keys to navigate, press Enter to select.\n";

                    char yearKey = _getch();
                    switch (yearKey)
                    {
                    case KEY_UP:
                        yearSelected = (yearSelected - 1 + 2) % 2; // Toggle between 2024 and 2025
                        break;
                    case KEY_DOWN:
                        yearSelected = (yearSelected + 1) % 2; // Toggle between 2024 and 2025
                        break;
                    case KEY_ESC:
                        inYearSelection = false;
                        break;
                    case KEY_ENTER:
                        if (yearSelected == 0)
                            generateAnnualReport("2024"); // Generate for 2024
                        else
                            generateAnnualReport("2025"); // Generate for 2025
                        inYearSelection = false;
                        break;
                    }
                }
                break;
            case 1:
                inReportingMenu = false; // Exit to main menu
                break;
            }
            break;
        case KEY_ESC:
            inReportingMenu = false; // Exit to main menu
            break;
        }
    }
}

void Report::saveToCSV(const std::vector<std::map<std::string, int>>& data, const std::string& year)
{
    // Open a file stream
    std::ofstream file("annual_report_" + year + ".csv");
    if (!file)
    {
        throw std::runtime_error("Error: Unable to create CSV file.");
    }

    // Write the header row
    file << "Month,Returned,Damaged,Lost,Total\n";

    // Write the data rows for each month
    const std::vector<std::string> months = { "January", "February", "March", "April", "May", "June",
                                              "July", "August", "September", "October", "November", "December" };

    for (int i = 0; i < 12; ++i)
    {
        file << months[i] << ","
            << data[i].at("Returned") << ","
            << data[i].at("Damaged") << ","
            << data[i].at("Lost") << ","
            << data[i].at("Total") << "\n";
    }

    file.close();
    std::cout << "CSV file created successfully: annual_report_" + year + ".csv\n";
}
