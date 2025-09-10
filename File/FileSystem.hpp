#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "File.hpp"
#include "../DataStructures/MaxHeap.hpp"
#include <map>
#include <string>

// Comparator for ordering File pointers by last modification time for RECENT_FILES.
struct CompareByModTime {
    bool operator()(const File* a, const File* b) const {
        return a->getLastModTime() < b->getLastModTime();
    }
};

// Comparator for ordering File pointers by version count for BIGGEST_TREES.
struct CompareByVersionCount {
    bool operator()(const File* a, const File* b) const {
        return a->getTotalVersions() < b->getTotalVersions();
    }
};

class FileSystem {
private:
    std::map<std::string, File*> files;
    MaxHeap<File*, CompareByModTime>* recent_files_heap;
    MaxHeap<File*, CompareByVersionCount>* biggest_trees_heap;

    void rebuildHeaps();

public:
    FileSystem();
    ~FileSystem();

    void create(const std::string& filename);
    void read(const std::string& filename);
    void insert(const std::string& filename, const std::string& content);
    void update(const std::string& filename, const std::string& content);
    void snapshot(const std::string& filename, const std::string& message);
    void rollback(const std::string& filename, int versionID = -1);
    void history(const std::string& filename);
    void recentFiles(int num);
    void biggestTrees(int num);
};

#endif // FILESYSTEM_HPP