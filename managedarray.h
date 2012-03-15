#ifndef MANAGEDARRAY_H
#define MANAGEDARRAY_H

#include <cstdlib>

//This implementation is specifically designed for use with arrays of doubles.
class ManagedArray
{
    double **data;
    int m_max;

public:
    ManagedArray(int sz = 0) : m_max(sz) { sz > 0 ? data = new double*[sz] : data = NULL; }

    ~ManagedArray() { purge(); }

    void purge();

    double *&operator[](int index) { return data[index]; }

    int max() { return m_max; }

    void grow();
    //Will halve the array size, but likely cause data loss.
    //void shrink();
};
/*

template <class T>
class ManagedArray
{
    T *data;
    int m_max;

public:
    ManagedArray(int sz = 0) : m_max(sz) { sz > 0 ? data = new T[sz] : data = NULL; }

    ~ManagedArray() { purge(); }

    void purge();

    T& operator[](int index) { return data[index]; }

    int max() { return m_max; }

    void grow();
    //Will halve the array size, but likely cause data loss.
    //void shrink();
};*/

#endif // MANAGEDARRAY_H
