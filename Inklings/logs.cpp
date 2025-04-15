#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include <chrono>

struct LogEntry {
    std::string rawLine;
    std::chrono::system_clock::time_point timestamp;
};

// Function to parse timestamp from log line
std::chrono::system_clock::time_point parseTimestamp(const std::string& line) {
    std::istringstream ss(line.substr(0, line.find(','))); // Extract timestamp part
    std::tm t = {};
    char dot;
    int milliseconds;

    // Parse format: HH:MM:SS.mmm
    ss >> std::get_time(&t, "%H:%M:%S") >> dot >> milliseconds;

    auto time = std::chrono::system_clock::from_time_t(std::mktime(&t));
    time += std::chrono::milliseconds(milliseconds);

    return time;
}

int main() {
    std::string directory = "logFolder";
    std::string excludeFile = "actions.txt";
    std::vector<LogEntry> entries;

    // Open the logFolder directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(directory.c_str())) != NULL) {
        // Read each file in the directory
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;

            // Skip "." and ".." and the excluded file
            if (filename == "." || filename == ".." || filename == excludeFile) {
                continue;
            }

            // Create the full file path
            std::string filePath = directory + "/" + filename;
            
            // Open and read the file
            std::ifstream file(filePath);
            if (file.is_open()) {
                std::cout << "Reading from file: " << filePath << std::endl;
                
                std::string line;
                while (std::getline(file, line)) {
                    if (!line.empty()) {
                        LogEntry entry;
                        entry.rawLine = line;
                        entry.timestamp = parseTimestamp(line);
                        entries.push_back(entry);
                    }
                }
                
                file.close();
            } else {
                std::cerr << "Failed to open file: " << filePath << std::endl;
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Unable to open directory: " << directory << std::endl;
        return 1;
    }

    // Sort entries based on parsed timestamp
    std::sort(entries.begin(), entries.end(), [](const LogEntry& a, const LogEntry& b) {
        return a.timestamp < b.timestamp;
    });

    // Write sorted entries to actions.txt
    std::string outputFile = directory + "/actions.txt";
    std::ofstream outFile(outputFile, std::ios::trunc);
    if (outFile.is_open()) {
        for (const auto& entry : entries) {
            outFile << entry.rawLine << "\n";
        }
        outFile.close();
        std::cout << "File sorted successfully into " << outputFile << std::endl;
    } else {
        std::cerr << "Failed to open file for writing: " << outputFile << std::endl;
        return 1;
    }

    return 0;
}
