#include "FileSystem.hpp"
#include <iostream>
#include <iomanip>

FileSystem::FileSystem() {
    recent_files_heap = new MaxHeap<File*, CompareByModTime>();
    biggest_trees_heap = new MaxHeap<File*, CompareByVersionCount>();
}

FileSystem::~FileSystem() {
    for (auto const& [key, val] : files) {
        delete val;
    }
    delete recent_files_heap;
    delete biggest_trees_heap;
}

void FileSystem::rebuildHeaps() {
    recent_files_heap->clear();
    biggest_trees_heap->clear();
    for (auto const& [filename, file_ptr] : files) {
        recent_files_heap->insert(file_ptr);
        biggest_trees_heap->insert(file_ptr);
    }
}

void FileSystem::create(const std::string& filename) {
    if (files.count(filename)) {
        std::cerr << "Error: File '" << filename << "' already exists." << std::endl;
        return;
    }
    File* new_file = new File(filename);
    files[filename] = new_file;
    rebuildHeaps();
    std::cout << "Created file '" << filename << "'." << std::endl;
}

void FileSystem::read(const std::string& filename) {
    if (!files.count(filename)) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    std::cout << files[filename]->read() << std::endl;
}

void FileSystem::insert(const std::string& filename, const std::string& content) {
    if (!files.count(filename)) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    files[filename]->insert(content);
    rebuildHeaps();
}

void FileSystem::update(const std::string& filename, const std::string& content) {
    if (!files.count(filename)) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    files[filename]->update(content);
    rebuildHeaps();
}

void FileSystem::snapshot(const std::string& filename, const std::string& message) {
    if (!files.count(filename)) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    files[filename]->snapshot(message);
}

void FileSystem::rollback(const std::string& filename, int versionID) {
    if (!files.count(filename)) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    if (!files[filename]->rollback(versionID)) {
        if (versionID == -1) {
            std::cerr << "Error: Cannot rollback from root version." << std::endl;
        } else {
            std::cerr << "Error: Version ID " << versionID << " not found for file '" << filename << "'." << std::endl;
        }
    }
}

void FileSystem::history(const std::string& filename) {
    if (!files.count(filename)) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    files[filename]->history();
}

void FileSystem::recentFiles(int num) {
    MaxHeap<File*, CompareByModTime> temp_heap = *recent_files_heap;
    std::cout << "Most Recently Modified Files:" << std::endl;
    int count = 0;
while (!temp_heap.isEmpty() && (num == -1 || count < num)) {
        File* file = temp_heap.extractMax();
        char time_buf[20];
        time_t mod_time = file->getLastModTime();
        std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&mod_time));
        std::cout << "  - " << file->getFilename() << " (Last modified: " << time_buf << ")" << std::endl;
        count++;
    }
}

void FileSystem::biggestTrees(int num) {
    MaxHeap<File*, CompareByVersionCount> temp_heap = *biggest_trees_heap;
    std::cout << "Files with Most Versions:" << std::endl;
    int count = 0;
    while (!temp_heap.isEmpty() && (num == -1 || count < num)) {
        File* file = temp_heap.extractMax();
        std::cout << "  - " << file->getFilename() << " (" << file->getTotalVersions() << " versions)" << std::endl;
        count++;
    }
}