// src/minheap.cpp

#include "minheap.h"
#include <iostream>
#include <stdexcept>

// ─── CONSTRUCTOR / DESTRUCTOR ─────────────────────────────────────────────────

MinHeap::MinHeap(int cap) {
    capacity = cap;
    size     = 0;
    heap     = new HeapNode*[capacity];

    // Pre-fill heap with all slots 0 to cap-1
    // This is the initial state: all memory is free
    for (int i = 0; i < capacity; i++) {
        heap[i] = new HeapNode(i);
        size++;
    }
    // The array [0,1,2,...,n-1] is already a valid min-heap
    // because it's sorted — no heapify needed
}

MinHeap::~MinHeap() {
    for (int i = 0; i < size; i++) delete heap[i];
    delete[] heap;
}

// ─── HELPERS ─────────────────────────────────────────────────────────────────

void MinHeap::swap(int i, int j) {
    HeapNode* tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

// Bubble UP: after inserting at end, swap with parent while smaller
void MinHeap::heapify_up(int i) {
    while (i > 0 && heap[i]->slot < heap[parent(i)]->slot) {
        swap(i, parent(i));
        i = parent(i);
    }
}

// Bubble DOWN: after removing root, swap with smaller child while larger
void MinHeap::heapify_down(int i) {
    int smallest = i;
    int l = left(i);
    int r = right(i);

    if (l < size && heap[l]->slot < heap[smallest]->slot) smallest = l;
    if (r < size && heap[r]->slot < heap[smallest]->slot) smallest = r;

    if (smallest != i) {
        swap(i, smallest);
        heapify_down(smallest);
    }
}

// ─── OPERATIONS ──────────────────────────────────────────────────────────────

int MinHeap::peek() const {
    if (size == 0) throw std::runtime_error("Heap is empty");
    return heap[0]->slot;
}

int MinHeap::pop() {
    if (size == 0) throw std::runtime_error("Out of memory slots");

    int min_slot = heap[0]->slot;

    // Move last element to root, shrink size
    delete heap[0];
    heap[0] = heap[size - 1];
    size--;

    // Restore heap property by bubbling down
    if (size > 0) heapify_down(0);

    return min_slot;
}

void MinHeap::push(int slot) {
    if (size >= capacity) throw std::runtime_error("Heap overflow");

    // Add at end, bubble up
    heap[size] = new HeapNode(slot);
    heapify_up(size);
    size++;
}

int  MinHeap::get_size() const { return size;      }
bool MinHeap::is_empty() const { return size == 0; }

void MinHeap::print() const {
    std::cout << "Heap [";
    for (int i = 0; i < size; i++) {
        std::cout << heap[i]->slot;
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}