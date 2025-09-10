#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <vector>
#include <string>
#include <stdexcept>

// Functor for hashing keys. Can be specialized for different types.
template <typename K>
struct KeyHasher {
    // Default hash for integral types
    size_t operator()(const K& key, int capacity) const {
        return static_cast<size_t>(key) % capacity;
    }
};

// Specialization for std::string keys
template <>
struct KeyHasher<std::string> {
    size_t operator()(const std::string& key, int capacity) const {
        size_t hash = 0;
        for (char c : key) {
            // A simple but effective string hash algorithm
            hash = hash * 31 + c;
        }
        return hash % capacity;
    }
};


template <typename K, typename V>
class HashMap {
private:
    struct HashNode {
        K key;
        V value;
        HashNode* next;

        HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    std::vector<HashNode*> buckets;
    int capacity;
    int num_elements;
    KeyHasher<K> hasher;

    int hash(const K& key) const {
        return hasher(key, capacity);
    }

public:
    HashMap(int initial_capacity = 16) : capacity(initial_capacity), num_elements(0) {
        buckets.resize(capacity, nullptr);
    }

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

    void insert(const K& key, const V& value) {
        int bucket_index = hash(key);
        HashNode* head = buckets[bucket_index];
        
        // Check if key already exists and update it
        HashNode* current = head;
        while (current!= nullptr) {
            if (current->key == key) {
                current->value = value;
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

    void remove(const K& key) {
        int bucket_index = hash(key);
        HashNode* head = buckets[bucket_index];
        HashNode* prev = nullptr;
        HashNode* entry = head;

        while (entry!= nullptr && entry->key!= key) {
            prev = entry;
            entry = entry->next;
        }

        if (entry == nullptr) {
            return;
        }

        if (prev == nullptr) {
            buckets[bucket_index] = entry->next;
        } else {
            prev->next = entry->next;
        }
        delete entry;
        num_elements--;
    }

    // Returns a vector of all values in the HashMap for iteration purposes.
    std::vector<V> get_all_values() const {
        std::vector<V> values;
        for (int i = 0; i < capacity; ++i) {
            HashNode* entry = buckets[i];
            while (entry!= nullptr) {
                values.push_back(entry->value);
                entry = entry->next;
            }
        }
        return values;
    }
};

#endif // HASHMAP_HPP