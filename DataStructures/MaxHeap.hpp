#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include <vector>
#include <stdexcept>
#include <algorithm> // For std::swap

// A templated Max-Heap implementation that accepts a custom comparator.
template <typename T, typename Comparator>
class MaxHeap {
private:
    std::vector<T> heap;
    Comparator compare;

    // Helper functions to get parent and child indices.
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    // "Bubbles down" an element to maintain the max-heap property.
    void heapifyDown(int i) {
        int left = leftChild(i);
        int right = rightChild(i);
        int largest = i;

        if (left < size() && compare(heap[largest], heap[left])) {
            largest = left;
        }
        if (right < size() && compare(heap[largest], heap[right])) {
            largest = right;
        }

        if (largest!= i) {
            std::swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }

    // "Bubbles up" an element to maintain the max-heap property.
    void heapifyUp(int i) {
        while (i > 0 && compare(heap[parent(i)], heap[i])) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

public:
    MaxHeap(Comparator comp = Comparator()) : compare(comp) {}

    // Inserts a new element into the heap. [4]
    void insert(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    // Removes and returns the maximum element from the heap. [4]
    T extractMax() {
        if (isEmpty()) {
            throw std::out_of_range("Heap is empty");
        }
        T max_val = heap;
        heap = heap.back();
        heap.pop_back();
        if (!isEmpty()) {
            heapifyDown(0);
        }
        return max_val;
    }
    
    void clear() {
        heap.clear();
    }

    bool isEmpty() const { return heap.empty(); }
    int size() const { return heap.size(); }
};

#endif // MAXHEAP_HPP