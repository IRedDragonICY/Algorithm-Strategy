#include <windows.h>
#include <iostream>
#include <list>
#include <vector>
#include <iomanip>
#include "draws.h"
using namespace std;

struct TreeNode {
    int nodeIdx;
    float cost;
    vector<TreeNode*> children;
    TreeNode(int idx, float c) : nodeIdx(idx), cost(c) {}
};

Graph graph;
char markText[10];

vector<int> pathSequence;

float lowerBound;

float completeCost;

TreeNode* decisionTree = nullptr;

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

float reduceRows(vector<float> costMatrix[], int rowIdx, int numNodes)
{
    float minValue = (float)INT_MAX;
    for (int i = 0; i < numNodes; i++)
        if (costMatrix[rowIdx].at(i) < minValue)
            minValue = costMatrix[rowIdx].at(i);
    for (int i = 0; i < numNodes; i++)
    {
        if (costMatrix[rowIdx].at(i) >= (float)INT_MAX)
            continue;
        costMatrix[rowIdx].at(i) -= minValue;
    }
    return (minValue >= (float)INT_MAX ? 0.0f : minValue);
}

float reduceCols(vector<float> costMatrix[], int colIdx, int numNodes)
{
    float minValue = (float)INT_MAX;
    for (int i = 0; i < numNodes; i++)
        if (costMatrix[i].at(colIdx) < minValue)
            minValue = costMatrix[i].at(colIdx);
    for (int i = 0; i < numNodes; i++)
    {
        if (costMatrix[i].at(colIdx) >= (float)INT_MAX)
            continue;
        costMatrix[i].at(colIdx) -= minValue;
    }
    return (minValue >= (float)INT_MAX ? 0.0f : minValue);
}

void printCostMatrix(vector<float> costMatrix[], int numNodes)
{
    cout << "Cost Matrix:" << endl;
    for (int i = 0; i < numNodes; i++)
    {
        for (int j = 0; j < numNodes; j++)
        {
            if (costMatrix[i].at(j) >= (float)INT_MAX)
                cout << "INF\t";
            else
                cout << costMatrix[i].at(j) << "\t";
        }
        cout << endl;
    }
}

float reduceCostMatrix(vector<float> costMatrix[], int numNodes)
{
    float sums = 0.0f;
    cout << "Reducing rows:" << endl;
    for (int i = 0; i < numNodes; i++)
    {
        float rowReduction = reduceRows(costMatrix, i, numNodes);
        sums += rowReduction;
        cout << "Row " << i << " reduction: " << rowReduction << endl;
        printCostMatrix(costMatrix, numNodes);
    }
    cout << "Reducing columns:" << endl;
    for (int i = 0; i < numNodes; i++)
    {
        float colReduction = reduceCols(costMatrix, i, numNodes);
        sums += colReduction;
        cout << "Column " << i << " reduction: " << colReduction << endl;
        printCostMatrix(costMatrix, numNodes);
    }
    cout << "Total reduction: " << sums << endl;
    return sums;
}

void printTree(TreeNode* node, int level)
{
    if (!node) return;
    cout << setw(level * 4) << "" << "Node " << node->nodeIdx << " Cost: " << node->cost << endl;
    for (TreeNode* child : node->children)
    {
        printTree(child, level + 1);
    }
}

