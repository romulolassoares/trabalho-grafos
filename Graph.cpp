#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <string.h>
#include <vector>
#include <algorithm>

#define INFINITO 10000000

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
 **************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{
    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{
    Node *next_node = this->first_node;
    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{
    return this->order;
}

int Graph::getNumberEdges()
{
    return this->number_edges;
}

// Function that verifies if the graph is directed
bool Graph::getDirected() {
    return this->directed;
}

// Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge() {
    return this->weighted_edge;
}

// Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode() {
    return this->weighted_node;
}

Node *Graph::getFirstNode()
{
    return this->first_node;
}

Node *Graph::getLastNode()
{
    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    Node *next;
    Node *aux = nullptr;

    // Verifica se j?? existe algum n??
    if (this->getFirstNode() == nullptr) {
        this->first_node = new Node(id);
        this->last_node = this->getFirstNode();
    }
    else {
        if (!this->searchNode(id))
        {
            next = this->getFirstNode();
            // Procura o ??ltimo n?? inserido
            while (next != nullptr)
            {
                aux = next;
                next = next->getNextNode();
            }
            // Inseri o n?? na ??ltima posi????o
            aux->setNextNode(new Node(id));
            this->last_node = this->getNode(id);
        }
    }
}

void Graph::insertEdge(int id, int target_id, float weight) {
    // Procura se o n?? id existe. Se n??o existir insere ele no grafo
    if (!this->searchNode(id)) {
        this->insertNode(id);
        // cout << "Inserindo " << id << endl;
    }
    // Procura se o n?? target_id existe. Se n??o existir insere ele no grafo
    if (!this->searchNode(target_id)){
        this->insertNode(target_id);
        // cout << "Inserindo " << target_id << endl;
    }

    Node *nodeId = this->getNode(id);
    Node *nodeTargetId = this->getNode(target_id);

    if (this->getDirected()) {
        // Cria a aresta => id -> target_id
        nodeId->insertEdge(target_id, weight);

        // Aumenta os graus de sa??da e de entrada
        nodeId->incrementOutDegree();
        nodeTargetId->incrementInDegree();
    }
    else {
        // Cria a aresta => id - target_id
        if(!this->searchEdge(id, target_id)) {
            nodeId->insertEdge(target_id, weight);
            nodeTargetId->insertEdge(id, weight);
            // Aumenta os graus de sa??da e de entrada
            nodeId->incrementOutDegree();
            nodeTargetId->incrementOutDegree();
            nodeId->incrementInDegree();
            nodeTargetId->incrementInDegree();
        }
    }
    this->number_edges++;
}
bool Graph::searchEdge(int id, int target_id) {
    Node *node = this->getNode(id);
    Node *targetNode = this->getNode(target_id);
    Edge *edge = nullptr;

    edge = node->getFirstEdge();
    while(edge != nullptr) {
        if(edge->getTargetId() == target_id) {
            return true;
        }
        edge = edge->getNextEdge();
    }

    return false;
}

void Graph::removeNode(int id) {}

bool Graph::searchNode(int id)
{
    Node *node = this->getFirstNode();
    while (node != nullptr) {
        if (node->getId() == id)
            return true;
        node = node->getNextNode();
    }
    return false;
}


Node *Graph::getNode(int id)
{
    Node *node = this->getFirstNode();

    while (node != nullptr)
    {
        if (node->getId() == id)
            return node;
        node = node->getNextNode();
    }
    return nullptr;
}

// Function that prints a set of edges belongs breadth tree
//  void Graph::breadthFirstSearch(ofstream &output_file) {}
void Graph::depthFirstSearch(ofstream &output_file, int id)
{
    
    list<Edge> arvore, retorno;

    int *tempoDescobertaVertice = new int[this->getOrder()];
    int *tempoFinalDescobertaVertice = new int[this->getOrder()];

    int tempo = 0;

    int *pai = new int[this->getOrder()];

    for (int i = 0; i < this->getOrder(); i++)
    {
        tempoDescobertaVertice[i] = 0;
        tempoFinalDescobertaVertice[i] = 0;
        pai[i] = -1;
    }

    dfsRec(id, arvore, retorno, pai, tempo, tempoDescobertaVertice, tempoFinalDescobertaVertice);

    list<Edge>::iterator it;
    cout << "Arestas da ??rvore: " << endl;
    for (it = arvore.begin(); it != arvore.end(); it++)
    {
        cout << "[" << (*it).getOrigem() << "---" << (*it).getTargetId() << "]" << endl;
    }
    cout << "Arestas de Retorno: " << endl;
    for(it = retorno.begin(); it != retorno.end(); it++){
        cout << "[" << (*it).getOrigem() << "---" << (*it).getTargetId() << "]" << endl;
    }
    output_file.clear();
    output_file << "graph {" << endl;
    for (it = arvore.begin(); it != arvore.end(); it++)
    {
        output_file << "  " << (*it).getOrigem() << " -- " << (*it).getTargetId() << endl;
    }
    output_file << "}" << endl;

    // Deletando vetores
    delete[] tempoDescobertaVertice;
    delete[] tempoFinalDescobertaVertice;
    delete[] pai;
}

