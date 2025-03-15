#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "quartz/quartz.hpp"

const std::string QUARTZ_VERSION = "v1.0";

#ifdef _WIN32
// For Windows, use _getch() from conio.h
inline char my_getch() { return _getch(); }
#else
// For Unix, disable line buffering and echo.
char my_getch() {
    char buf = 0;
    struct termios old = { 0 };
    tcgetattr(0, &old);
    struct termios newt = old;
    newt.c_lflag &= ~ICANON;  // disable buffered I/O
    newt.c_lflag &= ~ECHO;    // disable echo
    tcsetattr(0, TCSANOW, &newt);
    read(0, &buf, 1);
    tcsetattr(0, TCSANOW, &old);
    return buf;
}
#endif

// Global command history storage
static std::vector<std::string> commandHistory; // Stores previous commands
static size_t historyIndex = 0;                  // Index for history navigation

// Refreshes the input line with the current command
void refreshLine(const std::string& input) {
    std::cout << "\rquartz> " << input << "    "; // clear line with extra spaces
    std::cout << "\rquartz> " << input;             // reprint prompt and input
    std::cout.flush();
}

// Autocompletes filenames. If input starts with "./", that prefix is removed for matching and reattached.
void completeFilename(std::string& input) {
    std::vector<std::string> matches;
    std::string prefix = "";
    std::string base;

    if (input.rfind("./", 0) == 0) {
        prefix = "./";
        base = input.substr(2);
    }
    else {
        base = input;
    }

    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        if (entry.is_regular_file()) {
            std::string fname = entry.path().filename().string();
            if (fname.rfind(base, 0) == 0) { // prefix match
                matches.push_back(prefix + fname);
            }
        }
    }
    if (matches.empty())
        return;
    if (matches.size() == 1) {
        std::string suggestion = matches[0];
        std::string remainder = suggestion.substr(input.size());
        input += remainder;
        std::cout << remainder;
        return;
    }
    // Multiple matches: determine common prefix.
    std::string common = matches[0];
    for (const auto& match : matches) {
        size_t i = 0;
        while (i < common.size() && i < match.size() && common[i] == match[i])
            i++;
        common = common.substr(0, i);
    }
    if (common.size() > input.size()) {
        std::string addition = common.substr(input.size());
        input += addition;
        std::cout << addition;
    }
    else {
        std::cout << "\n";
        for (const auto& match : matches)
            std::cout << match << "   ";
        std::cout << "\nquartz> " << input;
    }
}

// Reads a line of input character-by-character, handling special keys:
//   - Tab: filename autocompletion
//   - Backspace: delete last character
//   - Arrow keys (up/down): navigate command history
std::string readLine() {
    std::string input;
    char ch;
    while (true) {
        ch = my_getch();

#ifdef _WIN32
        if (ch == -32) {
            char arrow = my_getch();
            if (arrow == 72) { // Up arrow
                if (!commandHistory.empty() && historyIndex > 0) {
                    historyIndex--;
                    input = commandHistory[historyIndex];
                    std::cout << "\033[2K\r";  // Clear the line and move cursor to the beginning
                    refreshLine(input);
                    std::cout.flush(); // Ensure flush after refreshing
                }
                continue;
            }
            else if (arrow == 80) { // Down arrow
                if (historyIndex < commandHistory.size() - 1) {
                    historyIndex++;
                    input = commandHistory[historyIndex];
                    std::cout << "\033[2K\r";  // Clear the line and move cursor to the beginning
                    refreshLine(input);
                    std::cout.flush(); // Ensure flush after refreshing
                }
                else {
                    input = "";  // Clear the line when at the empty state
                    historyIndex = commandHistory.size(); // Reset to the "new command" state
                    std::cout << "\033[2K\r";  // Clear the line and move cursor to the beginning
                    refreshLine(input);  // Clear the input
                    std::cout.flush(); // Ensure flush after refreshing
                }
                continue;
            }
            else {
                continue;
            }
        }
#else
        if (ch == 27) {
            char seq1 = my_getch();
            if (seq1 == '[') {
                char seq2 = my_getch();
                if (seq2 == 'A') { // Up arrow
                    if (!commandHistory.empty() && historyIndex > 0) {
                        historyIndex--;
                        input = commandHistory[historyIndex];
                        std::cout << "\033[2K\r";  // Clear the line and move cursor to the beginning
                        refreshLine(input);
                        std::cout.flush(); // Ensure flush after refreshing
                    }
                    continue;
                }
                else if (seq2 == 'B') { // Down arrow
                    if (historyIndex < commandHistory.size() - 1) {
                        historyIndex++;
                        input = commandHistory[historyIndex];
                        std::cout << "\033[2K\r";  // Clear the line and move cursor to the beginning
                        refreshLine(input);
                        std::cout.flush(); // Ensure flush after refreshing
                    }
                    else {
                        input = "";  // Clear the line when at the empty state
                        historyIndex = commandHistory.size(); // Reset to the "new command" state
                        std::cout << "\033[2K\r";  // Clear the line and move cursor to the beginning
                        refreshLine(input);  // Clear the input
                        std::cout.flush(); // Ensure flush after refreshing
            }
                    continue;
        }
    }
}
#endif

#ifdef _WIN32
        if (ch == '\r') { // Enter key on Windows returns carriage return
#else
        if (ch == '\n' || ch == '\r') { // Unix: newline or carriage return
#endif
            std::cout << std::endl;
            break;
        }
        else if (ch == '\t') { // Handle Tab for autocomplete
            completeFilename(input);
        }
        else if (ch == 8 || ch == 127) { // Handle backspace
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b";
            }
        }
        else {
            input.push_back(ch);
            std::cout << ch;
        }
        std::cout.flush();
        }

    // Save command to history if not empty.
    if (!input.empty()) {
        commandHistory.push_back(input);
        historyIndex = commandHistory.size();
    }
    return input;
    }

// Executes a Quartz command.
void executeQuartzCommand(const std::string & command) {
    Quartz::run_code(command.c_str());
}

// Executes a Quartz file.
void executeQuartzFile(const std::string & filename) {
    Quartz::run_file(filename.c_str());
}

// Prints the shell header.
void printHeader() {
    std::cout << "========================\n";
    std::cout << "     Quartz Shell     \n";
    std::cout << "       " << QUARTZ_VERSION << "           \n";
    std::cout << "    by Mercury Dev     \n";
    std::cout << "========================\n";
}

// Main loop: reads user input and processes commands.
int main() {
    std::string input;
    printHeader();

    while (true) {
        std::cout << "quartz> ";
        input = readLine();

        if (input == "exit") {
            break;
        }
        else if (input == "help") {
            std::cout << "Available commands:\n";
            std::cout << "  help        - Show this help message\n";
            std::cout << "  exit        - Exit the Quartz Shell\n";
            std::cout << "  [cmd]       - Execute a Quartz command\n";
            std::cout << "  [filename]  - Execute commands from a file\n";
            continue;
        }
        // Execute as file if input starts with "./" or contains ".qz"
        else if ((input.size() >= 2 && input.substr(0, 2) == "./") ||
            (input.find(".qz") != std::string::npos)) {
            executeQuartzFile(input);
            continue;
        }
        else if (input.empty()) {
            continue;
        }
        else {
            executeQuartzCommand(input);
        }
    }

    std::cout << "Exiting Quartz Shell..." << std::endl;
    return 0;
}
