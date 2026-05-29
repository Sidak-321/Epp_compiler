// include/minheap.h
// Min-Heap memory allocator
// Always gives the smallest available memory slot

#ifndef MINHEAP_H
#define MINHEAP_H

#include "heapnode.h"

class MinHeap {
private:
    HeapNode** heap;   // array of HeapNode pointers
    int        capacity;
    int        size;

    // Helpers for navigating the heap array
    int parent(int i) { return (i - 1) / 2; }
    int left  (int i) { return 2 * i + 1;   }
    int right (int i) { return 2 * i + 2;   }

    void swap(int i, int j);

    // After push: bubble new element UP until heap property restored
    void heapify_up(int i);

    // After pop: bubble root DOWN until heap property restored
    void heapify_down(int i);

public:
    MinHeap(int capacity);
    ~MinHeap();

    // Get smallest slot (doesn't remove)
    int  peek()      const;

    // Remove and return smallest slot
    int  pop();

    // Return a slot back to the heap
    void push(int slot);

    // How many slots are available
    int  get_size()  const;

    // Is the heap empty? (all slots used)
    bool is_empty()  const;

    // Print current state
    void print()     const;
};

#endif