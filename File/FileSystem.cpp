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
        recent_files_heap->insert(file_ptr);
        biggest_trees_heap->insert(file_ptr);
    }
}

void FileSystem::create(const std::string& filename) {
    if (files->get(filename)!= nullptr) {
        std::cerr << "Error: File '" << filename << "' already exists." << std::endl;
        return;
    }
    system_clock++;
    File* new_file = new File(filename, system_clock);
    files->insert(filename, new_file);
    rebuildHeaps();
    std::cout << "Created file '" << filename << "'." << std::endl;
}

void FileSystem::read(const std::string& filename) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    std::cout << (*file_ptr)->read() << std::endl;
}

void FileSystem::insert(const std::string& filename, const std::string& content) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    system_clock++;
    (*file_ptr)->insert(content, system_clock);
    rebuildHeaps();
}

void FileSystem::update(const std::string& filename, const std::string& content) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    system_clock++;
    (*file_ptr)->update(content, system_clock);
    rebuildHeaps();
}

void FileSystem::snapshot(const std::string& filename, const std::string& message) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    system_clock++;
    (*file_ptr)->snapshot(message, system_clock);
}

void FileSystem::rollback(const std::string& filename, int versionID) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    if (!(*file_ptr)->rollback(versionID)) {
        if (versionID == -1) {
            std::cerr << "Error: Cannot rollback from root version." << std::endl;
        } else {
            std::cerr << "Error: Version ID " << versionID << " not found for file '" << filename << "'." << std::endl;
        }
    }
}

void FileSystem::history(const std::string& filename) {
    File** file_ptr = files->get(filename);
    if (file_ptr == nullptr) {
        std::cerr << "Error: File '" << filename << "' not found." << std::endl;
        return;
    }
    (*file_ptr)->history();
}

void FileSystem::recentFiles(int num) {
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

void FileSystem::biggestTrees(int num) {
    MaxHeap<File*, CompareByVersionCount> temp_heap = *biggest_trees_heap;
    std::cout << "Files with Most Versions:" << std::endl;
int count = 0;
while (!temp_heap.isEmpty() && (num == -1 || count < num)) {
    File* file = temp_heap.extractMax();
    std::cout << "  - " << file->getFilename() << " (" << file->getTotalVersions() << " versions)" << std::endl;
    count++;
}