void Graph::dfsRec(int id, list<Edge> &arvore, list<Edge> &retorno, int *pai, int tempo, int *tempoDescoberta, int *tempoFinal)
{
    tempo++;
    tempoDescoberta[id] = tempo;
    Node *no = getNode(id);

    for (Edge *adj = no->getFirstEdge(); adj != nullptr; adj = adj->getNextEdge())
    {
        Edge aresta = Edge(adj->getTargetId());
        aresta.setOrigem(id);

        if (tempoDescoberta[adj->getTargetId()] == 0)
        {
            arvore.push_back(aresta);

            pai[adj->getTargetId()] = id;

            dfsRec(adj->getTargetId(), arvore, retorno, pai, tempo, tempoDescoberta, tempoFinal);
        }
        else
        {
            if ((tempoFinal[adj->getTargetId()] == 0) && (pai[id] != adj->getTargetId())){
                retorno.push_back(aresta);
            }
        }
    }
    tempoFinal[id] = tempo;
}

// float Graph::floydMarshall(int idSource, int idTarget) {}

float Graph::dijkstra(int idSource, int idTarget)
{
    if(!this->getDirected()){
        float *distancia = new float[this->order];
        float infinito = numeric_limits<float>::max();
        int *mapeamento = new int[this->order];
        int *aPercorrer = new int[this->order];
        int *noAnterior = new int[this->order];

        for (int i = 0; i < this->order; i++)
        {
            mapeamento[i] = i;
            if (i == idSource)
            {
                distancia[i] = 0;
                aPercorrer[i] = 0;
            }
            else
            {
                distancia[i] = infinito;
                aPercorrer[i] = 1;
            }
            noAnterior[i] = -1;
        }
        auxDijkstra(distancia, aPercorrer, noAnterior, mapeamento, idSource);
        return distancia[idTarget];
    }
    else{
        cout << "ERROR!!!" << endl;
        return 0;
    }
}

void Graph::auxDijkstra(float *distancia, int *aPercorrer, int *noAnterior, int *map, int atual)
{
    int indiceAtual = mapeamento(map, atual);
    int indiceAresta;
    Edge *adj;
    Node *aux = this->getFirstNode();

    //enquanto a aresta n??o ?? nula preenche os vetores de distancia e noAnterior
    while (aux != nullptr)
    {
        if(atual == aux->getId())
        {
            while (aux != nullptr)
            {
                adj = aux->getFirstEdge();
                while(adj != nullptr)
                {
                    indiceAresta = mapeamento(map, adj->getTargetId());
                    //caso o indicie atual da aresta n??o seja -1
                    if (distancia[indiceAresta] != -1)
                    {
                        if (distancia[indiceAresta] > distancia[indiceAtual] + adj->getWeight())
                        {
                            distancia[indiceAresta] = distancia[indiceAtual] + adj->getWeight();
                            noAnterior[indiceAresta] = atual;
                        }
                    }
                     else
                    {
                        distancia[indiceAresta] = distancia[indiceAtual] + adj->getWeight();
                        noAnterior[indiceAresta] = atual;
                    }
                    adj = adj->getNextEdge();
                }
                aux = nullptr;
            }
        }
        else{
            aux = aux->getNextNode();
            }
    }
    int menor = -1;

    for (int i = 0; i < this->getOrder() && menor == -1; i++)
    {
        if (aPercorrer[i])
        {
            if (distancia[i] != -1)
            {
                menor = distancia[i];
                atual = map[i];
            }
        }
    }
    if (menor != -1)
    {
        for (int i = 0; i < this->getOrder(); i++)
        {
            if (aPercorrer[i])
                if (distancia[i] != -1)
                    if (distancia[i] < menor)
                    {
                        menor = distancia[i];
                        atual = map[i];
                    }
        }
        aPercorrer[indiceAtual] = 0;
        auxDijkstra(distancia, aPercorrer, noAnterior, map, atual);
    }
}

