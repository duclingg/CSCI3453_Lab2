#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <exception>

template <typename T>
class Queue {
public:
    Queue<T>() { arr = new T[1]; used = 0; cap = 1; }
    Queue<T>(const Queue& d);
    explicit Queue<T>(unsigned newCap)
        { arr = new T[newCap]; used = 0; cap = newCap; }
    ~Queue<T>() { delete [] arr; }
    
    // Constant members
    unsigned size() const { return used; }
    unsigned capacity() const { return cap; }
    const T* find(const T&) const;
    T front() const;
    T back() const;
    
    // Modification members
    T& back();
    void pop_back();
    void push_back(const T&);
    T& front();
    void pop_front();
    void push_front(const T&);  // added 2015/07/08 for queue FIFO ops
    T* find(const T&);
    void resize(unsigned);
    void moveToBack(int);
    void moveToFront(int);
    
    // Operators
    const T& operator [ ] (unsigned) const;
    T& operator [ ] (unsigned);
    Queue& operator = (const Queue&);
    void operator += (const Queue&);
    
private:
    T* arr;
    unsigned used, cap;

    class badBracket
    {
    public:
        badBracket(unsigned index)
        {
            std::cout << "\n**out of bounds, target = " << index << "**" << std::endl;
        }
    };

};

#include "Queue.cpp"
#endif