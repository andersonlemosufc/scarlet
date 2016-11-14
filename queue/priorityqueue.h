#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "./comparable/comparable.h"

#include <QDebug>

class PriorityQueue
{
public:
    PriorityQueue();
    PriorityQueue(int capacity);
    PriorityQueue(Comparable **elements, int length);

private:
    static const int CAPACIDADE_INICIAL = 50;
    Comparable** array;
    int size;
    int capacity;

public:
    void insert(Comparable* c);
    Comparable* retrieveMin();
    Comparable* removeMin();
    bool isEmpty();
    int getSize();
    static void heapSort(Comparable **elements, int length);

private:
    void duplicateArray();
    void heapify(int hole);
    void buildHeap();

};

#endif // PRIORITYQUEUE_H