int Graph::mapeamento(int *map, int id)
{
    for (int i = 0; i < this->getOrder(); i++)
    {
        if (map[i] == id)
            return i;
    }
    return -1;
}

// function that prints a topological sorting
//  void topologicalSorting() {}

// void breadthFirstSearch(ofstream& output_file) {}

void Graph::cleanVisited()
{
    //Ponteiro para percorrer entre os nos
    Node *n = this->getFirstNode();

    while (n != nullptr)
    {
        //Seta o no como nao visitado
        n->setVisited(false);
        //Ponteiro passa a apontar para o proximo no do grafo.
        n = n->getNextNode();
    }
}

Graph* Graph::getVerticeInduzido() {
    int qtd_vertices,vertice;
    vector<int> ids_subgrafo;
    ids_subgrafo.clear();
    cout << "Digite a quantidade de vertices do subgrafo: "<< endl;
    cin >> qtd_vertices;
    while(qtd_vertices > this->getOrder()) //loop para digitar ate um valor de ordem valida para o subgrafo induzido
    {
        cout << "O subgrafo nao pode ser maior que o grafo. Digite um numero valido para vertices do subgrafo: ";
        cin >> qtd_vertices;
        cout <<endl;
    }
    int i=0;
    while(i<qtd_vertices) {
        //cout <<i;
        cout << "Digite o " << i+1 << "o vertice do subgrafo:";
        cin >> vertice;
        if(this->searchNode(vertice)) {
            ids_subgrafo.push_back(vertice);
        }
        else {
            while(!this->searchNode(vertice)){ //caso nao exista o vertice no grafo original
                cout << "Vertice invalido. Digite apenas vertices presentes no grafo!" << endl;
                cout << "Digite o " << i+1 << "o vertice do subgrafo:";
                cin >> vertice;
            }
        }
        i++;
    }
    //subgrafo induzido
    Graph *subgrafoVInduzido = new Graph(ids_subgrafo.size(), this->getDirected(), this->getWeightedEdge(), this->getWeightedNode());
    //adiciona arestas no subgrafo
    this->cleanVisited(); //seta tudo como nao visitado
    for(int i=0;i<ids_subgrafo.size();i++) {
        for (int j = i + 1; j < ids_subgrafo.size(); j++)
            if ((!this->getNode(ids_subgrafo[j])->getVisited() && this->getNode(ids_subgrafo[i])->searchEdge(ids_subgrafo[j]))) { //verifica se tem a aresta no grafo original
                Edge *aresta_aux = this->getNode(ids_subgrafo[i])->getEdge(
                        ids_subgrafo[j]);
                subgrafoVInduzido->insertEdge(ids_subgrafo[i], ids_subgrafo[j], aresta_aux->getWeight());
            }

        this->getNode(ids_subgrafo[i])->setVisited(true); //altera para visitado
    }
    cout << "Grafo construido!" <<endl;
    cout << "Num de vertices:" <<subgrafoVInduzido->getOrder() << endl;
    cout << "Num de arestas: " << subgrafoVInduzido->getNumberEdges()<<endl;

    return subgrafoVInduzido;
}

void Graph::imprimirAgmByKruskal(ofstream &outputfile, int ordem,int numero_arestas, Aresta_aux arestas_finais[])
{
    cout << "---ALGORITMO DE KRUSKAL---" << endl;
    int somatorio_pesos = 0;
    cout<< "---ESTRUTURA APRESENTADA---:"<<endl;
    cout << "(NO ORIGEM -- N0 DESTINO) - PESO do NO" << endl;
    cout << "-------------------------------" << endl;

    for (int i = 0; i < numero_arestas; i++)
    {
        cout << "(" << arestas_finais[i].origem << " -- " << arestas_finais[i].destino << ") - " << arestas_finais[i].peso << endl;
        somatorio_pesos += arestas_finais[i].peso;
    }
    cout << "ORDEM DO SUBGRAFO:" <<" "<< ordem << endl;
    cout << "NUMERO de ARESTAS: " << numero_arestas << endl;
    cout << "SOMATORIO FINAL DOS PESOS: " << somatorio_pesos << endl;

    cout << "-------------------------------" << endl;
    //saida dot
   outputfile << "graph {" << endl;
   for (int i = 0; i < numero_arestas; i++)
   {
       outputfile << "  " << arestas_finais[i].origem << " -- " << arestas_finais[i].destino;
       outputfile << " [label = " << arestas_finais[i].peso << "]" << endl;
   }
   outputfile << "}" << endl;
}
//verifica se a aresta ja ta na lista
bool Graph::arestaNaLista( Aresta_aux listEdges[], int id, int destino, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (listEdges[i].origem == destino && listEdges[i].destino == id)
            return true;
    }
    return false;
}

