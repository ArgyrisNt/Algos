#ifndef H_EDGE
#define H_EDGE

#include <iostream>
#include "node.h"

namespace Geometry
{

    template<class T>
    struct edge
    {
        edge(const node<T>& _v1, const node<T>& _v2) : v1(&_v1), v2(&_v2) {}
        T dot(std::vector<T> &a, std::vector<T> &b);
        bool is_encroached_by_point(node<T> &p);

        template<class U>
        friend std::ostream &operator<<(std::ostream &os, const edge<U> &e);
        bool containsNode(node<T> &n);
        
        const node<T>* v1;
        const node<T>* v2;
        bool isbad = false;
    };

} // namespace Geometry

#include "edge.cpp"

#endif