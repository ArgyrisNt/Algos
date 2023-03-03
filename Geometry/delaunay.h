#ifndef H_DELAUNAY
#define H_DELAUNAY

#include <iostream>
#include <vector>

#include "node.h"
#include "edge.h"
#include "triangle.h"
#include "quad.h"
#include "ConvexHull.h"

namespace Geometry
{

    template <class T>
    class delaunay
    {
        public:
            const std::vector<triangle<T>> &triangulate(std::vector<node<T>> &_nodes); // Bowyer-Watson Algorithm
            const std::vector<triangle<T>> &RuppertRefine(const double thress); // Ruppert's Algorithm
            const std::vector<triangle<T>> &LloydSmooth(); // Lloyd's Algorithm
            const std::vector<triangle<T>> &LaplaceSmooth(); // Laplace Smoothing
            std::vector<quad<T>> &ConvertToQuads(std::vector<node<T>> &pts, int steps); // Catmull-Clark Subdivision

            const std::vector<triangle<T>> &getTriangles() const { return triangles; }
            const std::vector<edge<T>> &getEdges() const { return edges; }
            const std::vector<node<T>> &getNodes() const { return nodes; }                     

        private:
            std::vector<std::pair<edge<T>, int>> collect_encroached_edges();
            std::vector<triangle<T>> collect_bad_triangles(const double thress);
            std::vector<node<T>> computeVoronoi();
            std::vector<node<T>> findNeighbours(node<T> n);

            std::vector<triangle<T>> triangles;
            std::vector<edge<T>> edges;
            std::vector<node<T>> nodes;
            std::vector<quad<T>> quads;
            std::vector<node<T>> convex_hull;
    };

} // namespace Geometry

#include "delaunay.cpp"

#endif