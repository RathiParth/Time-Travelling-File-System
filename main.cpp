#include "File/FileSystem.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// Function to parse the remainder of a stringstream after the command and filename.
std::string get_remaining_content(std::stringstream& ss) {
    std::string content;
    std::string temp;
    if (ss >> temp) {
        content += temp;
        while (ss >> temp) {
            content += " " + temp;
        }
    }
    return content;
}

int main() {
    FileSystem fs;
    std::string line;

    while (std::getline(std::cin, line)) {
        std::stringstream ss(line); //
        std::string command;
        ss >> command;

        if (command == "CREATE") {
            std::string filename;
            if (ss >> filename) fs.create(filename);
            else std::cerr << "Usage: CREATE <filename>" << std::endl;
        } else if (command == "READ") {
            std::string filename;
            if (ss >> filename) fs.read(filename);
            else std::cerr << "Usage: READ <filename>" << std::endl;
        } else if (command == "INSERT") {
            std::string filename;
            if (ss >> filename) {
                std::string content = get_remaining_content(ss);
                fs.insert(filename, content);
            } else {
                std::cerr << "Usage: INSERT <filename> <content>" << std::endl;
            }
        } else if (command == "UPDATE") {
            std::string filename;
            if (ss >> filename) {
                std::string content = get_remaining_content(ss);
                fs.update(filename, content);
            } else {
                std::cerr << "Usage: UPDATE <filename> <content>" << std::endl;
            }
        } else if (command == "SNAPSHOT") {
            std::string filename;
            if (ss >> filename) {
                std::string message = get_remaining_content(ss);
                fs.snapshot(filename, message);
            } else {
                std::cerr << "Usage: SNAPSHOT <filename> <message>" << std::endl;
            }
        } else if (command == "ROLLBACK") {
            std::string filename;
            if (ss >> filename) {
                int versionID;
                if (ss >> versionID) fs.rollback(filename, versionID);
                else fs.rollback(filename);
            } else {
                std::cerr << "Usage: ROLLBACK <filename>" << std::endl;
            }
        } else if (command == "HISTORY") {
            std::string filename;
            if (ss >> filename) fs.history(filename);
            else std::cerr << "Usage: HISTORY <filename>" << std::endl;
        } else if (command == "RECENT_FILES") {
            int num;
            if (ss >> num) fs.recentFiles(num);
            else fs.recentFiles(-1); // -1 indicates no limit
        } else if (command == "BIGGEST_TREES") {
            int num;
            if (ss >> num) fs.biggestTrees(num);
            else fs.biggestTrees(-1); // -1 indicates no limit
        } else if (!command.empty()) {
            std::cerr << "Error: Unknown command '" << command << "'." << std::endl;
        }
    }

    return 0;
}