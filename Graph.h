/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>

using namespace std;

class Graph {

    //Atributes
    private:
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node* first_node;
        Node* last_node;

    public:
        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        //Destructor
        ~Graph();
        //Getters
        int getOrder();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node* getFirstNode();
        Node* getLastNode();
        //Other methods
        void insertNode(int id);
        void insertEdge(int id, int target_id, float weight);
        void removeNode(int id);
        bool searchNode(int id);
        Node* getNode(int id);
        bool searchEdge(int id, int target_id);

        //methods phase1
        void topologicalSorting();
        void breadthFirstSearch(ofstream& output_file);
        void depthFirstSearch(ofstream& output_file, int id);
        Graph* getVertexInduced(int* listIdNodes);
        Graph* agmKuskal();
        Graph* agmPrim();
        float floydMarshall(int idSource, int idTarget);
        float dijkstra(int idSource, int idTarget);
        int localClusteringCoefficient(int idNode);
        // int localClusteringCoefficient(int idNode);

        //methods phase1
        float greed();
        float greedRandom();
        float greedRactiveRandom();

        //axiliar methods
        void printGraph();
        void printGraphDot(ofstream& file);
        int minimalDistance(bool visitados[], int distance[]);
        void dfsRec(int id, list<Edge> &arvore, list<Edge> &retorno, int *pai, int tempo, int *tempoDescoberta, int *tempoFinal);
        int mapeamento(int *map, int id);

    private:
        //Auxiliar methods
        bool searchListAdj(int idNode, int idToFind);
        int countNodeInAdjList(int idNode, int idToFind);
};

#endif // GRAPH_H_INCLUDED
