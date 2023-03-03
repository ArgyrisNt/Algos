#ifndef H_NODE
#define H_NODE

#include <iostream>
#include <limits>
#include <cmath>

namespace Geometry 
{

    template<class T>
    struct node
    {
        node() {};
        node(T _x, T _y) : x(_x), y(_y) {}

        T dist2(const node<T> &v) const;
        T norm2() const;

        int isLeft(node<T> P0, node<T> P1, node<T> P2);
        int is_inside(std::vector<node<T>> &pts);
        bool is_onBoundary(std::vector<node<T>> &pts);

        bool operator==(const node<T> &);
        node<T> operator+(const node<T> &);
        node<T> operator/(const double);
        node<T> operator*(const double);

        template<class U>
        friend std::ostream &operator<<(std::ostream &os, const node<U> &v);

        T x;
        T y;
    };

} // namespace Geometry

#include "node.cpp"

#endif