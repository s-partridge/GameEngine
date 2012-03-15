#include "managedarray.h"

void ManagedArray::purge()
{
    if(data != NULL)
    {
        delete [] data;
        data = NULL;
    }
}

void ManagedArray::grow()
{
    if(m_max > 0)
    {
        double **temp = new double*[m_max * 2];

        for(int x = 0; x < m_max; ++x)
        {
            temp[x] = data[x];
        }

        delete [] data;

        m_max *= 2;
    }
    else
    {
        m_max = 1;
        double **temp = new double*[m_max];
        data = temp;
    }
}

/*void ManagedArray::shrink()
{
    if(max > 1)
    {
        max /= 2;

        T *temp = new T[max];
        for(int x = 0; x < max; ++x)
        {
            temp[x] = data[x];
        }

        delete [] data;

        data = temp;
    }
}*/