void BBRecursive(
    int rootIdx,
    int startIdx,
    int levelIdx,
    Graph graph,
    bool visited[],
    vector<int> path,
    vector<float> costMatrix[],
    float costRoot,
    TreeNode* currentNode
)
{
    int tLevelIdx = levelIdx;
    bool* tVisited = new bool[graph.getNumNodes()];
    vector<float>* tCostMatrix = new vector<float>[graph.getNumNodes()];
    for (int i = 0; i < graph.getNumNodes(); i++)
    {
        tVisited[i] = visited[i];
        for (int j = 0; j < graph.getNumNodes(); j++)
            tCostMatrix[i].push_back(costMatrix[i].at(j));
    }
    vector<int> tPathSequence;
    for (int i = 0; i < path.size(); i++)
        tPathSequence.push_back(path.at(i));
    
    tLevelIdx++;
    tVisited[startIdx] = true;
    tPathSequence.push_back(startIdx);
    
    for (int n = 0; n < graph.getAdjNodes()[startIdx].size(); n++)
    {
        int nodeIdx = graph.getAdjNodes()[startIdx].at(n);
        if (!tVisited[nodeIdx])
        {
            for (int i = 0; i < graph.getNumNodes(); i++)
            {
                tCostMatrix[startIdx].at(i) = (float)INT_MAX;
                tCostMatrix[i].at(nodeIdx) = (float)INT_MAX;
            }
            tCostMatrix[nodeIdx].at(0) = (float)INT_MAX;
            
            cout << "Current cost matrix before reduction:" << endl;
            printCostMatrix(tCostMatrix, graph.getNumNodes());
            
            float r = reduceCostMatrix(tCostMatrix, graph.getNumNodes());
            float edgeCost = costMatrix[startIdx].at(nodeIdx);
            float costNode = costRoot + edgeCost + r;
            
            cout << "Cost to node " << nodeIdx << ": " << costNode << " (Root cost: " << costRoot << " + Edge cost: " << edgeCost << " + Reduction: " << r << ")" << endl;

            TreeNode* childNode = new TreeNode(nodeIdx, costNode);
            currentNode->children.push_back(childNode);
            
            if (costNode <= lowerBound)
            {
                BBRecursive(rootIdx, nodeIdx, tLevelIdx, graph, tVisited,
                    tPathSequence, tCostMatrix, costNode, childNode);
                
                if (tLevelIdx == graph.getNumNodes() - 1)
                {
                    tPathSequence.push_back(nodeIdx);
                    tPathSequence.push_back(rootIdx);
                    float finalCost = 0.0f;
                    for (int i = 0; i < tPathSequence.size(); i++)
                        if (i >= 1)
                            finalCost += graph.getNodeDistance()[tPathSequence.at(i - 1)].at(tPathSequence.at(i));
                    
                    if (finalCost < completeCost)
                    {
                        lowerBound = costNode;
                        completeCost = finalCost;
                        pathSequence.clear();
                        for (int i = 0; i < tPathSequence.size(); i++)
                            pathSequence.push_back(tPathSequence.at(i));
                    }
                }
            }
            else
            {
                cout << "Pruning node " << nodeIdx << " with cost " << costNode << " as it exceeds the lower bound " << lowerBound << endl;
            }
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
            costMatrix[i].push_back(graph.getCostMatrix()[i].at(j));
    }
    
    float costRoot = reduceCostMatrix(costMatrix, graph.getNumNodes());

    decisionTree = new TreeNode(startIdx, costRoot);
    BBRecursive(startIdx, startIdx, 0, graph, visited, pathSequence, costMatrix, costRoot, decisionTree);
    
    float finalDistance = 0.0f;
    cout << "Final solution = ";
    for (int i = 0; i < pathSequence.size(); i++)
    {
        cout << pathSequence.at(i) << " ";
        if (i >= 1)
            finalDistance += graph.getNodeDistance()[pathSequence.at(i - 1)].at(pathSequence.at(i));
    }
    cout << " = " << finalDistance << endl;

    cout << "\nDecision Tree:" << endl;
    printTree(decisionTree, 0);

}

int main(int argc, char** argv)
{
    graph.setIsDirected(false);
    graph.setNumLevels(3);
    graph.setNumNodes(4);
    
    graph.addNode(0, 0, 1.0f);
    graph.addNode(3, 1, 1.0f);
    graph.addNode(1, 2, 1.0f);
    graph.addNode(2, 2, 1.0f);
    
    graph.addEdge(0, 1, 10.0f);
    graph.addEdge(0, 3, 20.0f);
    graph.addEdge(0, 2, 15.0f);
    graph.addEdge(1, 3, 25.0f);
    graph.addEdge(1, 2, 35.0f);
    graph.addEdge(3, 2, 30.0f);
    
    graph.setNodePosition();
    graph.setAdjStatus();
    
    BB(0, graph);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("NAMA / NIM - PRAKTIKUM STRATEGI ALGORITMA BRANCH AND BOUND");
    
    initView();
    
    glutDisplayFunc(displayGraph);
    glutReshapeFunc(reshapeView);
    
    glutMainLoop();
    return 0;
}
