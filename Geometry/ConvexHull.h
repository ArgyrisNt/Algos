#ifndef H_CONVEXHULL
#define H_CONVEXHULL

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include "node.h"

namespace Geometry
{

    template <class T>
    class ConvexHull
    {
        public:
            static ConvexHull<T> Jarvis;
            static ConvexHull<T> GrahamScan;

            ConvexHull(std::string _mode) : mode(_mode) {}

            std::vector<node<T>> compute(std::vector<node<T>>& pts);

        private:
            std::string mode;

            int orientation(node<T> p, node<T> q, node<T> r);
            node<T> nextToTop(std::stack<node<T>> &S);
            void swap(node<T> &p1, node<T> &p2);
            int distSq(node<T> p1, node<T> p2);
            int compare(const node<T>& vp1, const node<T>& vp2, node<T> &p0);
            std::vector<node<T>> mySort(std::vector<node<T>> &pts, node<T>& p0);

            std::vector<node<T>> JarvisAlgorithm(std::vector<node<T>>& pts);
            std::vector<node<T>> GrahamScanAlgorithm(std::vector<node<T>>& pts);
    };
}

#include "ConvexHull.cpp"

#endif