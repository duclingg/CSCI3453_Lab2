#include "queue.h"
#include <cassert>
#include <cstdlib>

template <typename T>
Queue<T>::Queue(const Queue& d) {
    arr = new T[d.cap];
    cap = d.cap;
    used = d.used;

    for (int i = 0; i < used; ++i) {
        arr[i] = d.arr[i];
    }
}

template <typename T>
const T& Queue<T>::operator [ ] (unsigned target) const {
    try {
        if (target>=used || target<0) {
            throw badBracket(target);
        }
    }
    catch(badBracket& b) {
        // error message output by badBracket constructor
    }
    
    return arr[target];
}

template <typename T>
T& Queue<T>::operator [ ] (unsigned target) {
    try {
        if (target>=used || target<0) {
            throw badBracket(target);
        } 
    }
    catch(badBracket& b) {
        // error message output by badBracket constructor
        // if (b.target > 0) { this.resize(b.target); } <--?
    }
    
    return arr[target];
}


template <typename T>
const T* Queue<T>::find(const T& target) const {
    for (int i = 0; i < used; ++i) {
        if (arr[i] == target) {
            return arr[i];
        }
    }
    
    return 0;
}

template <typename T>
T Queue<T>::front() const {
    return arr[0];
}

template <typename T>
T Queue<T>::back() const {
    return arr[used-1];
}

template <typename T>
T& Queue<T>::front() {
    return arr[0];
}

template <typename T>
T& Queue<T>::back() {
    return arr[used-1];
}

template <typename T>
T* Queue<T>::find(const T& target) {
    for (int i = 0; i < used; ++i) {
        if (arr[i] == target) {
            return arr[i];
        }
    }
    
    return 0;
} 

template <typename T>
void Queue<T>::pop_back() {
    if (used > 0) {
        T* temp = new T[cap];

        for (int i = 0; i < used-1; ++i) {
            temp[i] = arr[i];
        }
        delete [] arr;
        arr = temp;
        --used;
    }
}

template <typename T>
void Queue<T>::push_back(const T& addend) {
    assert(used <= cap);

    // resize array by 1 when needed
    if (used + 1 == cap) {
        T *newT = new T[cap+1]; 

        for (int i = 0; i < used; ++i) {
            newT[i] = arr[i];
        }

        newT[used] = addend;
        ++cap;
        ++used;
        delete [] arr;
        arr = newT;
    } else {
        arr[used] = addend;
        ++used;
    }
}


template <typename T>
void Queue<T>::resize (unsigned newSize) {
    T *newT = new T[newSize];
    
    for (int i = 0; i < newSize; ++i) {
        if (i < newSize) {
            newT[i] = arr[i];
        }
    }
    delete [] arr;
    arr = newT;

    if (used > newSize) {
        used = newSize;
    }
    cap = newSize;
}


template <typename T>
Queue<T>& Queue<T>::operator = (const Queue<T>& rHS) {
    
    T* temp = new T[rHS.capacity()];
    for (int i = 0; i < cap; ++i) {
        temp[i] = rHS[i];
    }
    delete [] arr;
    arr = temp;
    used = rHS.size();
    cap = rHS.capacity();
    
    return temp;
}


template <typename T>
void Queue<T>::operator += (const Queue<T>& rHS) {
    unsigned newSize = rHS.size() + used, pos = rHS.size();
    T* newT = new T[newSize];

    for (int i = 0; i < used; ++i) {
        newT[i] = arr[i];
    }
    
    for (int i = pos; i < newSize; ++i) {
        newT[i] = rHS[i];
    }

    delete [] arr;
    arr = newT;
    used = newSize;
    cap = newSize;
}


template <typename T>
void Queue<T>::pop_front() {
    if (used > 0) {
        T* temp = new T[cap];

        for (int i = 1; i < used; ++i) {
            temp[i-1] = arr[i];
        }
        delete [] arr;
        arr = temp;
        --used;
    }
}


template <typename T>
void Queue<T>::push_front(const T& insert) {
    if (used + 1 == cap) {
        T *newT = new T[cap+1];

        for (int i = 0; i < used; ++i) {
            newT[i] = arr[i];
        }
        ++cap;
        delete [] arr;
        arr = newT;
    }
    
    {
        for (int i = used; i > 0; --i) {
            arr[i] = arr[i-1];
        }
    }
    
    {
        arr[0] = insert;
        ++used;
    }
}

template <typename T>
void Queue<T>::moveToBack(int index) {
    if (used > 0) {
        T* temp = new T[cap];

        for (int i = 0; i < used; ++i) {
            if (i != index && i <= index) {
                temp[i] = arr[i];
            } else if (i > index) {
                temp[i-1] = arr[i];
            }
        }

        temp[used-1] = arr[index];
        delete [] arr;
        arr = temp;
    }
}


template <typename T>
void Queue<T>::moveToFront(int index) {
    if (used > 0) {
        T* temp = new T[cap];

        for (int i = 0; i < used; ++i) {
            if (i != index && i <= index) {
                temp[i+1] = arr[i];
            } else if (i > index) {
                temp[i] = arr[i];
            }
        }
        
        temp[0] = arr[index];
        delete [] arr;
        arr = temp;
    }
}