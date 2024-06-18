// Header untuk merancang graf 
// Praktikum Strategi Algoritma, Teknik Informatika, UAD
// Adhi Prahara. 2020.

#include <iostream>
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

using namespace std;

///////////////////////////////////////////////////////////
// Vec3 - class untuk tipe data titik 3D
///////////////////////////////////////////////////////////
class Vec3
{
private:
	float X, Y, Z;
public:
	// constructor
	Vec3();
	Vec3(float X, float Y, float Z);
	
	// get() - set()
	float getX();
	float getY();
	float getZ();
	void setPosition(float X, float Y, float Z);
};

// constructor
Vec3::Vec3() { }

Vec3::Vec3(float X, float Y, float Z) 
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
}

// ambil posisi sumbu X
float Vec3::getX() { return X; }
// ambil posisi sumbu Y
float Vec3::getY() { return Y; }
// ambil posisi sumbu Z
float Vec3::getZ() { return Z; }

// set posisi
void Vec3::setPosition(float X, float Y, float Z) 
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Graph - class untuk merancang model graph  
///////////////////////////////////////////////////////////
class Graph 
{ 
private:
	// tipe graf: directed atau undirected
	bool isDirected;
	
	// jumlah node
    int numNodes;    
    // jumlah level kedalaman
    int numLevels;
  
  	// array untuk menyimpan daftar node
  	vector<int>* nodeList;
  	
    // array untuk menyimpan daftar node yang bertetangga
    vector<int>* adjNodes;   
    
    // array untuk menyimpan status ketetanggan antar node
    vector<bool>* adjStatus; 
    
    // array untuk menyimpan jarak antar node
    vector<float>* nodeDistance;
    
    // array untuk menyimpan cost jarak antar node
    vector<float>* costMatrix;
    
    // array untuk menyimpan posisi node
    Vec3* nodePosition;

	// array untuk menyimpan skala posisi node
    float* nodeScale;    
public: 
	// constructor
	Graph(); 
	
	// numNodes = jumlah node
	// numLevels = jumlah level kedalaman
    Graph(int numNodes, int numLevels);  
    
    // fungsi untuk menambah node pada graf
    // nodeIdx = indeks node 
    // level = level kedalaman dari node
    void addNode(int nodeIdx, int level);  

	// fungsi untuk menambah node pada graf
    // nodeIdx = indeks node 
    // level = level kedalaman dari node
    // scale = skala jarak antar node
    void addNode(int nodeIdx, int level, float scale);  
	  
    // fungsi untuk menambah edge pada graf
    // sourceIdx = indeks node asal
    // targetIdx = indeks node tujuan
    void addEdge(int sourceIdx, int targetIdx);  
    
    // fungsi untuk menambah jarak disetiap edge
    // sourceIdx = indeks node asal
    // targetIdx = indeks node tujuan
    // distance = jarak antar node
    void addEdge(int sourceIdx, int targetIdx, float distance);
    
    // fungsi untuk mengisi dan mengambil variabel 
    void setIsDirected(bool isDirected);
    void setNumNodes(int numNodes);
    void setNumLevels(int numLevels);
    void setNodePosition();
    void setAdjStatus();
    
	bool getIsDirected();
    int getNumNodes();
    int getNumLevels();
    vector<int>* getListNodes();
    vector<int>* getAdjNodes();
    vector<bool>* getAdjStatus();
    vector<float>* getNodeDistance();
    vector<float>* getCostMatrix();
    Vec3* getNodePosition();
	float* getNodeScale();
}; 

// constructor 
Graph::Graph() { }

// numNodes = jumlah node
// numLevels = jumlah level kedalaman
Graph::Graph(int numNodes, int numLevels) 
{ 
    this->numNodes = numNodes; 
    this->numLevels = numLevels;
    nodeList = new vector<int>[numLevels];
    adjNodes = new vector<int>[numNodes];
    adjStatus = new vector<bool>[numNodes];
    nodePosition = new Vec3[numNodes];
    nodeScale = new float[numNodes];
    for (int i=0; i<numNodes; i++)
		nodeScale[i] = 1.0f;
    
    // inisialisasi jarak antar node
    nodeDistance = new vector<float>[numNodes];
    costMatrix = new vector<float>[numNodes];
    for (int i=0; i<numNodes; i++) 
	{
		for (int j=0; j<numNodes; j++) 
		{
			nodeDistance[i].push_back(0.0f);
			costMatrix[i].push_back((float)INT_MAX);
		}
	}
} 

// fungsi untuk menambah node pada graf
// nodeIdx = indeks node 
// level = level kedalaman dari node
void Graph::addNode(int nodeIdx, int level) 
{ 
    nodeList[level].push_back(nodeIdx);
} 

// fungsi untuk menambah node pada graf
// nodeIdx = indeks node 
// level = level kedalaman dari node
void Graph::addNode(int nodeIdx, int level, float scale) 
{ 
    nodeList[level].push_back(nodeIdx);
    nodeScale[nodeIdx] = scale;
} 

