#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "File.hpp"
#include "../DataStructures/MaxHeap.hpp"
#include "../DataStructures/HashMap.hpp"
#include <string>

class File;

struct ChangeT {
    bool operator()(const File* a, const File* b) const {
        return a->LastChangeT() < b->LastChangeT();
    }
};

struct VersionCount {
    bool operator()(const File* a, const File* b) const {
        return a->getTotalVersions() < b->getTotalVersions();
    }
};

class FileSystem {
private:
    HashMap<std::string, File*>* files;
    MaxHeap<File*, ChangeT>* recentFiles;
    MaxHeap<File*, VersionCount>* biggestTree;
    unsigned long long system_clock;

    void rebuildHeaps();

public:
    FileSystem();
    ~FileSystem();

    void CREATE(const std::string& filename);
    void READ(const std::string& filename);
    void INSERT(const std::string& filename, const std::string& content);
    void UPDATE(const std::string& filename, const std::string& content);
    void SNAPSHOT(const std::string& filename, const std::string& message);
    void ROLLBACK(const std::string& filename, int versionID = -1);
    void HISTORY(const std::string& filename);
    void RECENT_FILES(int num);
    void BIGGEST_TREES(int num);
};

#endif // FILESYSTEM_HPP