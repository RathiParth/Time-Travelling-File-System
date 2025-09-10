#include "File.hpp"
#include <iostream>
#include <iomanip>

File::File(const std::string& name) : filename(name), next_version_id(1) {
    root = new TreeNode(0, "", nullptr);
    root->message = "Initial version";
    root->snapshot_timestamp = std::time(nullptr);
    active_version = root;
    last_modification_time = root->created_timestamp;

    version_map = new HashMap<int, TreeNode*>();
    version_map->insert(0, root);
}

File::~File() {
    delete root; // Triggers recursive deletion of all nodes
    delete version_map;
}

std::string File::getFilename() const { return filename; }
time_t File::getLastModTime() const { return last_modification_time; }
int File::getTotalVersions() const { return next_version_id; }

std::string File::read() const {
    return active_version->content;
}

void File::insert(const std::string& content) {
    if (active_version->snapshot_timestamp!= 0) { // Is a snapshot, create new version
        TreeNode* new_version = new TreeNode(next_version_id++, active_version->content + content, active_version);
        active_version->children.push_back(new_version);
        active_version = new_version;
        version_map->insert(new_version->version_id, new_version);
    } else { // Not a snapshot, modify in place
        active_version->content += content;
    }
    last_modification_time = std::time(nullptr);
}

void File::update(const std::string& content) {
    if (active_version->snapshot_timestamp!= 0) { // Is a snapshot, create new version
        TreeNode* new_version = new TreeNode(next_version_id++, content, active_version);
        active_version->children.push_back(new_version);
        active_version = new_version;
        version_map->insert(new_version->version_id, new_version);
    } else { // Not a snapshot, modify in place
        active_version->content = content;
    }
    last_modification_time = std::time(nullptr);
}

void File::snapshot(const std::string& message) {
    if (active_version->snapshot_timestamp == 0) {
        active_version->message = message;
        active_version->snapshot_timestamp = std::time(nullptr);
    }
}

bool File::rollback(int versionID) {
    if (versionID!= -1) { // Rollback to a specific version
        TreeNode** target_node_ptr = version_map->get(versionID);
        if (target_node_ptr) {
            active_version = *target_node_ptr;
            return true;
        }
        return false; // Version not found
    } else { // Rollback to parent
        if (active_version->parent) {
            active_version = active_version->parent;
            return true;
        }
        return false; // Already at root
    }
}

void File::history() const {
    std::cout << "History for file: " << filename << std::endl;
    TreeNode* current = active_version;
    while (current!= nullptr) {
        if (current->snapshot_timestamp!= 0) {
            char time_buf;
            std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&current->snapshot_timestamp));
            std::cout << "  - Version " << current->version_id << " [" << time_buf << "]: " << current->message << std::endl;
        }
        current = current->parent;
    }
}