// Praktikum 8: Branch and Bound
// Algoritma Branch and Bound untuk menyelesaikan Travelling Salesman Problem

#include <windows.h>
#include <iostream>
#include <list>
#include "draws.h"
using namespace std;

// global Graph
Graph graph;
char markText[10];

// hasil dari algoritma
vector<int> pathSequence;

// batas bawah dari cost untuk pruning tree
float lowerBound;

// cost tur lengkap
float completeCost;

// fungsi untuk menandai hasil
void drawResult()
{
	glPushMatrix();
	
	// gambar edges
	float radius = 15.0f;
	for (int i=1; i<pathSequence.size(); i++)
	{
		int sourceIdx = pathSequence.at(i-1);
		int targetIdx = pathSequence.at(i);
		drawLine(
			graph.getNodePosition(),
			sourceIdx,
			targetIdx,
			Vec3(1.0f,0.0f,1.0f),
			radius, 3.0f, true
		);
	}
	
	// gambar teks
	sprintf(markText,"%s","start");
	Vec3 position(
		graph.getNodePosition()[pathSequence.at(0)].getX()+2.0f*radius,
		graph.getNodePosition()[pathSequence.at(0)].getY()+2.0f*radius,
		0.0f
	);
	drawText(position, Vec3(1.0f,0.0f,1.0f), markText, radius, 2.0f);
	glPopMatrix();
}

// taruh semua obyek yang akan digambar di fungsi display()
void displayGraph()
{
	// bersihkan dan reset layar dan buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	// posisikan kamera pandang
	gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	// panggil fungsi untuk menggambar obyek
	drawNodes();
	drawResult();
	drawEdges();
	drawDistances();
	
	// tampilkan obyek ke layar
	glutSwapBuffers();
}

// reduce cost pada baris
float reduceRows(vector<float> costMatrix[], int rowIdx, int numNodes)
{
	float minValue = (float)INT_MAX;
	for (int i=0; i<numNodes; i++)
		if (costMatrix[rowIdx].at(i) < minValue)
			minValue = costMatrix[rowIdx].at(i);
	for (int i=0; i<numNodes; i++)
	{
		if (costMatrix[rowIdx].at(i) >= (float)INT_MAX)
			continue;
		costMatrix[rowIdx].at(i) -= minValue;
	}
	return (minValue >= (float)INT_MAX ? 0.0f : minValue);
}

// reduce cost pada kolom
float reduceCols(vector<float> costMatrix[], int colIdx, int numNodes)
{
	float minValue = (float)INT_MAX;
	for (int i=0; i<numNodes; i++)
		if (costMatrix[i].at(colIdx) < minValue)
			minValue = costMatrix[i].at(colIdx);
	for (int i=0; i<numNodes; i++)
	{
		if (costMatrix[i].at(colIdx) >= (float)INT_MAX)
			continue;
		costMatrix[i].at(colIdx) -= minValue;
	}
	return (minValue >= (float)INT_MAX ? 0.0f : minValue);
}

// reduce cost matrix
float reduceCostMatrix(vector<float> costMatrix[], int numNodes)
{
	float sums = 0.0f;
	for (int i=0; i<numNodes; i++)
		sums += reduceRows(costMatrix, i, numNodes);
	for (int i=0; i<numNodes; i++)
		sums += reduceCols(costMatrix, i, numNodes);
	return sums;
}

