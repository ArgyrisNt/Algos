#include <iostream>

namespace Searching
{

    int SearchingAlgorithms::binary_search(std::vector<int> &elements, int x, int a, int b)
    {
        if (b >= a)
        {
            int mid = a + (b - a) / 2;

            if (x == elements[mid])
            {
                std::cout << x << " is at position " << mid << std::endl;
                return mid;
            }
            else if (x < elements[mid])
            {
                b = mid - 1;
                return binary_search(elements, x, a, b);
            }
            else if (x > elements[mid])
            {
                a = mid + 1;
                return binary_search(elements, x, a, b);
            }
        }

        std::cout << x << " is not in vector" << std::endl;
        return -1;
    }

    void SearchingAlgorithms::BFS(Graph &graph, int startVertex)
    {
        std::queue<int> q;
        q.push(startVertex);
        graph.visited[startVertex] = true;

        while (!q.empty())
        {
            int firstElem = q.front();
            std::cout << firstElem << " ";
            q.pop();
            graph.visited[firstElem] = true;
            for (auto el : graph.adjLists[firstElem])
            {
                if (!graph.visited[el])
                {
                    q.push(el);
                    graph.visited[el] = true;
                }
            }
        }
    }

    Node *SearchingAlgorithms::createNode(int _data)
    {
        Node *newNode = new Node();
        newNode->data = _data;
        newNode->left = nullptr;
        newNode->right = nullptr;

        return newNode;
    }

    void SearchingAlgorithms::preorder(Node *root)
    {
        if (root == nullptr)
            return;

        std::cout << root->data << " ";
        preorder(root->left);
        preorder(root->right);
    }

    void SearchingAlgorithms::inorder(Node *root)
    {
        if (root == nullptr)
            return;

        inorder(root->left);
        std::cout << root->data << " ";
        inorder(root->right);
    }

    void SearchingAlgorithms::postorder(Node *root)
    {
        if (root == nullptr)
            return;

        postorder(root->left);
        postorder(root->right);
        std::cout << root->data << " ";
    }

} // namespace Searching
