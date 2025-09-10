#include "File.hpp"
#include <iostream>

File::File(const std::string& name, unsigned long long creation_time) 
    : filename(name), next_version_id(1) {
    root = new TreeNode(0, "", creation_time, nullptr);
    root->message = "Initial version";
    root->snapshot_timestamp = creation_time;
    active_version = root;
    last_modification_time = creation_time;
    version_map = new HashMap<int, TreeNode*>();
    version_map->insert(0, root);
}

File::~File() {
    delete root;
    delete version_map;
}

std::string File::getFilename() const { return filename; }
unsigned long long File::getLastModTime() const { return last_modification_time; }
int File::getTotalVersions() const { return next_version_id; }

std::string File::read() const {
    return active_version->content;
}

void File::insert(const std::string& content, unsigned long long mod_time) {
    if (active_version->snapshot_timestamp!= 0) {
        TreeNode* new_version = new TreeNode(next_version_id++, active_version->content + content, mod_time, active_version);
        active_version->children.push_back(new_version);
        active_version = new_version;
        version_map->insert(new_version->version_id, new_version);
    } else {
        active_version->content += content;
    }
    last_modification_time = mod_time;
}

void File::update(const std::string& content, unsigned long long mod_time) {
    if (active_version->snapshot_timestamp!= 0) {
        TreeNode* new_version = new TreeNode(next_version_id++, content, mod_time, active_version);
        active_version->children.push_back(new_version);
        active_version = new_version;
        version_map->insert(new_version->version_id, new_version);
    } else {
        active_version->content = content;
    }
    last_modification_time = mod_time;
}

void File::snapshot(const std::string& message, unsigned long long snap_time) {
    if (active_version->snapshot_timestamp == 0) {
        active_version->message = message;
        active_version->snapshot_timestamp = snap_time;
    }
}

bool File::rollback(int versionID) {
    if (versionID!= -1) {
        TreeNode** target_node_ptr = version_map->get(versionID);
        if (target_node_ptr) {
            active_version = *target_node_ptr;
            return true;
        }
        return false;
    } else {
        if (active_version->parent) {
            active_version = active_version->parent;
            return true;
        }
        return false;
    }
}

void File::history() const {
    std::cout << "History for file: " << filename << std::endl;
    TreeNode* current = active_version;
    while (current!= nullptr) {
        if (current->snapshot_timestamp!= 0) {
            std::cout << "  - Version " << current->version_id 
                      << ": " << current->message << std::endl;
        }
        current = current->parent;
    }
}