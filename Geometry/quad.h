#ifndef H_QUAD
#define H_QUAD

#include <iostream>
#include "node.h"

namespace Geometry
{

    template <class T>
    struct quad
    {
        quad(const node<T> &_v1, const node<T> &_v2, const node<T> &_v3, const node<T> &_v4)
            : v1(&_v1), v2(&_v2), v3(&_v3), v4(&_v4) {}

        template <class U>
        friend std::ostream &operator<<(std::ostream &os, const quad<U> &t);

        bool containsEdge(edge<T> &e);
        bool containsNode(node<T> &n);

        const node<T> *v1;
        const node<T> *v2;
        const node<T> *v3;
        const node<T> *v4;
        bool isbad = false;
    };

} // namespace Geometry

#include "quad.cpp"

#endif