#include <iostream>
#include <vector>
#include <typeinfo>
#include "delaunay.h"

namespace Geometry
{

    template <class T>
    const std::vector<triangle<T>>& delaunay<T>::triangulate(std::vector<node<T>> &_nodes)
    {
        // Bowyer-Watson Algorithm
        nodes = _nodes;
        edges = {};
        triangles = {};
        T minX = nodes[0].x;
        T minY = nodes[0].y;
        T maxX = minX;
        T maxY = minY;

        for (std::size_t i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i].x < minX) minX = nodes[i].x;
            if (nodes[i].y < minY) minY = nodes[i].y;
            if (nodes[i].x > maxX) maxX = nodes[i].x;
            if (nodes[i].y > maxY) maxY = nodes[i].y;
        }

        const T dx = maxX - minX;
        const T dy = maxY - minY;
        const T deltaMax = std::max(dx, dy);
        const T midx = (minX + maxX) / 2;
        const T midy = (minY + maxY) / 2;

        const node<T> p1(midx - 20 * deltaMax, midy - deltaMax);
        const node<T> p2(midx, midy + 20 * deltaMax);
        const node<T> p3(midx + 20 * deltaMax, midy - deltaMax);

        triangles.push_back(triangle<T>(p1,p2,p3));

        for (auto p = begin(nodes); p!= end(nodes); ++p)
        {
            std::vector<edge<T>> polygon;

            for (auto & t : triangles)
            {
                if (t.circumCircleContains(*p))
                {
                    t.isbad = true;
                    polygon.push_back(edge<T>(*t.v1,*t.v2));
                    polygon.push_back(edge<T>(*t.v2, *t.v3));
                    polygon.push_back(edge<T>(*t.v3, *t.v1));
                }
            }

            triangles.erase(std::remove_if(begin(triangles),end(triangles),[](triangle<T> &t){
                return t.isbad;
            }),end(triangles));

            for (auto e1 = begin(polygon); e1!= end(polygon); ++e1)
            {
                for (auto e2 = e1 + 1; e2!= end(polygon); ++e2)
                {
                    if (almost_equal(*e1,*e2))
                    {
                        e1->isbad = true;
                        e2->isbad = true;
                    }
                }
            }

            polygon.erase(std::remove_if(begin(polygon),end(polygon),[](edge<T> &e){
                return e.isbad;
            }),end(polygon));

            for (const auto e : polygon)
            {
                triangles.push_back(triangle<T>(*e.v1,*e.v2,*p));
            }
            
        }

        triangles.erase(std::remove_if(begin(triangles),end(triangles),[p1,p2,p3](triangle<T> &t){
            return t.containsNode(p1) || t.containsNode(p2) || t.containsNode(p3);
        }),end(triangles));

        for (const auto t : triangles)
        {
            edges.push_back(edge<T>{*t.v1, *t.v2});
            edges.push_back(edge<T>{*t.v2, *t.v3});
            edges.push_back(edge<T>{*t.v3, *t.v1});
        }
        convex_hull = ConvexHull<T>::Jarvis.compute(nodes);

        std::cout << "Nodes: " << nodes.size() << std::endl;
        std::cout << "Edges: " << edges.size() << std::endl;
        std::cout << "Triangles: " << triangles.size() << std::endl;

        return triangles;
    }

    template <class T>
    std::vector<std::pair<edge<T>,int>> delaunay<T>::collect_encroached_edges()
    {
        std::vector<std::pair<edge<T>, int>> encroached_edges;
        int id = 0;
        for (auto edge_iter = edges.begin(); edge_iter!= edges.end(); edge_iter++)
        {
            for (auto node_iter = nodes.begin(); node_iter!= nodes.end(); node_iter++)
            {
                if ((*edge_iter).is_encroached_by_point(*node_iter))
                {
                    encroached_edges.push_back(std::make_pair(*edge_iter,id));
                    break;
                }
            }
            id++;
        }
        std::cout << "Encroached edges: " << encroached_edges.size() << std::endl;

        return encroached_edges;
    }

    template <class T>
    std::vector<triangle<T>> delaunay<T>::collect_bad_triangles(const double thress)
    {
        std::vector<triangle<T>> bad_triangles;
        for (auto triangle_iter = triangles.begin(); triangle_iter!= triangles.end(); triangle_iter++)
        {
            if ((*triangle_iter).is_bad(thress))
            {
                bad_triangles.push_back(*triangle_iter);
            }
        }
        std::cout << "Bad triangles: " << bad_triangles.size() << std::endl;

        return bad_triangles;;
    }

    template <class T>
    const std::vector<triangle<T>> &delaunay<T>::RuppertRefine(const double thress)
    {
        // Ruppert's Algorithm
        std::vector<std::pair<edge<T>, int>> encroached_edges = collect_encroached_edges();
        std::vector<triangle<T>> bad_triangles = collect_bad_triangles(thress);

        int i = 0;
        std::vector<node<T>> new_nodes;
        while (i < encroached_edges.size())
        {
            edge<T> edge_iter = encroached_edges[i].first;
            T midx = (edge_iter.v1->x + edge_iter.v2->x) / 2.0;
            T midy = (edge_iter.v1->y + edge_iter.v2->y) / 2.0;
            node<T> mid(midx, midy);
            new_nodes.push_back(mid);

            i++;
        }

        auto triangle_iter = bad_triangles.begin();
        while (triangle_iter != bad_triangles.end())
        {
            node<T> center = (*triangle_iter).findCircumCenter();
            bool counter = false;
            for (auto edge: encroached_edges)
            {
                if (edge.first.is_encroached_by_point(center))
                {
                    T midx = (edge.first.v1->x + edge.first.v2->x) / 2.0;
                    T midy = (edge.first.v1->y + edge.first.v2->y) / 2.0;
                    node<T> mid(midx, midy);
                    new_nodes.push_back(mid);
                    counter = true;
                    break;
                }
            }

            if (!counter && center.is_inside(convex_hull))
            {
                new_nodes.push_back(center);
            }

            triangle_iter++;
        }

        for (auto node : new_nodes)
        {
            nodes.push_back(node);
        }

        triangles = triangulate(nodes);

        return triangles;
    }

    template <class T>
    std::vector<node<T>> delaunay<T>::computeVoronoi()
    {
        std::vector<node<T>> VoronoiNodes;
        for (auto tr : triangles)
        {
            VoronoiNodes.push_back(tr.findCircumCenter());
        }

        std::vector<node<T>> VoronoiCentroids;
        for (auto n : nodes)
        {
            double sum_x = 0.0, sum_y = 0.0;
            int counter = 0;
            // find node's corresponding triangles
            for (int i = 0; i < triangles.size(); i++)
            {
                if (triangles[i].containsNode(n))
                {
                    sum_x += VoronoiNodes[i].x;
                    sum_y += VoronoiNodes[i].y;
                    counter++;  
                }
            }
            VoronoiCentroids.push_back(node<T>(sum_x/counter, sum_y/counter));
        }

        return VoronoiCentroids;
    }

    template <class T>
    const std::vector<triangle<T>> &delaunay<T>::LloydSmooth()
    {
        // Lloyd's Algorithm
        std::vector<node<T>> VoronoiCentroids = computeVoronoi();
        int cnt = 0;
        for (int i = 0; i < VoronoiCentroids.size(); i++)
        {
            if (VoronoiCentroids[i].is_inside(convex_hull) && nodes[i].is_inside(convex_hull))
            {
                cnt++;
                nodes[i] = VoronoiCentroids[i];
            }
        }
        std::cout << "Smoothed " << cnt << " nodes" << std::endl;
        triangles = triangulate(nodes);

        return triangles;
    }

    template <class T>
    std::vector<node<T>> delaunay<T>::findNeighbours(node<T> n)
    {
        std::vector<node<T>> neighbours;
        for (auto ed : edges)
        {
            if (almost_equal(*ed.v1, n) && std::find(neighbours.begin(), neighbours.end(), *ed.v2) == neighbours.end())
            {
                neighbours.push_back(*ed.v2);
            }
            else if (almost_equal(*ed.v2, n) && std::find(neighbours.begin(), neighbours.end(), *ed.v1) == neighbours.end())
            {
                neighbours.push_back(*ed.v1);
            }
        }
        return neighbours;
    }

    template <class T>
    const std::vector<triangle<T>> &delaunay<T>::LaplaceSmooth()
    {
        std::vector<std::vector<node<T>>> neighbours;
        for (int i = 0; i < nodes.size(); i++)
        {
            neighbours.push_back(findNeighbours(nodes[i]));
        }
        int cnt = 0;
        for (int i = 0; i < nodes.size(); i++)
        {
            double sum_x = 0.0, sum_y = 0.0;
            for (auto el : neighbours[i])
            {
                sum_x += el.x;
                sum_y += el.y;
            }
            node<T> new_node(sum_x / neighbours[i].size(), sum_y / neighbours[i].size());
            if (new_node.is_inside(convex_hull) && nodes[i].is_inside(convex_hull)) 
            {
                nodes[i] = new_node;
                cnt++;
            }
        }
        std::cout << "Smoothed " << cnt << " nodes" << std::endl;
        triangles = triangulate(nodes);

        return triangles;
    }

    template <class T>
    std::vector<quad<T>> &delaunay<T>::ConvertToQuads(std::vector<node<T>> &pts, int steps)
    {
        // - - - - - - - - - - 1st step - - - - - - - - - -
        nodes = {};
        // polygon from boundary is the initial face;
        double sum_x = 0.0, sum_y = 0.0;
        for (auto pt : convex_hull)
        {
            sum_x += pt.x;
            sum_y += pt.y;
        }
        node<T> *init_face_point = new node<T>(sum_x / convex_hull.size(), sum_y / convex_hull.size());

        // update faces (triangles in this face)
        triangles = {};
        edges = {};
        for (int i = 0; i < convex_hull.size(); i++)
        {
            int j = (i + 1) % convex_hull.size();
            triangles.push_back(triangle<T>(convex_hull[i], *init_face_point, convex_hull[j]));
            edges.push_back(edge<T>{convex_hull[i], *init_face_point});
        }

        // - - - - - - - - - - 2nd step - - - - - - - - - -
        std::vector<node<T>> *face_points = new std::vector<node<T>>;
        for (auto t : triangles)
        {
            double temp_x = (t.v1->x + t.v2->x + t.v3->x) / 3.0;
            double temp_y = (t.v1->y + t.v2->y + t.v3->y) / 3.0;
            node<T>* temp = new node<T>(temp_x, temp_y);
            face_points->push_back(*temp);
            nodes.push_back(*temp);
        }
        
        // compute edge points and mid points
        std::vector<node<T>> *edge_points = new std::vector<node<T>>;
        std::vector<node<T>> *mid_points = new std::vector<node<T>>;
        for (int i = 0; i < edges.size(); i++)
        {
            int j;
            if (i == 0) j = convex_hull.size() - 1;
            else j = i - 1;
            node<T>* mid_point = new node<T>((edges[i].v1->x + edges[i].v2->x) / 2.0, (edges[i].v1->y + edges[i].v2->y) / 2.0);
            node<T>* avg_face_point = new node<T>(((*face_points)[i].x + (*face_points)[j].x) / 2.0, ((*face_points)[i].y + (*face_points)[j].y) / 2.0);
            edge_points->push_back((*avg_face_point + *mid_point) / 2.0);
            mid_points->push_back(*mid_point);
            nodes.push_back((*avg_face_point + *mid_point) / 2.0);
        }

        // compute final new point
        double f_sum_x = 0.0, f_sum_y = 0.0, m_sum_x = 0.0, m_sum_y = 0.0;
        for (int i = 0; i < face_points->size(); i++)
        {
            f_sum_x += (*face_points)[i].x;
            f_sum_y += (*face_points)[i].y;
            m_sum_x += (*mid_points)[i].x;
            m_sum_y += (*mid_points)[i].y;
        }
        node<T>* F = new node<T>(f_sum_x / face_points->size(), f_sum_y / face_points->size());
        node<T>* R = new node<T>(m_sum_x / mid_points->size(), m_sum_y / mid_points->size());
        node<T>* new_point = new node<T>((*F + (*R) * 2.0 + *init_face_point * (face_points->size() - 3)) / face_points->size());

        nodes.push_back(*new_point);

        // compute new faces (quads) and new edges
        quads = {};
        edges = {};
        for (int i = 0; i < triangles.size(); i++)
        {
            int j = (i + 1) % edge_points->size();
            double local_mid_x = (triangles[i].v1->x + triangles[i].v3->x) / 2.0;
            double local_mid_y = (triangles[i].v1->y + triangles[i].v3->y) / 2.0;
            const node<T>* local_mid = new node<T>(local_mid_x, local_mid_y);
            nodes.push_back(*local_mid);
            quad<T>* new_quad1 = new quad<T>(*triangles[i].v1, (*edge_points)[i], (*face_points)[i], *local_mid);
            quad<T> *new_quad2 = new quad<T>(*local_mid, (*face_points)[i], (*edge_points)[j], *triangles[i].v3);
            quad<T> *new_quad3 = new quad<T>((*edge_points)[i], *new_point, (*edge_points)[j], (*face_points)[i]);
            new_quad1->isbad = true;
            new_quad2->isbad = true;
            quads.push_back(*new_quad1);
            quads.push_back(*new_quad2);
            quads.push_back(*new_quad3);

            edges.push_back(edge<T>(*new_quad1->v1, *new_quad1->v2));
            edges.push_back(edge<T>(*new_quad1->v2, *new_quad1->v3));
            edges.push_back(edge<T>(*new_quad1->v3, *new_quad1->v4));
            edges.push_back(edge<T>(*new_quad1->v4, *new_quad1->v1));
            edges.push_back(edge<T>(*new_quad2->v1, *new_quad2->v2));
            edges.push_back(edge<T>(*new_quad2->v2, *new_quad2->v3));
            edges.push_back(edge<T>(*new_quad2->v3, *new_quad2->v4));
            edges.push_back(edge<T>(*new_quad2->v4, *new_quad2->v1));
            edges.push_back(edge<T>(*new_quad3->v1, *new_quad3->v2));
            edges.push_back(edge<T>(*new_quad3->v2, *new_quad3->v3));
            edges.push_back(edge<T>(*new_quad3->v3, *new_quad3->v4));
            edges.push_back(edge<T>(*new_quad3->v4, *new_quad3->v1));
        }

        int step = 0;
        while (step < steps)
        {
            // new face points
            std::vector<node<T>>* new_face_points = new std::vector<node<T>>;
            for (auto quad : quads)
            {
                double temp_x = (quad.v1->x + quad.v2->x + quad.v3->x + quad.v4->x) / 4.0;
                double temp_y = (quad.v1->y + quad.v2->y + quad.v3->y + quad.v4->y) / 4.0;
                node<T> *temp = new node<T>(temp_x, temp_y);
                new_face_points->push_back(*temp);
            }

            // new edge points
            std::vector<node<T>> *new_edge_points = new std::vector<node<T>>;
            std::vector<node<T>> *new_mid_points = new std::vector<node<T>>;
            for (auto ed : edges)
            {
                double sum_x = 0.0, sum_y = 0.0;
                int cnt = 0;
                for (int i = 0; i < quads.size(); i++)
                {
                    if (quads[i].containsEdge(ed))
                    {
                        sum_x += (*new_face_points)[i].x;
                        sum_y += (*new_face_points)[i].y;
                        cnt++;
                    }
                }
                node<T> *mid_point = new node<T>((ed.v1->x + ed.v2->x) / 2.0, (ed.v1->y + ed.v2->y) / 2.0);
                node<T> *avg_face_point = new node<T>(sum_x / cnt, sum_y / cnt);
                new_edge_points->push_back((*avg_face_point + *mid_point) / 2.0);
                new_mid_points->push_back(*mid_point);
            }

            // new final point
            for (int i = 0; i < nodes.size(); i++)
            {
                double sum_x = 0.0, sum_y = 0.0;
                int cnt = 0;
                for (int j = 0; j < quads.size(); j++)
                {
                    if (quads[j].containsNode(nodes[i]))
                    {
                        sum_x += (*new_face_points)[j].x;
                        sum_y += (*new_face_points)[j].y;
                        cnt++;
                    }
                }
                node<T> *F = new node<T>(sum_x / cnt, sum_y / cnt);

                sum_x = 0.0, sum_y = 0.0;
                cnt = 0;
                for (int j = 0; j < edges.size(); j++)
                {
                    if (edges[j].containsNode(nodes[i]))
                    {
                        sum_x += (*new_mid_points)[j].x;
                        sum_y += (*new_mid_points)[j].y;
                        cnt++;
                    }
                }
                node<T> *R = new node<T>(sum_x / cnt, sum_y / cnt);
                node<T> *new_point = new node<T>((*F + (*R) * 2.0 + nodes[i] * (cnt - 3)) / cnt);
            }

            // new quads and edges
            nodes = {};
            edges = {};
            std::vector<quad<T>>* new_quads = new std::vector<quad<T>>;
            for (int i = 0; i < quads.size(); i++)
            {
                int j = i * 3 + i;
                node<T> *local_mid = &(*new_edge_points)[j + 3];
                double local_mid_x, local_mid_y;
                if (quads[i].isbad)
                {
                    local_mid_x = (quads[i].v1->x + quads[i].v4->x) / 2.0;
                    local_mid_y = (quads[i].v1->y + quads[i].v4->y) / 2.0;
                    local_mid = new node<T>(local_mid_x, local_mid_y);
                }
                quad<T> *new_quad1 = new quad<T>(*quads[i].v1, (*new_edge_points)[j], (*new_face_points)[i], *local_mid);
                quad<T> *new_quad2 = new quad<T>(*local_mid, (*new_face_points)[i], (*new_edge_points)[j + 2], *quads[i].v4);
                quad<T> *new_quad3 = new quad<T>((*new_edge_points)[j], *quads[i].v2, (*new_edge_points)[j + 1], (*new_face_points)[i]);
                quad<T> *new_quad4 = new quad<T>((*new_face_points)[i], (*new_edge_points)[j + 1], *quads[i].v3, (*new_edge_points)[j + 2]);
                if (quads[i].isbad)
                {
                    new_quad1->isbad = true;
                    new_quad2->isbad = true;
                }
                new_quads->push_back(*new_quad1);
                new_quads->push_back(*new_quad2);
                new_quads->push_back(*new_quad3);
                new_quads->push_back(*new_quad4);

                edges.push_back(edge<T>(*new_quad1->v1, *new_quad1->v2));
                edges.push_back(edge<T>(*new_quad1->v2, *new_quad1->v3));
                edges.push_back(edge<T>(*new_quad1->v3, *new_quad1->v4));
                edges.push_back(edge<T>(*new_quad1->v4, *new_quad1->v1));
                edges.push_back(edge<T>(*new_quad2->v1, *new_quad2->v2));
                edges.push_back(edge<T>(*new_quad2->v2, *new_quad2->v3));
                edges.push_back(edge<T>(*new_quad2->v3, *new_quad2->v4));
                edges.push_back(edge<T>(*new_quad2->v4, *new_quad2->v1));
                edges.push_back(edge<T>(*new_quad3->v1, *new_quad3->v2));
                edges.push_back(edge<T>(*new_quad3->v2, *new_quad3->v3));
                edges.push_back(edge<T>(*new_quad3->v3, *new_quad3->v4));
                edges.push_back(edge<T>(*new_quad3->v4, *new_quad3->v1));
                edges.push_back(edge<T>(*new_quad4->v1, *new_quad4->v2));
                edges.push_back(edge<T>(*new_quad4->v2, *new_quad4->v3));
                edges.push_back(edge<T>(*new_quad4->v3, *new_quad4->v4));
                edges.push_back(edge<T>(*new_quad4->v4, *new_quad4->v1));

                nodes.push_back(*new_quad1->v2);
                nodes.push_back(*new_quad2->v2);
                nodes.push_back(*new_quad3->v2);
                nodes.push_back(*new_quad4->v2);
            }

            quads = *new_quads;
            step++;
        }

        return quads;
    }   

}