// trocar 2 elementos
void trocar(Aresta_aux *a1,  Aresta_aux *a2)
{
    Aresta_aux troca = *a1;
    *a1 = *a2;
    *a2 = troca;
}

//ordenar os elementos a partir de um pivo
int particao(Aresta_aux vetor[], int menor, int maior)
{
    // pivo
    int elemento_pivo = vetor[maior].peso;

    int i = (menor - 1);

    for (int j = menor; j <= maior - 1; j++)
    {
        if (vetor[j].peso <= elemento_pivo)
        {
            i++;
            trocar(&vetor[i], &vetor[j]);
        }
    }
    trocar(&vetor[i + 1], &vetor[maior]);
    return (i + 1);
}
//ordenar vetor de arestas
void quickSort(Aresta_aux vetor[], int menor, int maior)
{
    if (menor < maior)
    {
        int indice = particao(vetor, menor, maior); //indice particionamento
        quickSort(vetor, menor, indice - 1);
        quickSort(vetor, indice + 1, maior);
    }
}
void Graph::agmByKruskal( ofstream &outputFile,Graph *grafo)
{
    if(grafo->getNumberEdges() == 0){ //caso nao haja nenhuma aresta formada no subgrafo
        cout<<"Este subgrafo nao contem arestas!"<<endl;
        return;
    }
    Node *p = grafo->getFirstNode(); //ponteiro para os nos
    Edge *aresta = p->getFirstEdge(); //ponteiro para as arestas
    int numArestas_grafo = grafo->getNumberEdges(); //numero de arestas do subgrafo
    Aresta_aux *lista_arestas = new Aresta_aux[numArestas_grafo]; //lista com as arestas

    int i=0; //contador
    while(p!= nullptr){ //percorre o subgrafo induzido preenchendo peso,origem e destino da aresta
        aresta=p->getFirstEdge();
        while (aresta!= nullptr){
            if(!arestaNaLista(lista_arestas,p->getId(),aresta->getTargetId(),i)){
                lista_arestas[i].origem=p->getId();
                lista_arestas[i].destino=aresta->getTargetId();
                lista_arestas[i].peso=aresta->getWeight();
                i++;
            }
            aresta=aresta->getNextEdge();
        }
        p=p->getNextNode();
    }

    quickSort(lista_arestas, 0, numArestas_grafo - 1); //ordenando vetor de arestas

    int numero_arestas = 0; //contador da qtd de arestas na solucao
    int atual = 0; //indice para solucao
    int vArestas[numArestas_grafo]; //vetor cujo tamanho eh a a qtd de arestas

    Aresta_aux *arestas_finais = new Aresta_aux[numArestas_grafo]; //vetor com as arestas da solucao

    for (int i = 0; i < numArestas_grafo; i++) //inicializa o vetor de 0 ao numero de arestas
    {
        vArestas[i] = i;
    }

    for (int i = 0; i < numArestas_grafo; i++) //percorre as arestas adicionando no vetor na solucao
    {
        int x = lista_arestas[i].origem;
        int y = lista_arestas[i].destino;
        if (vArestas[x] != vArestas[y])
        {
            arestas_finais[atual] = lista_arestas[i];
            atual++;
            numero_arestas++;
            int id_antigo = vArestas[x]; //vertice antigo recebe origem
            int id_novo = vArestas[y]; //vertice novo recebe destino
            for (int j = 0; j < numArestas_grafo; j++) //atualiza o vetor para indices do id destino(novo)
            {
                if (vArestas[j] == id_antigo)
                {
                    vArestas[j] = id_novo;
                }
            }
        }
    }
    imprimirAgmByKruskal(outputFile,grafo->getOrder(), numero_arestas, arestas_finais);

    //desalocando
    delete[] arestas_finais;
    delete[] lista_arestas;
}


