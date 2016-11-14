#include "priorityqueue.h"

PriorityQueue::PriorityQueue()
{
    this->size = 0;
    this->array = new Comparable*[CAPACIDADE_INICIAL+1];
    this->capacity = CAPACIDADE_INICIAL;
}

PriorityQueue::PriorityQueue(int capacity)
{
    this->size = 0;
    this->capacity = capacity;
    this->array = new Comparable*[capacity+1];
}

PriorityQueue::PriorityQueue(Comparable* *elements, int length)
{
    this->size = length;
    this->array = new Comparable*[length+1];
    for(int k=0;k<length;k++) this->array[k+1] = elements[k];
    this->buildHeap();
}

void PriorityQueue::insert(Comparable *c)
{
    if(this->size==this->capacity) this->duplicateArray();
    int hole = ++this->size;
    while(hole>1 && c->compareTo(this->array[hole/2])<0){
        this->array[hole] = this->array[hole/2];
        hole/=2;
    }
    this->array[hole] = c;
}

Comparable *PriorityQueue::retrieveMin()
{
    return this->array[1];
}

Comparable *PriorityQueue::removeMin()
{
    Comparable *min = this->retrieveMin();
    this->array[1] = this->array[this->size--];
    this->heapify(1);
    return min;
}

bool PriorityQueue::isEmpty()
{
    return (this->size==0);
}

int PriorityQueue::getSize()
{
    return this->size;
}

void PriorityQueue::heapSort(Comparable* *elements, int length)
{
    PriorityQueue *p = new PriorityQueue(elements, length);
    for(int k=0;k<length;k++){
        elements[k] = p->removeMin();
    }
}

void PriorityQueue::duplicateArray()
{
    this->capacity*=2;
    Comparable** novo = new Comparable*[this->capacity+1];
    for(int k=1,t=this->size+1;k<t;k++)
        novo[k] = this->array[k];
    this->array = novo;
}

void PriorityQueue::heapify(int hole)
{
    Comparable* aux = this->array[hole];
    int child = hole*2;
    bool isHeap = false;
    while(child<=this->size && !isHeap){
        if(child!=this->size && this->array[child+1]->compareTo(this->array[child])<0)
            child++;
        if(this->array[child]->compareTo(aux)<0){
            this->array[hole] = this->array[child];
            hole = child;
            child = hole*2;
        } else {
            isHeap = true;
        }
    }
    this->array[hole] = aux;
}

void PriorityQueue::buildHeap()
{
    for(int k=this->size/2;k>0;k--)
        this->heapify(k);
}

