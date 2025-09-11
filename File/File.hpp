#ifndef FILE_HPP
#define FILE_HPP

#include "../DataStructures/TreeNode.hpp"
#include "../DataStructures/HashMap.hpp"
#include <string>
#include <ctime> // Required for time_t

using namespace std;

class File {
private:
    string filename;
    TreeNode* root;
    TreeNode* curr_version;
    HashMap<int, TreeNode*>* version_map;
    int next_version_id;
    time_t last_change_t; // Using time_t

public:
    // All function signatures now use time_t
    File(const string& name, time_t creation_time);
    ~File();

    string getFilename() const;
    time_t LastChangeT() const;
    int TotalVersions() const;
    int ActiveVersionId() const;

    string READ() const;
    void INSERT(const string& content, time_t mod_time);
    void UPDATE(const string& content, time_t mod_time);
    void SNAPSHOT(const string& message, time_t snap_time);
    bool ROLLBACK(int versionID = -1);
    void HISTORY() const;
};

#endif // FILE_HPP
