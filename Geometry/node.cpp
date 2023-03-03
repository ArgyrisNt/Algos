#include <iostream>
#include "node.h"

namespace Geometry
{

    template <class T>
    int node<T>::isLeft(node<T> P0, node<T> P1, node<T> P2)
    {
        return ((P1.x - P0.x) * (P2.y - P0.y) - (P2.x - P0.x) * (P1.y - P0.y));
    }

    template <class T>
    int node<T>::is_inside(std::vector<node<T>>& pts)
    {
        if (is_onBoundary(pts)) return 0;
        
        int cn = 0;
        for (int i = 0; i < pts.size(); i++)
        {
            if (((pts[i].y <= y) && (pts[i + 1].y > y)) || ((pts[i].y > y) && (pts[i + 1].y <= y)))
            {
                float vt = (float)(y - pts[i].y) / (pts[i + 1].y - pts[i].y);
                if (x < pts[i].x + vt * (pts[i + 1].x - pts[i].x))
                    ++cn;                                      
            }
        }
        return (cn & 1); 
    }

    template <class T>
    bool node<T>::is_onBoundary(std::vector<node<T>> &pts)
    {
        for (int i = 0; i < pts.size(); i++)
        {
            if (almost_equal(pts[i], *this))
            {
                return true;
                break;
            }
        }
        return false;
    }

    template <class U>
    std::ostream &operator<<(std::ostream &os, const node<U> &v)
    {
        os << "(" << v.x << " , " << v.y << ")";
        return os;
    }

    template <class T>
    bool almost_equal(const node<T> &v1, const node<T> &v2)
    {
        return (fabs(v1.x - v2.x) <= std::numeric_limits<double>::epsilon() * fabs(v1.x + v2.x) * static_cast<double>(2) || fabs(v1.x - v2.x) < std::numeric_limits<double>::min()) &&
               (fabs(v1.y - v2.y) <= std::numeric_limits<double>::epsilon() * fabs(v1.y + v2.y) * static_cast<double>(2) || fabs(v1.y - v2.y) < std::numeric_limits<double>::min());
    }

    template <class T>
    T node<T>::dist2(const node<T> &v) const
    {
        return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y);
    }

    template <class T>
    T node<T>::norm2() const
    {
        return x * x + y * y;
    }

    template <class T>
    bool node<T>::operator==(const node<T> &n)
    {
        return almost_equal(*this, n);
    }

    template <class T>
    node<T> node<T>::operator+(const node<T> &n)
    {
        node<T> res((*this).x + n.x, (*this).y + n.y);
        return res;
    }

    template <class T>
    node<T> node<T>::operator/(const double n)
    {
        node<T> res((*this).x / n, (*this).y / n);
        return res;
    }

    template <class T>
    node<T> node<T>::operator*(const double n)
    {
        node<T> res((*this).x * n, (*this).y * n);
        return res;
    }

}