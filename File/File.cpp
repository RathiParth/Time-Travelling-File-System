#include "File.hpp"
#include <iostream>

File::File(const std::string& name, unsigned long long t0) 
    : filename(name), next_version_id(1) {
    root = new TreeNode(0, "", t0, nullptr);
    root->message = "Initial_empty_snapshot";
    root->snapshot_timestamp = t0;
    curr_version = root;
    last_change_t = t0;
    version_map = new HashMap<int, TreeNode*>();
    version_map->INSERT(0, root);
}

File::~File() {
    delete root;
    delete version_map;
}

std::string File::getFilename() const { return filename; }
unsigned long long File::LastChangeT() const { return last_change_t; }
int File::getTotalVersions() const { return next_version_id; }
int File::getActiveVersionId() const { return curr_version->version_id; }

std::string File::READ() const {
    return curr_version->content;
}

void File::INSERT(const std::string& content, unsigned long long mod_time) {
    if (curr_version->snapshot_timestamp != 0) {
        bool is_branch = !curr_version->children.empty();
        TreeNode* new_version = new TreeNode(next_version_id++, curr_version->content + content, mod_time, curr_version);
        
        std::cout << "New version " << new_version->version_id << " created for '" << filename << "'.";
        if (is_branch) {
            std::cout << " Parent is version " << curr_version->version_id << ".";
        }
        std::cout << std::endl;
        
        curr_version->children.push_back(new_version);
        curr_version = new_version;
        version_map->INSERT(new_version->version_id, new_version);
    } else {
        curr_version->content += content;
    }
    last_change_t = mod_time;
}

void File::UPDATE(const std::string& content, unsigned long long mod_time) {
    if (curr_version->snapshot_timestamp != 0) {
        bool is_branch = !curr_version->children.empty();
        TreeNode* new_version = new TreeNode(next_version_id++, content, mod_time, curr_version);
        
        std::cout << "New version " << new_version->version_id << " created for '" << filename << "'.";
        if (is_branch) {
            std::cout << " Parent is version " << curr_version->version_id << ".";
        }
        std::cout << std::endl;

        curr_version->children.push_back(new_version);
        curr_version = new_version;
        version_map->INSERT(new_version->version_id, new_version);
    } else {
        curr_version->content = content;
    }
    last_change_t = mod_time;
}

void File::SNAPSHOT(const std::string& message, unsigned long long snap_time) {
    if (curr_version->snapshot_timestamp == 0) {
        curr_version->message = message;
        curr_version->snapshot_timestamp = snap_time;
        std::cout << "Snapshot created for active version " << curr_version->version_id << " of '" << filename << "'." << std::endl;
    }
}

bool File::ROLLBACK(int versionID) {
    if (versionID != -1) {
        TreeNode** target_node_ptr = version_map->get(versionID);
        if (target_node_ptr) {
            curr_version = *target_node_ptr;
            return true;
        }
        return false;
    } else {
        if (curr_version->parent) {
            curr_version = curr_version->parent;
            return true;
        }
        return false;
    }
}

void File::HISTORY() const {
    std::cout << "History for " << filename << ":" << std::endl;
    TreeNode* current = curr_version;
    while (current != nullptr) {
        if (current->snapshot_timestamp != 0) {
            // Using placeholder for timestamp as in the requested output
            std::cout << "Version " << current->version_id << ": <timestamp> - " << current->message << std::endl;
        }
        current = current->parent;
    }
}

