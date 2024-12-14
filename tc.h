#ifndef TC_H
#define	TC_H

// Foreground Colors
#define RESET "\033[0m"         // Reset to default
#define BLACK "\033[30m"        // Black
#define RED "\033[31m"          // Red
#define GREEN "\033[32m"        // Green
#define YELLOW "\033[33m"       // Yellow
#define BLUE "\033[34m"         // Blue
#define MAGENTA "\033[35m"      // Magenta
#define CYAN "\033[36m"         // Cyan
#define WHITE "\033[37m"        // White

// Background Colors
#define BG_BLACK "\033[40m"     // Black background
#define BG_RED "\033[41m"       // Red background
#define BG_GREEN "\033[42m"     // Green background
#define BG_YELLOW "\033[43m"    // Yellow background
#define BG_BLUE "\033[44m"      // Blue background
#define BG_MAGENTA "\033[45m"   // Magenta background
#define BG_CYAN "\033[46m"      // Cyan background
#define BG_WHITE "\033[47m"     // White background

// Text Styles
#define BOLD "\033[1m"          // Bold text
#define UNDERLINE "\033[4m"     // Underlined text
#define REVERSED "\033[7m"      // Reversed foreground and background colors

#define clear_screen() puts("\x1b[2j")

#endif