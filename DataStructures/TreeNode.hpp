#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <string>
#include <vector>

class TreeNode {
public:
    int version_id;
    std::string content;
    std::string message;
    unsigned long long created_timestamp;
    unsigned long long snapshot_timestamp;
    TreeNode* parent;
    std::vector<TreeNode*> children;

    TreeNode(int id, std::string initial_content, unsigned long long creation_time, TreeNode* p = nullptr)
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