// fungsi rekursif dari branch and bound
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
	// inisialisasi
	int tLevelIdx = levelIdx;
	bool* tVisited = new bool[graph.getNumNodes()];
	vector<float> *tCostMatrix = new vector<float>[graph.getNumNodes()];
	for (int i=0; i<graph.getNumNodes(); i++)
	{
		tVisited[i] = visited[i];
		for (int j=0; j<graph.getNumNodes(); j++)
			tCostMatrix[i].push_back(costMatrix[i].at(j));
	}
	vector<int> tPathSequence;
	for (int i=0; i<path.size(); i++)
		tPathSequence.push_back(path.at(i));
	
	// update variabel
	tLevelIdx++;
	tVisited[startIdx] = true;
	tPathSequence.push_back(startIdx);
	
	// proses semua node yang bertetangga dengan node sekarang
	for (int n=0; n<graph.getAdjNodes()[startIdx].size(); n++)
	{
		int nodeIdx = graph.getAdjNodes()[startIdx].at(n);
		// apabila node belum dikunjungi
		if (!tVisited[nodeIdx])
		{
			// update cost matrix
			for (int i=0; i<graph.getNumNodes(); i++)
			{
				tCostMatrix[startIdx].at(i) = (float)INT_MAX;
				tCostMatrix[i].at(nodeIdx) = (float)INT_MAX;
			}
			tCostMatrix[nodeIdx].at(0) = (float)INT_MAX;
			
			// hitung reduce cost matrix di node cabang
			float r = reduceCostMatrix(tCostMatrix, graph.getNumNodes());
			float costNode = costRoot + costMatrix[startIdx].at(nodeIdx) + r;
			
			// rekursi node cabang apabila cost <= batas bawah cost
			if (costNode <= lowerBound)
			{
				// rekursi node cabang
				BBRecursive(rootIdx, nodeIdx, tLevelIdx, graph, tVisited,
				tPathSequence, tCostMatrix, costNode);
				
				// bila mencapai simpul daun
				if (tLevelIdx == graph.getNumNodes()-1)
				{
					// hitung cost tur lengkap
					tPathSequence.push_back(nodeIdx);
					tPathSequence.push_back(rootIdx);
					float finalCost = 0.0f;
					for (int i=0; i<tPathSequence.size();i++)
						if (i >= 1)
							finalCost +=
					graph.getNodeDistance()[tPathSequence.at(i-1)].at(tPathSequence.at(i));
					
					// apabila cost < cost tur lengkap sebelumnya
					if (finalCost < completeCost)
					{
						lowerBound = costNode;
						completeCost = finalCost;
						// update jalur tur lengkap
						pathSequence.clear();
						for (int i=0; i<tPathSequence.size(); i++)
							pathSequence.push_back(tPathSequence.at(i));
					}
				}
			}
			// reset cost matrix
			for (int i=0; i<graph.getNumNodes(); i++)
				for (int j=0; j<graph.getNumNodes(); j++)
					tCostMatrix[i].at(j) = costMatrix[i].at(j);
		}
	}
}

// fungsi penerapan branch and bound
void BB(int startIdx, Graph graph)
{
	// inisialisasi
	completeCost = (float)INT_MAX;
	lowerBound = (float)INT_MAX;
	pathSequence.clear();
	bool* visited = new bool[graph.getNumNodes()];
	vector<float>* costMatrix = new vector<float>[graph.getNumNodes()];
	for (int i=0; i<graph.getNumNodes(); i++)
	{
		visited[i] = false;
		for (int j=0; j<graph.getNumNodes(); j++)
			costMatrix[i].push_back(graph.getCostMatrix()[i].at(j));
	}
	
	// hitung reduce cost matrix di node akar
	float costRoot = reduceCostMatrix(costMatrix, graph.getNumNodes());
	
	// rekursi BB
	BBRecursive(startIdx, startIdx, 0, graph, visited, pathSequence, costMatrix, costRoot);
	
	// cetak hasil jalur tur lengkapnya
	float finalDistance = 0.0f;
	cout << "Final solusi = ";
	for (int i=0; i<pathSequence.size();i++)
	{
		cout << pathSequence.at(i) << " ";
		if (i >= 1)
			finalDistance += graph.getNodeDistance()[pathSequence.at(i-1)].at(pathSequence.at(i));
	}
	cout << " = " << finalDistance;
}

// kode tester
int main(int argc, char** argv)
{
	// inisialisasi graf
	graph.setIsDirected(false);
	graph.setNumLevels(2);
	graph.setNumNodes(4);
	
	// tambahkan node
	graph.addNode(0, 0, 1.2f);
	graph.addNode(1, 0, 1.0f);
	graph.addNode(2, 1, 1.0f);
	graph.addNode(3, 1, 1.0f);
	
	// tambahkan edge dengan jarak
	graph.addEdge(0, 1, 12.0f);
	graph.addEdge(0, 2, 10.0f);
	graph.addEdge(0, 3, 5.0f);
	graph.addEdge(1, 2, 9.0f);
	graph.addEdge(1, 3, 8.0f);
	graph.addEdge(2, 3, 15.0f);
	
	// estimate node position
	graph.setNodePosition();
	graph.setAdjStatus();
	
	// terapkan algoritma BB mulai dari indeks node-0
	BB(0, graph);
	
	// inisialisasi jendela OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	// set ukuran jendela tampilan dalam piksel
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// posisi jendela dilayar komputer dalam piksel
	glutInitWindowPosition(100, 100);
	
	// judul jendela (isi dengan NAMA / NIM - JUDUL PRAKTIKUM)
	glutCreateWindow("Mohammad Farid Hendianto / 2200018401 - PRAKTIKUM STRATEGI ALGORITMA BRANCH AND BOUND");
	
	// panggil fungsi init untuk inisialisasi awal
	initView();
	
	// event handler untuk display
	glutDisplayFunc(displayGraph);
	glutReshapeFunc(reshapeView);
	
	// looping
	glutMainLoop();
	return 0;
}