list<int> Graph::getFechoTransitivoIndireto(list<int> &fechoIndireto, int *id) {
    if (!this->getDirected()) {
        cout << "O grafo nao eh direcionado!" << endl;
        return fechoIndireto;
    } else {

        //caso no vertice nao exista no grafo.
        while (!searchNode(*id)) {
            cout << "O vertice n??o existe no grafo.Por favor digite outro:";
            cin >> *id;
        }

        fechoIndireto.push_back(*id); //adiciona o vertice na lista de fecho

        //loop para lista de nos
        for (Node *auxiliar = this->getFirstNode(); auxiliar != nullptr; auxiliar = auxiliar->getNextNode()) {

            int nodeId = auxiliar->getId();

            //loop para  lista de adjacencia procurando  arestas em que o vertice eh destino
            for (Edge *adjacencia = auxiliar->getFirstEdge();
                 adjacencia != nullptr; adjacencia = adjacencia->getNextEdge()) {
                int idAlvo = adjacencia->getTargetId();

                //Verifica se o vertice eh destino

                if ((idAlvo == *id) &&
                    (find(fechoIndireto.begin(), fechoIndireto.end(), nodeId) == fechoIndireto.end())) {
                    getFechoTransitivoIndireto(fechoIndireto, &nodeId);
                    break;
                }
            }
        }
    }
    //retorna a lista com os vertices no fecho.
    return fechoIndireto;
}

void Graph::imprimirFechoTransitivoIndireto(ofstream &output_file, int id) {
    list<int> fechoIndireto;

    getFechoTransitivoIndireto(fechoIndireto, &id);
    list<int>::iterator x;

    cout << "|||| FECHO TRANSITIVO INDIRETO DO VERTICE " << id << ": ";


    for (x = fechoIndireto.begin(); x != fechoIndireto.end(); x++) {
        cout << *x << " ";
    }
    cout << "||||"<<endl;

}

void Graph::imprimirFechoTransitivoDireto(ofstream &output_file, int id) {

    list<int> fechoDireto; //cria lista para os fechos
    //chama a funcao para obter os fechos diretos de um id X
    fechoDireto = getFechoTransitivoDireto(fechoDireto, &id);

    list<int>::iterator x;
    cout << "|||| FECHO TRANSITIVO DIRETO DO VERTICE " << id << ": ";

    //Imprime o fecho transitivo na tela
    for (x = fechoDireto.begin(); x != fechoDireto.end(); x++) {
        cout << *x << " ";
    }
    cout << "||||"<<endl;
}

list<int> Graph::getFechoTransitivoDireto(list<int> &fechoDireto, int *id) {

    if (!this->getDirected()) { //caso o grafo nao seja direcionado
        cout << "O grafo nao eh direcionado!" << endl;
        return fechoDireto;
    } else {

        //caso no vertice nao exista no grafo.
        while (!searchNode(*id)) {
            cout << "O vertice nao existe no grafo.Por favor digite outro:" << endl;
            cin >> *id;
        }

        Node *node = getNode(*id);
        //Percorrer os nos adjacentes
        fechoDireto.push_back(*id); //insercao do vertice na lista.
        for (Edge *adjacencia = node->getFirstEdge(); adjacencia != nullptr; adjacencia = adjacencia->getNextEdge()) {

            int idAlvo = adjacencia->getTargetId();

            //Verifica se o no esta na lista do fecho.
            if (find(fechoDireto.begin(), fechoDireto.end(), idAlvo) == fechoDireto.end()) {
                //se tiver chama a funcao recursivamente passando ele.
                getFechoTransitivoDireto(fechoDireto, &idAlvo);
            }
        }
        //retorna a lista com os vertices no fecho.
        return fechoDireto;
    }
}

float Graph::localClusteringCoefficient(int idNode) {
    Node *node = this->getNode(idNode);
    Node *node2 = nullptr;
    Edge *edge = nullptr;
    Edge *edge2 = nullptr;
    float ccValue = 0;
    float dv = 0;
    float P = 0;

    if(node != nullptr) {
        dv = node->getInDegree();
        edge = node->getFirstEdge();
        float idEdge;
        while(edge != nullptr) {
            idEdge = edge->getTargetId();
            node2 = this->getNode(idEdge);
            // Acessa a lista de adj da lista de adj de idNode
            if (node2 != nullptr) {
                edge2 = node2->getFirstEdge();
                while(edge2 != nullptr) {
                    P += this->countNodeInAdjList(edge2->getTargetId(), idNode);
                    // cout << P << endl;
                    edge2 = edge2->getNextEdge();
                }
            }
            edge = edge->getNextEdge();
        }
    }
    if(dv != 1) {
        dv=dv*(dv-1);
    }
    ccValue = float(P/dv);
    // cout << P << "/" << dv << endl;
    return ccValue;
}

