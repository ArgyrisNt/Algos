// Example of Delaunay triangulation and some refinement algorithms

/*
g++ -IC:\SFML-2.5.1\include -c ex1.cpp -o ex1.o
g++ -LC:\SFML-2.5.1\lib .\ex1.o -o ex1.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows

int numberPoints = 6;
std::vector<Geometry::node<double>> points;
points.push_back(Geometry::node<double>{100.0, 100.0});
points.push_back(Geometry::node<double>{150.0, 300.0});
points.push_back(Geometry::node<double>{200.0, 300.0});
points.push_back(Geometry::node<double>{300.0, 100.0});
points.push_back(Geometry::node<double>{300.0, 500.0});
points.push_back(Geometry::node<double>{500.0, 300.0});
*/

#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include "..\Geometry\delaunay.h"
#include "..\Geometry\ConvexHull.h"
#include <SFML/Graphics.hpp>
#include <iterator>
#include <array>

void plot_points(sf::RenderWindow &wind, std::vector<Geometry::node<double>> _nodes)
{
    for (const auto p : _nodes)
    {
        sf::RectangleShape s{sf::Vector2f(4, 4)};
        s.setPosition(static_cast<float>(p.x), static_cast<float>(p.y));
        wind.draw(s);
    }
}

void plot_edges(sf::RenderWindow &wind, std::vector<Geometry::edge<double>> _edges)
{
    std::vector<std::array<sf::Vertex, 2>> lines;
    for (const auto &e : _edges)
    {
        const std::array<sf::Vertex, 2> line{{
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v1->x + 2.),
                static_cast<float>(e.v1->y + 2.))),
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v2->x + 2.),
                static_cast<float>(e.v2->y + 2.))),
        }};
        wind.draw(line.data(), 2, sf::Lines);
    }
}

void plot_quads(sf::RenderWindow &wind, std::vector<Geometry::quad<double>> _quads)
{
    std::vector<std::array<sf::Vertex, 2>> lines;
    for (const auto &e : _quads)
    {
        const std::array<sf::Vertex, 2> line1{{
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v1->x + 2.),
                static_cast<float>(e.v1->y + 2.))),
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v2->x + 2.),
                static_cast<float>(e.v2->y + 2.))),
        }};
        const std::array<sf::Vertex, 2> line2{{
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v2->x + 2.),
                static_cast<float>(e.v2->y + 2.))),
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v3->x + 2.),
                static_cast<float>(e.v3->y + 2.))),
        }};
        const std::array<sf::Vertex, 2> line3{{
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v3->x + 2.),
                static_cast<float>(e.v3->y + 2.))),
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v4->x + 2.),
                static_cast<float>(e.v4->y + 2.))),
        }};
        const std::array<sf::Vertex, 2> line4{{
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v4->x + 2.),
                static_cast<float>(e.v4->y + 2.))),
            sf::Vertex(sf::Vector2f(
                static_cast<float>(e.v1->x + 2.),
                static_cast<float>(e.v1->y + 2.))),
        }};
        wind.draw(line1.data(), 2, sf::Lines);
        wind.draw(line2.data(), 2, sf::Lines);
        wind.draw(line3.data(), 2, sf::Lines);
        wind.draw(line4.data(), 2, sf::Lines);
    }
}


int main()
{

    // - - - - - - - - - - - - - - - - Define a set of points - - - - - - - - - - - - - - - -

    // int numberPoints = 40;
    // std::default_random_engine eng(std::random_device{}());
    // std::uniform_real_distribution<double> dist_w(0, 800);
    // std::uniform_real_distribution<double> dist_h(0, 600);
    // std::cout << "Generating " << numberPoints << " random points" << std::endl;
    // std::vector<Geometry::node<double>> points;
    // for (int i = 0; i < numberPoints; ++i)
    // {
    //     points.push_back(Geometry::node<double>{dist_w(eng), dist_h(eng)});
    // }

    int numberPoints = 6;
    std::vector<Geometry::node<double>> points;
    points.push_back(Geometry::node<double>{100.0, 100.0});
    points.push_back(Geometry::node<double>{150.0, 300.0});
    points.push_back(Geometry::node<double>{200.0, 300.0});
    points.push_back(Geometry::node<double>{300.0, 100.0});
    points.push_back(Geometry::node<double>{300.0, 500.0});
    points.push_back(Geometry::node<double>{500.0, 300.0});

    // - - - - - - - - - - - - - - - - Delaunay triangulation - - - - - - - - - - - - - - - -

    Geometry::delaunay<double> triangulation;
    std::vector<Geometry::triangle<double>> triangles = triangulation.triangulate(points);


    // - - - - - - - - - - - - - - - - - Ruppert's Refinement - - - - - - - - - - - - - - - -

    double thresshold = 25.0;
    for (int i = 0; i < 0; i++)
    {
        std::cout << "\nRefinement . . . \n";
        triangles = triangulation.RuppertRefine(thresshold);
    }


    // - - - - - - - - - - - - - - - - - - Lloyd's Smoothing - - - - - - - - - - - - - - - - -

    for (int i = 0; i < 0; i++)
    {
        std::cout << "\nSmoothing . . . \n";
        triangles = triangulation.LloydSmooth();
    }

    
    // - - - - - - - - - - - - - - - - - - Laplace Smoothing - - - - - - - - - - - - - - - - -
    
    //std::cout << "\nSmoothing . . . \n";
    //triangles = triangulation.LaplaceSmooth();


    // - - - - - - - - - - - - - - - - - - Catmull-Clark Quadrangulation - - - - - - - - - - - - - - - - -
    const auto start = std::chrono::high_resolution_clock::now();
    std::vector<Geometry::quad<double>> new_quads = triangulation.ConvertToQuads(points, 5);
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << std::endl;
    

    // - - - - - - - - - - - - - - - - - - - Plot with SFML - - - - - - - - - - - - - - - - - -

    sf::RenderWindow window(sf::VideoMode(800, 600), "Delaunay triangulation");
    window.setFramerateLimit(1);

    //plot_points(window, triangulation.getNodes());
    plot_edges(window, triangulation.getEdges());
    //plot_quads(window, new_quads);

    window.display();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}