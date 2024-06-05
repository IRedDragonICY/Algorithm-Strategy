#include <windows.h>
#include <iostream>
#include <list>
#include <vector>
#include "draws.h"

using namespace std;

Graph graph;
char markText[10];
vector<int> pathSequence;
float lowerBound;
float completeCost;

void drawResult()
{
    glPushMatrix();
    float radius = 15.0f;
    for (int i = 1; i < pathSequence.size(); i++)
    {
        int sourceIdx = pathSequence.at(i - 1);
        int targetIdx = pathSequence.at(i);
        drawLine(
            graph.getNodePosition(),
            sourceIdx,
            targetIdx,
            Vec3(1.0f, 0.0f, 1.0f),
            radius, 3.0f, true
        );
    }

    sprintf(markText, "%s", "start");
    Vec3 position(
        graph.getNodePosition()[pathSequence.at(0)].getX() + 2.0f * radius,
        graph.getNodePosition()[pathSequence.at(0)].getY() + 2.0f * radius,
        0.0f
    );
    drawText(position, Vec3(1.0f, 0.0f, 1.0f), markText, radius, 2.0f);
    glPopMatrix();
}

void displayGraph()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    drawNodes();
    drawResult();
    drawEdges();
    drawDistances();
    glutSwapBuffers();
}

