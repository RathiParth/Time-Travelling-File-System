#ifndef FILE_HPP
#define FILE_HPP

#include "../DataStructures/TreeNode.hpp"
#include "../DataStructures/HashMap.hpp"
#include <string>
#include <ctime> // Required for time_t

class File {
private:
    std::string filename;
    TreeNode* root;
    TreeNode* curr_version;
    HashMap<int, TreeNode*>* version_map;
    int next_version_id;
    time_t last_change_t; // Using time_t

public:
    // All function signatures now use time_t
    File(const std::string& name, time_t creation_time);
    ~File();

    std::string getFilename() const;
    time_t LastChangeT() const;
    int TotalVersions() const;
    int ActiveVersionId() const;

    std::string READ() const;
    void INSERT(const std::string& content, time_t mod_time);
    void UPDATE(const std::string& content, time_t mod_time);
    void SNAPSHOT(const std::string& message, time_t snap_time);
    bool ROLLBACK(int versionID = -1);
    void HISTORY() const;
};

#endif // FILE_HPP