// fungsi untuk menambah edge pada graf
// sourceIdx = indeks node asal
// targetIdx = indeks node tujuan  
void Graph::addEdge(int sourceIdx, int targetIdx) 
{ 
    adjNodes[sourceIdx].push_back(targetIdx);
    if (!isDirected) 
    	adjNodes[targetIdx].push_back(sourceIdx);
} 

// fungsi untuk menambah jarak edge pada graf
// sourceIdx = indeks node asal
// targetIdx = indeks node tujuan 
// distance = jarak antar node 
void Graph::addEdge(int sourceIdx, int targetIdx, float distance) 
{ 
	adjNodes[sourceIdx].push_back(targetIdx);
    nodeDistance[sourceIdx].at(targetIdx) = distance;
    costMatrix[sourceIdx].at(targetIdx) = distance;
    if (!isDirected) 
    {
    	adjNodes[targetIdx].push_back(sourceIdx);
    	nodeDistance[targetIdx].at(sourceIdx) = distance;
    	costMatrix[targetIdx].at(sourceIdx) = distance;
    }
} 

// fungsi untuk mengatur tipe graf
void Graph::setIsDirected(bool isDirected) 
{
	this->isDirected = isDirected;	
}

// fungsi untuk mengatur jumlah node
void Graph::setNumNodes(int numNodes) 
{
	this->numNodes = numNodes; 
    adjNodes = new vector<int>[numNodes];
    adjStatus = new vector<bool>[numNodes];
    nodePosition = new Vec3[numNodes];
    nodeScale = new float[numNodes];
    for (int i=0; i<numNodes; i++)
		nodeScale[i]= 1.0f;
    
    // inisialisasi jarak antar node
    nodeDistance = new vector<float>[numNodes];
    costMatrix = new vector<float>[numNodes];
    for (int i=0; i<numNodes; i++) 
	{
		for (int j=0; j<numNodes; j++) 
		{
			nodeDistance[i].push_back(0.0f);
			costMatrix[i].push_back((float)INT_MAX);
		}
	}
}

// fungsi untuk mengatur level kedalaman node
void Graph::setNumLevels(int numLevels) 
{
	this->numLevels = numLevels; 
	nodeList = new vector<int>[numLevels];
}

bool Graph::getIsDirected() 
{
	return isDirected;
}

// fungsi untuk mengambil variabel jumlah node
int Graph::getNumNodes() 
{
    return numNodes;
}

// fungsi untuk mengambil variabel level kedalaman node
int Graph::getNumLevels() 
{
    return numLevels;
}

// fungsi untuk mengambil variabel daftar node
vector<int>* Graph::getListNodes() 
{
	return nodeList;
} 

// fungsi untuk mengambil variabel daftar node yang terhubung
vector<int>* Graph::getAdjNodes() 
{
	return adjNodes;
} 

// fungsi untuk mengambil variabel posisi node
Vec3* Graph::getNodePosition() 
{
	return nodePosition;
} 

// fungsi untuk mengambil status ketetanggaan antar node
vector<bool>* Graph::getAdjStatus()
{
	return adjStatus;
}

// fungsi untuk mengambil jarak antar node
vector<float>* Graph::getNodeDistance()
{
	return nodeDistance;
}

// fungsi untuk mengisi variabel posisi node
void Graph::setNodePosition() 
{	
	// tentukan posisi nodes sesuai kedalaman level
	float radius = 15;
	float halfHeight = SCREEN_HEIGHT / 2.0f;
	float distDepth = SCREEN_HEIGHT / (numLevels + 1);
	for (int i=0; i<numLevels; i++) 
	{
		vector<int> localNodes = nodeList[i];
    	float halfWidth = SCREEN_WIDTH / 2.0f;
    	float distBranch = SCREEN_WIDTH / (localNodes.size() + 1);
    	for (int j=0; j<localNodes.size(); j++) 
		{
			int nodeIdx = localNodes.at(j);
			nodePosition[nodeIdx] = Vec3(
				(j+1) * nodeScale[nodeIdx] * distBranch - halfWidth, 
				halfHeight - (i+1) * distDepth,
				0.0f);
    	}	
	}
} 

// fungsi status ketetanggan antar node
void Graph::setAdjStatus() 
{
	for (int i=0; i<numNodes; i++) 
	{
		for (int j=0; j<numNodes; j++)
			adjStatus[i].push_back(false);
		for (int j=0; j<adjNodes[i].size(); j++)
		{
			int nodeIdx = adjNodes[i].at(j);
			adjStatus[i].at(nodeIdx) = true;
		}
	}
}

// fungsi untuk menyusun matriks bobot jarak
vector<float>* Graph::getCostMatrix() 
{
	return costMatrix;
}

// global Graph
extern Graph graph;
