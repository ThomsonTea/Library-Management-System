#include "table.h"

void tableFormat(tabulate::Table table) {
    // Start of table format
    // Format the header row (first row)
    table[0].format()
        .font_color(tabulate::Color::white)              // Set font color to white
        .font_background_color(tabulate::Color::cyan)    // Set background color to cyan
        .padding_top(0.5)
        .padding_bottom(0.5)
        .border_color(tabulate::Color::white);           // Set border color to white

    // Format the data rows
    for (auto& row : table) {
        row.format()
            .font_style({ tabulate::FontStyle::bold })      // Set font style to bold
            .font_align(tabulate::FontAlign::center)      // Center align text
            .corner("*")                                  // No corner style
            .border_color(tabulate::Color::white);        // Set border color to white
    }

    // Print the table after applying the format
    std::cout << table << std::endl;
}

void paraTableFormat(tabulate::Table table)
{
    // Start of table format
    // Format the header row (first row)
    table.format()
        .padding_top(0.5)
        .padding_bottom(0.5)
        .font_align(tabulate::FontAlign::center)         // Center-align header
        .border_color(tabulate::Color::white);           // Set border color to white

    // Format the data rows
    for (size_t i = 0; i < table.size(); ++i)
    {
        table[i][0].format() // Left column (parameters)
            .font_color(tabulate::Color::cyan)
            .font_align(tabulate::FontAlign::left)       // Align parameters to the left
            .font_style({ tabulate::FontStyle::bold })   // Bold font for parameters
            .border_color(tabulate::Color::white);       // Set border color to white

        table[i][1].format() // Right column (values)
            .font_align(tabulate::FontAlign::right)      // Align values to the right
            .border_color(tabulate::Color::white);       // Set border color to white
    }

    // Print the table after applying the format
    std::cout << table << std::endl;
}

void pivotTableFormat(tabulate::Table table)
{
    // Start of table format
    // Format the header row (first row)
    table.format()
        .padding_top(0.5)
        .padding_bottom(0.5)
        .font_align(tabulate::FontAlign::center)         // Center-align header
        .border_color(tabulate::Color::white);           // Set border color to white

    // Apply cyan background and white text to the entire header row (first row)
    for (size_t i = 0; i < table[0].size(); ++i) // Loop through all header columns
    {
        table[0][i].format()
            .font_color(tabulate::Color::white)         // Set font color to white
            .font_background_color(tabulate::Color::cyan)  // Set background color to cyan
            .font_align(tabulate::FontAlign::center)     // Align header text in center
            .padding_top(0.5)
            .padding_bottom(0.5)
            .border_color(tabulate::Color::white);      // Set border color to white
    }

    // Format the data rows
    for (size_t i = 1; i < table.size(); ++i)  // Start from row 1 to avoid affecting the header
    {
        table[i][0].format() // Left column (parameters)
            .font_color(tabulate::Color::yellow)
            .font_align(tabulate::FontAlign::left)       // Align parameters to the left
            .font_style({ tabulate::FontStyle::bold })   // Bold font for parameters
            .border_color(tabulate::Color::white);       // Set border color to white

        table[i][1].format() // Right column (values)
            .font_align(tabulate::FontAlign::right)      // Align values to the right
            .border_color(tabulate::Color::white);       // Set border color to white
    }

    // Print the table after applying the format
    std::cout << table << std::endl;
}
