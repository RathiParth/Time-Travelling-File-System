#ifndef FILE_HPP
#define FILE_HPP

#include "../DataStructures/TreeNode.hpp"
#include "../DataStructures/HashMap.hpp"
#include <string>

class File {
private:
    std::string filename;
    TreeNode* root;
    TreeNode* curr_version;
    HashMap<int, TreeNode*>* version_map;
    int next_version_id;
    unsigned long long last_change_t;

public:
    File(const std::string& name, unsigned long long creation_time);
    ~File();

    std::string getFilename() const;
    unsigned long long LastChangeT() const;
    int getTotalVersions() const;
    int getActiveVersionId() const;

    std::string READ() const;
    void INSERT(const std::string& content, unsigned long long mod_time);
    void UPDATE(const std::string& content, unsigned long long mod_time);
    void SNAPSHOT(const std::string& message, unsigned long long snap_time);
    bool ROLLBACK(int versionID = -1);
    void HISTORY() const;
};

#endif // FILE_HPP

