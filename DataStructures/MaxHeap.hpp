#ifndef MAXHEAP_HPP
#define MAXHEAP_HPP

#include <vector>
#include <algorithm>
#include <iostream>

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
        if (largest != i) {
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
            std::cerr << "Error: Attempted to extract from an empty heap. Returning default-constructed value." << std::endl;
            return T();
        }
        T max_val = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!isEmpty()) {
            heapifyDown(0);
        }
        return max_val;
    }

    T peekMax() const {
        if (isEmpty()) {
            std::cerr << "Error: Attempted to peek into an empty heap. Returning default-constructed value." << std::endl;
            return T();
        }
        return heap[0];
    }

    // Removed the duplicate isEmpty() function.
    bool isEmpty() const {
        return heap.empty();
    }

    int size() const {
        return heap.size();
    }

    void clear() {
        heap.clear();
    }
};

#endif // MAXHEAP_HPP
