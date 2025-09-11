#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <string>
#include <vector>
#include <ctime> // Required for time_t

class TreeNode {
public:
    int version_id;
    std::string content;
    std::string message;
    time_t created_timestamp; // Changed to time_t
    time_t snapshot_timestamp; // Changed to time_t
    TreeNode* parent;
    std::vector<TreeNode*> children;

    // Updated constructor to use time_t
    TreeNode(int id, std::string initial_content, time_t creation_time, TreeNode* p = nullptr)
        : version_id(id), content(initial_content), message(""),
          created_timestamp(creation_time), snapshot_timestamp(0),
          parent(p) {}

    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child;
        }
    }
};

#endif // TREENODE_HPP
