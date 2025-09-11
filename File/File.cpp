#include "File.hpp"
#include <iostream>
#include <ctime>
#include <iomanip> // Required for time formatting
#include <sstream> // Required for string stream

File::File(const std::string& name, time_t t0)
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
time_t File::LastChangeT() const { return last_change_t; }
int File::TotalVersions() const { return next_version_id; }
int File::ActiveVersionId() const { return curr_version->version_id; }

std::string File::READ() const {
    return curr_version->content;
}

void File::INSERT(const std::string& content, time_t mod_time) {
    if (curr_version->snapshot_timestamp != 0) {
        TreeNode* new_version = new TreeNode(next_version_id++, curr_version->content + content, mod_time, curr_version);
        curr_version->children.push_back(new_version);
        std::cout << "New version " << new_version->version_id << " created for '" << filename << "'. Parent is version " << curr_version->version_id << "." << std::endl;
        curr_version = new_version;
        version_map->INSERT(new_version->version_id, new_version);
    } else {
        curr_version->content += content;
        std::cout << "Content inserted into active version " << curr_version->version_id << " of '" << filename << "'." << std::endl;
    }
    last_change_t = mod_time;
}

void File::UPDATE(const std::string& content, time_t mod_time) {
    if (curr_version->snapshot_timestamp != 0) {
        TreeNode* new_version = new TreeNode(next_version_id++, content, mod_time, curr_version);
        curr_version->children.push_back(new_version);
        std::cout << "New version " << new_version->version_id << " created for '" << filename << "'. Parent is version " << curr_version->version_id << "." << std::endl;
        curr_version = new_version;
        version_map->INSERT(new_version->version_id, new_version);
    } else {
        curr_version->content = content;
        std::cout << "Content updated for active version " << curr_version->version_id << " of '" << filename << "'." << std::endl;
    }
    last_change_t = mod_time;
}

void File::SNAPSHOT(const std::string& message, time_t snap_time) {
    if (curr_version->snapshot_timestamp == 0) {
        curr_version->message = message;
        curr_version->snapshot_timestamp = snap_time;
        std::cout << "Snapshot created for active version " << curr_version->version_id << " of '" << filename << "'." << std::endl;
    } else {
        std::cout << "Warning: Version " << curr_version->version_id << " is already a snapshot." << std::endl;
    }
}

bool File::ROLLBACK(int versionID) {
    if (versionID != -1) {
        TreeNode** target_node_ptr = version_map->get(versionID);
        if (target_node_ptr && (*target_node_ptr)->snapshot_timestamp != 0) {
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
            time_t snap_time = current->snapshot_timestamp;
            std::tm* ptm = std::localtime(&snap_time);
            std::stringstream ss;
            ss << std::put_time(ptm, "%a %b %d %H:%M:%S %Y");
            std::cout << "Version " << current->version_id << ": " << ss.str() << " - " << current->message << std::endl;
        }
        current = current->parent;
    }
}

