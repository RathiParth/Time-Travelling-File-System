#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>

template <typename T, typename Comparator>
class MaxHeap {
private:
    std::vector<T> heap;
    Comparator compare;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void heapifyDown(int i) {
        int left = leftChild(i);
        int right = rightChild(i);
        int largest = i;
        int heap_size = size();

        if (left < heap_size && compare(heap[largest], heap[left])) {
            largest = left;
        }
        if (right < heap_size && compare(heap[largest], heap[right])) {
            largest = right;
        }
        if (largest!= i) {
            std::swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }

    void heapifyUp(int i) {
        while (i > 0 && compare(heap[parent(i)], heap[i])) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

public:
    MaxHeap(Comparator comp = Comparator()) : compare(comp) {}

    void INSERT(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    T extractMax() {
        if (isEmpty()) {
            std::cerr << "Error: Attempted to extract from an empty heap. Terminating." << std::endl;
            exit(1);
        }
        // 1. Store the root element (the max value).
        T max_val = heap[0];
        
        // 2. Replace the root element with the last element.
        heap[0] = heap.back();

        // 3. Remove the last element.
        heap.pop_back();
        
        // 4. Call heapifyDown on the new root to restore the heap property.
        if (!isEmpty()) {
            heapifyDown(0);
        }
        
        // 5. Return the stored max value.
        return max_val;
    }
    
    void clear() {
        heap.clear();
    }

    bool isEmpty() const { return heap.empty(); }
    int size() const { return heap.size(); }
};

#endif // MAXHEAP_HPP