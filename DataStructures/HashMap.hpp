#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <vector>
#include <string>

template <typename K>
struct KeyHasher {
    size_t operator()(const K& key, int capacity) const {
        return static_cast<size_t>(key) % capacity;
    }
};

template <>
struct KeyHasher<std::string> {
    size_t operator()(const std::string& key, int capacity) const {
        size_t hash = 0;
        for (char c : key) {
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
        HashNode* current = head;
        while (current!= nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
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