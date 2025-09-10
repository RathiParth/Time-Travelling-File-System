#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <vector>
#include <stdexcept>

// A templated HashMap implementation using separate chaining for collision resolution.
template <typename K, typename V>
class HashMap {
private:
    // Node structure for the linked lists in each bucket.
    struct HashNode {
        K key;
        V value;
        HashNode* next;

        HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    std::vector<HashNode*> buckets;
    int capacity;
    int num_elements;

    // Hash function to map keys to bucket indices.
    int hash(const K& key) const {
        return static_cast<int>(key) % capacity;
    }

public:
    // Constructor initializes the bucket array.
    HashMap(int initial_capacity = 16) : capacity(initial_capacity), num_elements(0) {
        buckets.resize(capacity, nullptr);
    }

    // Destructor to clean up all dynamically allocated nodes.
    ~HashMap() {
        for (int i = 0; i < capacity; ++i) {
            HashNode* entry = buckets[i];
            while (entry!= nullptr) {
                HashNode* prev = entry;
                entry = entry->next;
                delete prev;
            }
        }
    }

    // Inserts a key-value pair. If the key exists, it updates the value. [3]
    void insert(const K& key, const V& value) {
        int bucket_index = hash(key);
        HashNode* head = buckets[bucket_index];
        
        // Check if key already exists
        HashNode* current = head;
        while (current!= nullptr) {
            if (current->key == key) {
                current->value = value; // Update existing key
                return;
            }
            current = current->next;
        }

        // Key not found, insert new node at the beginning of the list
        HashNode* newNode = new HashNode(key, value);
        newNode->next = head;
        buckets[bucket_index] = newNode;
        num_elements++;
    }

    // Retrieves the value associated with a key. Returns a pointer to the value or nullptr if not found. [3]
    V* get(const K& key) const {
        int bucket_index = hash(key);
        HashNode* entry = buckets[bucket_index];

        while (entry!= nullptr) {
            if (entry->key == key) {
                return &(entry->value);
            }
            entry = entry->next;
        }
        return nullptr;
    }

    // Removes a key-value pair.
    void remove(const K& key) {
        int bucket_index = hash(key);
        HashNode* head = buckets[bucket_index];
        HashNode* prev = nullptr;
        HashNode* entry = head;

        while (entry!= nullptr && entry->key!= key) {
            prev = entry;
            entry = entry->next;
        }

        if (entry == nullptr) { // Key not found
            return;
        }

        if (prev == nullptr) { // Node to remove is the head
            buckets[bucket_index] = entry->next;
        } else {
            prev->next = entry->next;
        }
        delete entry;
        num_elements--;
    }
};

#endif // HASHMAP_HPP