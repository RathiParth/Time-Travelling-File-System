#ifndef FILE_HPP
#define FILE_HPP

#include "../DataStructures/TreeNode.hpp"
#include "../DataStructures/HashMap.hpp"
#include <string>
#include <ctime>

class File {
private:
    std::string filename;
    TreeNode* root;
    TreeNode* active_version;
    HashMap<int, TreeNode*>* version_map;
    int next_version_id;
    time_t last_modification_time;

public:
    File(const std::string& name);
    ~File();

    std::string getFilename() const;
    time_t getLastModTime() const;
    int getTotalVersions() const;

    std::string read() const;
    void insert(const std::string& content);
    void update(const std::string& content);
    void snapshot(const std::string& message);
    bool rollback(int versionID = -1);
    void history() const;
};

#endif // FILE_HPP