#ifndef H_TRIANGLE
#define H_TRIANGLE

#include <iostream>
#include "node.h"
#include "edge.h"

namespace Geometry
{

    template<class T>
    struct triangle
    {
        triangle(const node<T>& _v1, const node<T>& _v2, const node<T>& _v3) : v1(&_v1), v2(&_v2), v3(&_v3) {}

        bool includesNode(const node<T> &n) const;
        bool containsNode(const node<T> &node) const;
        bool circumCircleContains(const node<T> &node) const;

        T dot(node<T> &a, node<T> &b);
        bool is_bad(const double thress);

        void lineFromPoints(const node<T> &P,
                            const node<T> &Q, double &a, double &b, double &c);
        void perpendicularBisectorFromLine(const node<T> &P,
                                           const node<T> &Q, double &a, double &b, double &c);
        node<T> lineLineIntersection(double a1, double b1, double c1,
                double a2, double b2, double c2);
        node<T> findCircumCenter();

        template <class U>
        friend std::ostream &operator<<(std::ostream &os, const triangle<U> &t);

        const node<T> *v1;
        const node<T> *v2;
        const node<T> *v3;
        bool isbad = false;
    };

} // namespace Geometry

#include "triangle.cpp"

#endif