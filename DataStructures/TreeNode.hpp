#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <string>
#include <vector>
#include <ctime>

// Represents a single version of a file in the version history tree. [1]
class TreeNode {
public:
    int version_id;
    std::string content;
    std::string message;
    time_t created_timestamp;
    time_t snapshot_timestamp;

    TreeNode* parent;
    std::vector<TreeNode*> children;

    // Constructor to initialize a new version node.
    TreeNode(int id, std::string initial_content, TreeNode* p = nullptr)
        : version_id(id), content(initial_content), parent(p),
          created_timestamp(std::time(nullptr)), snapshot_timestamp(0) {}

    // Destructor to recursively delete all child nodes to prevent memory leaks. [2]
    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child;
        }
    }
};

#endif // TREENODE_HPP