#include "FileSystem.hpp"
#include <iostream>
#include <vector>

FileSystem::FileSystem() : system_clock(0) {
    files = new HashMap<std::string, File*>();
    recent_files_heap = new MaxHeap<File*, CompareByModTime>();
    biggest_trees_heap = new MaxHeap<File*, CompareByVersionCount>();
}

FileSystem::~FileSystem() {
    std::vector<File*> all_files = files->get_all_values();
    for (File* file : all_files) {
        delete file;
    }
    delete files;
    delete recent_files_heap;
    delete biggest_trees_heap;
}

void FileSystem::rebuildHeaps() {
    recent_files_heap->clear();
    biggest_trees_heap->clear();
    std::vector<File*> all_files = files->get_all_values();
    for (File* file_ptr : all_files) {
        recent_files_heap->INSERT(file_ptr);
        biggest_trees_heap->INSERT(file_ptr);
    }
}

void FileSystem::CREATE(const std::string& filename) {
    if (files->get(filename)!= nullptr) {
        std::cerr << "Error: File '" << filename << "' already exists." << std::endl;
        return;
    }
    system_clock++;
    File* new_file = new File(filename, system_clock);
    files->INSERT(filename, new_file);
    rebuildHeaps();
    std::cout << "Created file '" << filename << "'." << std::endl;
}

void FileSystem::READ(const std::string& filename) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    std::cout << (*file_ptr)->READ() << std::endl;
}

void FileSystem::INSERT(const std::string& filename, const std::string& content) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    system_clock++;
    (*file_ptr)->INSERT(content, system_clock);
    rebuildHeaps();
}

void FileSystem::UPDATE(const std::string& filename, const std::string& content) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    system_clock++;
    (*file_ptr)->UPDATE(content, system_clock);
    rebuildHeaps();
}

void FileSystem::SNAPSHOT(const std::string& filename, const std::string& message) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    system_clock++;
    (*file_ptr)->SNAPSHOT(message, system_clock);
}

void FileSystem::ROLLBACK(const std::string& filename, int versionID) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    if (!(*file_ptr)->ROLLBACK(versionID)) {
        if (versionID == -1) {
            std::cerr << "Error: Cannot ROLLBACK from root version." << std::endl;
        } else {
            std::cerr << "Error: Version ID " << versionID << " not found for file '" << filename << "'." << std::endl;
        }
    }
}

void FileSystem::HISTORY(const std::string& filename) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    (*file_ptr)->HISTORY();
}

void FileSystem::RECENT_FILES(int num) {
    MaxHeap<File*, CompareByModTime> temp_heap = *recent_files_heap;
    std::cout << "Most Recently Modified Files:" << std::endl;
    int count = 0;
    while (!temp_heap.isEmpty() && (num == -1 || count < num)) {
        File* file = temp_heap.extractMax();
        std::cout << "  - " << file->getFilename() 
                  << " (Last modified at time: " << file->getLastModTime() << ")" << std::endl;
        count++;
    }
}

void FileSystem::BIGGEST_TREES(int num) {
    MaxHeap<File*, CompareByVersionCount> temp_heap = *biggest_trees_heap;
    std::cout << "Files with Most Versions:" << std::endl;
    int count = 0;
    while (!temp_heap.isEmpty() && (num == -1 || count < num)) {
        File* file = temp_heap.extractMax();
        std::cout << "  - " << file->getFilename() << " (" << file->getTotalVersions() << " versions)" << std::endl;
        count++;
    }
}