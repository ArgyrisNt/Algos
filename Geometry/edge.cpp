#include <iostream>
#include <vector>
#include "edge.h"

namespace Geometry
{

    template <class T>
    T edge<T>::dot(std::vector<T> &a, std::vector<T> &b)
    {
        T sum = 0;
        for (int i = 0; i < a.size(); i++)
            sum += a[i] * b[i];
        return sum;
    }

    template <class T>
    bool edge<T>::is_encroached_by_point(node<T> &p)
    {
        std::vector<T> a{v1->x - p.x, v1->y - p.y};
        std::vector<T> b{v2->x - p.x, v2->y - p.y};

        return (dot(a, b) < 0.0);
    }

    template <class U>
    std::ostream &operator<<(std::ostream &os, const edge<U> &e)
    {
        os << "edge: " << *e.v1 << " , " << *e.v2;
        return os;
    }

    template <class T>
    bool almost_equal(const edge<T> &e1, const edge<T> &e2)
    {
        return (almost_equal(*e1.v1, *e2.v1) && almost_equal(*e1.v2, *e2.v2)) ||
               (almost_equal(*e1.v1, *e2.v2) && almost_equal(*e1.v2, *e2.v1));
    }

    template <class T>
    bool edge<T>::containsNode(node<T> &e)
    {
        bool first = (almost_equal(e, *v1) || almost_equal(e, *v2));
        return first;
    }

}