// Fun????o para contar quantas vezes um n?? aparece na lista de adj de um determinado n??
int Graph::countNodeInAdjList(int idNode, int idToFind) {
    Node *node = this->getNode(idNode);
    Edge *edge = nullptr;
    int value = 0;

    if (node != nullptr) {
        edge = node->getFirstEdge();
        while (edge != nullptr) {
            if (edge->getTargetId() == idToFind)
                value++;
            edge = edge->getNextEdge();
        }
    }
    return value;
}

float Graph::averageClusteringCoefficient() {
    Node *node = this->getFirstNode();
    float ccValueTotal = 0;
    int id;
    float result;

    while(node != nullptr) {
        id = node->getId();
        ccValueTotal += this->localClusteringCoefficient(id);
        node = node->getNextNode();
    }

    result = ccValueTotal/this->getOrder();

    return result;
}

void Graph::printGraph() {
    Node *node = this->getFirstNode();
    Edge *edge = nullptr;

    cout << "Lista de adjac??ncia" << endl;

    if (this->getDirected()) {
        if (this->getWeightedEdge()) {
            while (node != nullptr) {
                edge = node->getFirstEdge();
                cout << node->getId() << " -> ";

                while (edge != nullptr) {
                    cout << edge->getTargetId() << " -(" << edge->getWeight() << ")-> ";
                    edge = edge->getNextEdge();
                }

                cout << "null" << endl;
                node = node->getNextNode();
            }
        }
        else {
            while (node != nullptr) {
                edge = node->getFirstEdge();
                cout << node->getId() << " -> ";

                while (edge != nullptr) {
                    cout << edge->getTargetId() << " -> ";
                    edge = edge->getNextEdge();
                }

                cout << "null" << endl;
                node = node->getNextNode();
            }
        }
    }
    else
    {
        if (this->getWeightedEdge())
        {
            while (node != nullptr)
            {
                edge = node->getFirstEdge();
                cout << node->getId() << " - ";

                while (edge != nullptr)
                {
                    cout << edge->getTargetId() << " -(" << edge->getWeight() << ")- ";
                    edge = edge->getNextEdge();
                }

                cout << "null" << endl;
                node = node->getNextNode();
            }
        }
        else
        {
            while (node != nullptr)
            {
                edge = node->getFirstEdge();
                cout << node->getId() << " -> ";

                while (edge != nullptr)
                {
                    cout << edge->getTargetId() << " - ";
                    edge = edge->getNextEdge();
                }

                cout << "null" << endl;
                node = node->getNextNode();
            }
        }
    }
}

void Graph::printGraphDot(ofstream &file) {
    if (file.is_open()) {
        cout << "Salvando o grafo" << endl;

        Node *node = this->getFirstNode();
        Edge *edge;

        // Verifica se ?? ou n??o direcionado
        if (this->getDirected()) {
            file << "digraph { \n";
        }
        else {
            file << "strict graph { \n";
        }

        // Verifica se o n?? tem peso
        if (this->getWeightedNode()) {
            while (node != nullptr)
            {
                file << "   " << node->getId() << " [weight = ";
                file << node->getWeight() << "] \n";
                node = node->getNextNode();
            }
        }

        node = this->getFirstNode();

        while (node != nullptr) {
            edge = node->getFirstEdge();
            while (edge != nullptr)
            {
                file << "   " << node->getId();
                if (this->getDirected()) {
                    file << "->";
                }
                else {
                    file << "--";
                }
                file << edge->getTargetId();

                if (this->getWeightedEdge()) {
                    file << " [label=" << edge->getWeight();
                    file << ",weight=" << edge->getWeight() << "]";
                }
                file << "\n";
                edge = edge->getNextEdge();
            }
            node = node->getNextNode();
        }

        file << "} \n";
    }
    else {
        cout << "Falha ao abrir o arquivo";
    }
}













