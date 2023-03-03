#include <iostream>
#include "triangle.h"

namespace Geometry
{

    template <class T>
    T triangle<T>::dot(node<T> &a, node<T> &b)
    {
        T sum = a.x * b.x + a.y * b.y;
        return sum;
    }

    template <class T>
    bool triangle<T>::is_bad(const double thress)
    {
        node<T> a(v2->x - v1->x, v2->y - v1->y);
        node<T> aa(-v2->x + v1->x, -v2->y + v1->y);
        node<T> b(v3->x - v2->x, v3->y - v2->y);
        node<T> bb(-v3->x + v2->x, -v3->y + v2->y);
        node<T> c(v1->x - v3->x, v1->y - v3->y);
        node<T> cc(-v1->x + v3->x, -v1->y + v3->y);

        double angle_ac = (180.0 / M_PI) * acos(dot(a, cc) / (sqrt(a.norm2()) * sqrt(cc.norm2())));
        double angle_ab = (180.0 / M_PI) * acos(dot(aa, b) / (sqrt(aa.norm2()) * sqrt(b.norm2())));
        double angle_bc = (180.0 / M_PI) * acos(dot(bb, c) / (sqrt(bb.norm2()) * sqrt(c.norm2())));

        return (angle_ac < thress || angle_ab < thress || angle_bc < thress);
    }

    template <class U>
    std::ostream &operator<<(std::ostream &os, const triangle<U> &t)
    {
        os << "triangle: " << *t.v1 << " , " << *t.v2 << " , " << *t.v3;
        return os;
    }

    template <class T>
    bool triangle<T>::containsNode(const node<T> &v) const
    {
        return almost_equal(*v1, v) || almost_equal(*v2, v) || almost_equal(*v3, v);
    }

    template <class T>
    bool triangle<T>::circumCircleContains(const node<T> &v) const
    {
        const T ab = v1->norm2();
        const T cd = v2->norm2();
        const T ef = v3->norm2();

        const T ax = v1->x;
        const T ay = v1->y;
        const T bx = v2->x;
        const T by = v2->y;
        const T cx = v3->x;
        const T cy = v3->y;

        const T circum_x = (ab * (cy - by) + cd * (ay - cy) + ef * (by - ay)) / (ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
        const T circum_y = (ab * (cx - bx) + cd * (ax - cx) + ef * (bx - ax)) / (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

        const node<T> circum(circum_x / 2, circum_y / 2);
        const T circum_radius = v1->dist2(circum);
        const T dist = v.dist2(circum);
        return dist <= circum_radius;
    }

    template <class T>
    void triangle<T>::lineFromPoints(const node<T>& P, const node<T>& Q, double &a, double &b, double &c)
    {
        a = Q.y - P.y;
        b = P.x - Q.x;
        c = a * (P.x) + b * (P.y);
    }

    template <class T>
    void triangle<T>::perpendicularBisectorFromLine(const node<T>& P, const node<T>& Q, double &a, double &b, double &c)
    {
        node<T> mid_point((P.x + Q.x) / 2, (P.y + Q.y) / 2);
        c = -b * (mid_point.x) + a * (mid_point.y);

        double temp = a;
        a = -b;
        b = temp;
    }

    template <class T>
    node<T> triangle<T>::lineLineIntersection(double a1, double b1, double c1,
                                              double a2, double b2, double c2)
    {
        double determinant = a1 * b2 - a2 * b1;
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        node<T> res(x, y);

        return res;
    }

    template <class T>
    node<T> triangle<T>::findCircumCenter()
    {
        double a, b, c;
        lineFromPoints(*v1, *v2, a, b, c);
        double e, f, g;
        lineFromPoints(*v2, *v3, e, f, g);

        perpendicularBisectorFromLine(*v1, *v2, a, b, c);
        perpendicularBisectorFromLine(*v2, *v3, e, f, g);

        node<T> circumcenter =
            lineLineIntersection(a, b, c, e, f, g);

        return circumcenter;
    }

    template <class T>
    bool triangle<T>::includesNode(const node<T> &n) const
    {
        /* Calculate area of triangle ABC */
        T A = abs((v1->x * (v2->y - v3->y) + v2->x * (v3->y - v1->y) + v3->x * (v1->y - v2->y)) / 2.0);

        /* Calculate area of triangle PBC */
        T A1 = abs((n.x * (v2->y - v3->y) + v2->x * (v3->y - n.y) + v3->x * (n.y - v2->y)) / 2.0);

        /* Calculate area of triangle PAC */
        T A2 = abs((v1->x * (n.y - v3->y) + n.x * (v3->y - v1->y) + v3->x * (v1->y - n.y)) / 2.0);

        /* Calculate area of triangle PAB */
        T A3 = abs((v1->x * (v2->y - n.y) + v2->x * (n.y - v1->y) + n.x * (v1->y - v2->y)) / 2.0);

        /* Check if sum of A1, A2 and A3 is same as A */
        return (A == A1 + A2 + A3);
    }

}