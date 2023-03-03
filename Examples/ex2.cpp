// Example of searching algorithms

#include <iostream>
#include <list>
#include <queue>
#include "..\Searching\SearchingAlgorithms.h"

int main()
{
    Searching::Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);

    Searching::SearchingAlgorithms search;
    search.BFS(g, 2);

    Searching::Node *root = search.createNode(1);
    root->left = search.createNode(2);
    root->right = search.createNode(3);
    root->left->left = search.createNode(4);
    root->left->right = search.createNode(5);
    root->left->right->left = search.createNode(9);
    root->right->left = search.createNode(6);
    root->right->right = search.createNode(7);
    root->right->right->left = search.createNode(15);

    std::cout << std::endl;
    search.preorder(root);
}