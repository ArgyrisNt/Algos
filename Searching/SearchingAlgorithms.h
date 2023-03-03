#include <iostream>
#include <vector>
#include "..\Searching\Graph.h"

namespace Searching
{

    struct Node
    {
        int data;
        Node *left;
        Node *right;
    };
    
    class SearchingAlgorithms
    {
        public:
            int binary_search(std::vector<int> &elements, int x, int a, int b);
            void BFS(Graph& graph, int startVertex);
            void preorder(Node *root);
            void inorder(Node *root);
            void postorder(Node *root);

            Node *createNode(int _data);
    };

} // namespace Searching

#include "SearchingAlgorithms.cpp"