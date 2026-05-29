// include/heapnode.h
// A single slot in the MinHeap

#ifndef HEAPNODE_H
#define HEAPNODE_H

class HeapNode {
public:
    int slot;  // memory slot number (0, 1, 2, ...)

    HeapNode(int s);
};

#endif