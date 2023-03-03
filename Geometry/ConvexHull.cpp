#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include "ConvexHull.h"

namespace Geometry
{

    std::string name1("Jarvis");
    template <class T>
    ConvexHull<T> ConvexHull<T>::Jarvis(name1);
    std::string name2("GrahamScan");
    template <class T>
    ConvexHull<T> ConvexHull<T>::GrahamScan(name1);

    template <class T>
    std::vector<node<T>> ConvexHull<T>::compute(std::vector<node<T>>& pts)
    {
        std::vector<node<T>> result;
        if (mode == "Jarvis") result = JarvisAlgorithm(pts);
        else if (mode == "GrahamScan") result = GrahamScanAlgorithm(pts);
        else throw std::runtime_error("Unknown algorithm");

        return result;
    }

    template <class T>
    int ConvexHull<T>::orientation(node<T> p, node<T> q, node<T> r)
    {
        int val = (q.y - p.y) * (r.x - q.x) -
                  (q.x - p.x) * (r.y - q.y);

        if (val == 0)
            return 0;
        return (val > 0) ? 1 : 2;
    }

    template <class T>
    node<T> ConvexHull<T>::nextToTop(std::stack<node<T>> &S)
    {
        node<T> p = S.top();
        S.pop();
        node<T> res = S.top();
        S.push(p);
        return res;
    }

    template <class T>
    void ConvexHull<T>::swap(node<T> &p1, node<T> &p2)
    {
        node<T> temp = p1;
        p1 = p2;
        p2 = temp;
    }

    template <class T>
    int ConvexHull<T>::distSq(node<T> p1, node<T> p2)
    {
        return (p1.x - p2.x) * (p1.x - p2.x) +
               (p1.y - p2.y) * (p1.y - p2.y);
    }

    template <class T>
    int ConvexHull<T>::compare(const node<T>& p1, const node<T>& p2, node<T> &p0)
    {
        // Find orientation
        int o = orientation(p0, p1, p2);
        if (o == 0)
            return (distSq(p0, p2) >= distSq(p0, p1)) ? -1 : 1;

        return (o == 2) ? -1 : 1;
    }

    template <class T>
    std::vector<node<T>> ConvexHull<T>::mySort(std::vector<node<T>> &pts, node<T>& p0)
    {
        for (int i = 1; i < pts.size() - 1; i++)
        {
            int min_pos = i;
            for (int j = i + 1; j < pts.size(); j++)
            {
                if (compare(pts[j],pts[min_pos],p0))
                    min_pos = j;
            }

            if (min_pos != i)
                std::swap(pts[i], pts[min_pos]);
        }
    }

    template <class T>
    std::vector<node<T>> ConvexHull<T>::JarvisAlgorithm(std::vector<node<T>>& _nodes)
    {
        std::vector<node<T>> hull;
        int j = 0;
        for (int i = 0; i < _nodes.size(); i++)
        {
            if (_nodes[i].x < _nodes[j].x)
                j = i;
        }
        int p = j, q;
        do
        {
            hull.push_back(_nodes[p]);
            q = (p + 1) % (_nodes.size());
            for (int i = 0; i < _nodes.size(); i++)
            {
                if (orientation(_nodes[p], _nodes[i], _nodes[q]) == 2)
                {
                    q = i;
                }
            }
            p = q;
        } while (p != j);

        return hull;
    }

    template <class T>
    std::vector<node<T>> ConvexHull<T>::GrahamScanAlgorithm(std::vector<node<T>>& points)
    {
        int ymin = points[0].y, min = 0;
        for (int i = 1; i < points.size(); i++)
        {
            int y = points[i].y;

            if ((y < ymin) || (ymin == y &&
                               points[i].x < points[min].x))
                ymin = points[i].y, min = i;
        }

        swap(points[0], points[min]);
        node<T> p0 = points[0];
        points = mySort(points, p0);

        int m = 1;
        for (int i = 1; i < points.size(); i++)
        {
            while (i < points.size() - 1 && orientation(p0, points[i],
                                            points[i + 1]) == 0)
                i++;

            points[m] = points[i];
            m++;
        }

        if (m < 3)
            throw std::runtime_error("Not enough points in convex hull");

        std::stack<node<T>> S;
        S.push(points[0]);
        S.push(points[1]);
        S.push(points[2]);

        for (int i = 3; i < m; i++)
        {
            while (S.size() > 1 && orientation(nextToTop(S), S.top(), points[i]) != 2)
                S.pop();
            S.push(points[i]);
        }

        std::vector<node<T>> hull;
        while (!S.empty())
        {
            node<T> p = S.top();
            hull.push_back(p);
            S.pop();
        }

        return hull;
    }


}