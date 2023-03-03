#include <iostream>
#include "quad.h"

namespace Geometry
{
    
    template <class U>
    std::ostream &operator<<(std::ostream &os, const quad<U> &t)
    {
        os << "quad: " << *t.v1 << " , " << *t.v2 << " , " << *t.v3 << " , " << *t.v4;
        return os;
    }

    template <class T>
    bool quad<T>::containsEdge(edge<T> &e)
    {
        bool first = (almost_equal(*e.v1, *v1) || almost_equal(*e.v1, *v2) || almost_equal(*e.v1, *v3) || almost_equal(*e.v1, *v4));
        bool second = (almost_equal(*e.v2, *v1) || almost_equal(*e.v2, *v2) || almost_equal(*e.v2, *v3) || almost_equal(*e.v2, *v4));
        return (first && second);
    }

    template <class T>
    bool quad<T>::containsNode(node<T> &e)
    {
        bool first = (almost_equal(e, *v1) || almost_equal(e, *v2) || almost_equal(e, *v3) || almost_equal(e, *v4));
        return first;
    }
}