void printCostMatrix(vector<float> costMatrix[], int numNodes)
{
    for (int i = 0; i < numNodes; i++)
    {
        for (int j = 0; j < numNodes; j++)
        {
            if (costMatrix[i][j] == (float)INT_MAX)
                cout << "INF ";
            else
                cout << costMatrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

float reduceRows(vector<float> costMatrix[], int rowIdx, int numNodes)
{
    float minValue = (float)INT_MAX;
    for (int i = 0; i < numNodes; i++)
        if (costMatrix[rowIdx][i] < minValue)
            minValue = costMatrix[rowIdx][i];

    for (int i = 0; i < numNodes; i++)
    {
        if (costMatrix[rowIdx][i] >= (float)INT_MAX)
            continue;
        costMatrix[rowIdx][i] -= minValue;
    }

    cout << "Baris " << rowIdx << " direduksi sebesar " << minValue << endl;
    return (minValue >= (float)INT_MAX ? 0.0f : minValue);
}

float reduceCols(vector<float> costMatrix[], int colIdx, int numNodes)
{
    float minValue = (float)INT_MAX;
    for (int i = 0; i < numNodes; i++)
        if (costMatrix[i][colIdx] < minValue)
            minValue = costMatrix[i][colIdx];

    for (int i = 0; i < numNodes; i++)
    {
        if (costMatrix[i][colIdx] >= (float)INT_MAX)
            continue;
        costMatrix[i][colIdx] -= minValue;
    }

    cout << "Kolom " << colIdx << " direduksi sebesar " << minValue << endl;
    return (minValue >= (float)INT_MAX ? 0.0f : minValue);
}

float reduceCostMatrix(vector<float> costMatrix[], int numNodes)
{
    float sums = 0.0f;
    for (int i = 0; i < numNodes; i++)
        sums += reduceRows(costMatrix, i, numNodes);
    for (int i = 0; i < numNodes; i++)
        sums += reduceCols(costMatrix, i, numNodes);
    cout << "Matriks biaya direduksi sebesar " << sums << endl;
    cout << "Matriks biaya setelah reduksi:" << endl;
    printCostMatrix(costMatrix, numNodes);
    return sums;
}

void BBRecursive(
    int rootIdx,
    int startIdx,
    int levelIdx,
    Graph graph,
    bool visited[],
    vector<int> path,
    vector<float> costMatrix[],
    float costRoot
)
{
    int tLevelIdx = levelIdx;
    bool* tVisited = new bool[graph.getNumNodes()];
    vector<float>* tCostMatrix = new vector<float>[graph.getNumNodes()];

    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        tVisited[i] = visited[i];
        for (int j = 0; j < graph.getNumNodes(); j++)
            tCostMatrix[i].push_back(costMatrix[i][j]);
    }

    vector<int> tPathSequence;
    for (int i = 0; i < path.size(); i++)
        tPathSequence.push_back(path[i]);

    tLevelIdx++;
    tVisited[startIdx] = true;
    tPathSequence.push_back(startIdx);

    for (int n = 0; n < graph.getAdjNodes()[startIdx].size(); n++)
    {
        int nodeIdx = graph.getAdjNodes()[startIdx][n];
        if (!tVisited[nodeIdx])
        {
            for (int i = 0; i < graph.getNumNodes(); i++)
            {
                tCostMatrix[startIdx][i] = (float)INT_MAX;
                tCostMatrix[i][nodeIdx] = (float)INT_MAX;
            }
            tCostMatrix[nodeIdx][0] = (float)INT_MAX;

            float r = reduceCostMatrix(tCostMatrix, graph.getNumNodes());
            float costNode = costRoot + costMatrix[startIdx][nodeIdx] + r;

            cout << "Mengeksplorasi node " << nodeIdx << " dengan biaya " << costRoot << " + " << costMatrix[startIdx][nodeIdx] << " + " << r << " = " << costNode << " dan batas bawah " << lowerBound << endl;

            if (costNode <= lowerBound)
            {
                BBRecursive(rootIdx, nodeIdx, tLevelIdx, graph, tVisited, tPathSequence, tCostMatrix, costNode);

                if (tLevelIdx == graph.getNumNodes() - 1)
                {
                    tPathSequence.push_back(nodeIdx);
                    tPathSequence.push_back(rootIdx);
                    float finalCost = 0.0f;
                    for (int i = 0; i < tPathSequence.size(); i++)
                        if (i >= 1)
                            finalCost += graph.getNodeDistance()[tPathSequence[i - 1]][tPathSequence[i]];

                    if (finalCost < completeCost)
                    {
                        lowerBound = costNode;
                        completeCost = finalCost;
                        pathSequence.clear();
                        for (int i = 0; i < tPathSequence.size(); i++)
                            pathSequence.push_back(tPathSequence[i]);
                    }
                }
            }
            else
            {
                cout << "Prunning node " << nodeIdx << " dengan biaya " << costNode << " melebihi batas bawah " << lowerBound << endl;
            }

            for (int i = 0; i < graph.getNumNodes(); i++)
                for (int j = 0; j < graph.getNumNodes(); j++)
                    tCostMatrix[i][j] = costMatrix[i][j];
        }
    }
    delete[] tVisited;
    delete[] tCostMatrix;
}

void BB(int startIdx, Graph graph)
{
    completeCost = (float)INT_MAX;
    lowerBound = (float)INT_MAX;
    pathSequence.clear();
    bool* visited = new bool[graph.getNumNodes()];
    vector<float>* costMatrix = new vector<float>[graph.getNumNodes()];

    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        visited[i] = false;
        for (int j = 0; j < graph.getNumNodes(); j++)
            costMatrix[i].push_back(graph.getCostMatrix()[i][j]);
    }

    cout << "Matriks biaya awal:" << endl;
    printCostMatrix(costMatrix, graph.getNumNodes());

    float costRoot = reduceCostMatrix(costMatrix, graph.getNumNodes());

    BBRecursive(startIdx, startIdx, 0, graph, visited, pathSequence, costMatrix, costRoot);

    float finalDistance = 0.0f;
    cout << "Solusi akhir = ";
    for (int i = 0; i < pathSequence.size(); i++)
    {
        cout << pathSequence[i] << " ";
        if (i >= 1)
            finalDistance += graph.getNodeDistance()[pathSequence[i - 1]][pathSequence[i]];
    }
    cout << " = " << finalDistance << endl;

    delete[] visited;
    delete[] costMatrix;
}

int main(int argc, char** argv)
{
	graph.setIsDirected(false);
	graph.setNumLevels(4);
	graph.setNumNodes(6);
	
	graph.addNode(0, 0, 1.0f);
	graph.addNode(1, 2, 1.0f);
	graph.addNode(2, 2, 1.0f);
	graph.addNode(3, 1, 1.0f);
	graph.addNode(4, 3, 1.0f);
	graph.addNode(5, 3, 1.0f);
	
	graph.addEdge(0, 1, 17.0f);
	graph.addEdge(0, 2, 13.0f);
	graph.addEdge(0, 3, 27.0f);
	graph.addEdge(1, 2, 21.0f);
	graph.addEdge(1, 3, 15.0f);
	graph.addEdge(1, 4, 46.0f);
	graph.addEdge(2, 3, 23.0f);
	graph.addEdge(2, 5, 17.0f);
	graph.addEdge(4, 5, 11.0f);
	
    graph.setNodePosition();
    graph.setAdjStatus();

    BB(0, graph);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    glutCreateWindow("Mohammad Farid Hendianto / 2200018401 - PRAKTIKUM STRATEGI ALGORITMA BRANCH AND BOUND");

    initView();

    glutDisplayFunc(displayGraph);
    glutReshapeFunc(reshapeView);

    glutMainLoop();
    return 0;
}
