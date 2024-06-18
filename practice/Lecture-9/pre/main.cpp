#include <windows.h>
#include <iostream>
#include <list>
#include <vector>
#include <limits.h>
#include <GL/glut.h>
#include "draws.h"
using namespace std;

// Global Graph
Graph graph;
char markText[10];
vector<int> pathSequence;
float finalCost;
int iterationCount = 0;  // Tambahkan variabel untuk menyimpan jumlah iterasi
float totalDistance = 0.0f;  // Tambahkan variabel untuk menyimpan total jarak

// Struktur untuk menyimpan informasi tabel iterasi
struct IterationInfo {
    int expandedNode;
    vector<int> liveNodes;
    float fValue;
};

vector<IterationInfo> iterationTable;

// Fungsi untuk menandai jalur hasil pencarian
void drawResult() {
    glPushMatrix();
    // Gambar edges
    float radius = 15.0f;
    for (int i = 1; i < pathSequence.size(); i++) {
        int sourceIdx = pathSequence.at(i-1);
        int targetIdx = pathSequence.at(i);
        drawLine(
            graph.getNodePosition(), sourceIdx, targetIdx,
            Vec3(1.0f, 0.0f, 1.0f), radius, 3.0f, true
        );
    }
    // Gambar teks
    sprintf(markText, "%s", "start");
    Vec3 position(
        graph.getNodePosition()[pathSequence.at(0)].getX() + 2.0f * radius,
        graph.getNodePosition()[pathSequence.at(0)].getY() + 2.0f * radius,
        0.0f
    );
    drawText(position, Vec3(1.0f, 0.0f, 1.0f), markText, radius, 2.0f);
    glPopMatrix();
}

// Taruh semua obyek yang akan digambar di fungsi display()
void displayGraph() {
    // Bersihkan dan reset layar dan buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Posisikan kamera pandang
    gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    // Panggil fungsi untuk menggambar obyek
    drawNodes();
    if (!pathSequence.empty())
        drawResult();
    drawEdges();
    drawDistances();
    // Tampilkan obyek ke layar
    glutSwapBuffers();
}

// Hitung jarak heuristik node asal ke node target misalnya menggunakan banyaknya busur minimal dari simpul asal ke target
float computeHeuristic(Graph graph, bool visited[], int startIdx, int endIdx) {
    // Inisialisasi
    bool* localVisited = new bool[graph.getNumNodes()];
    for (int i = 0; i < graph.getNumNodes(); i++)
        localVisited[i] = visited[i];
    // Jika simpul asal sudah terhubung ke simpul target
    if (graph.getAdjStatus()[startIdx].at(endIdx))
        return 1.0f;
    // Jika sudah sampai di simpul target
    if (startIdx == endIdx)
        return 0.0f;
    // Hitung banyaknya busur minimum antara simpul asal dan target
    float distance = 0.0f;
    for (int i = 0; i < graph.getAdjNodes()[startIdx].size(); i++) {
        int nodeIdx = graph.getAdjNodes()[startIdx].at(i);
        if (!localVisited[nodeIdx]) {
            localVisited[nodeIdx] = true;
            distance = 1.0f + computeHeuristic(graph, localVisited, nodeIdx, endIdx);
        }
    }
    return distance;
}

// Hitung cost perjalanan
float computeCost(Graph graph, int startIdx, int nodeIdx, int endIdx) {
    // Inisialisasi
    bool* visited = new bool[graph.getNumNodes()];
    for (int i = 0; i < graph.getNumNodes(); i++)
        visited[i] = false;
    // Hitung jarak g(n)
    float distance2root = 0.0f;
    if (pathSequence.size() > 1)
        for (int i = 1; i < pathSequence.size(); i++)
            distance2root += graph.getNodeDistance()[pathSequence.at(i-1)].at(pathSequence.at(i));
    float distance2travel = distance2root + graph.getNodeDistance()[startIdx].at(nodeIdx);
    // Hitung jarak h(n)
    float distance2target = computeHeuristic(graph, visited, nodeIdx, endIdx);
    // f(n) = g(n) + h(n)
    return distance2travel + distance2target;
}

// Fungsi rekursif dari branch and bound
void AStarRecursive(Graph graph, int startIdx, int endIdx, bool visited[], vector<int> &pathSequence) {
    // Tambahkan penghitungan iterasi
    iterationCount++;
    
    // Tandai node sekarang sebagai node yang dikunjungi
    visited[startIdx] = true;
    pathSequence.push_back(startIdx);
    // Base untuk berhenti
    if (startIdx == endIdx) return;
    
    // Cari cost minimum dari semua node yang bertetangga dengan node sekarang
    int minIdx;
    float minCost = (float)INT_MAX;
    vector<int> liveNodes;
    for (int i = 0; i < graph.getAdjNodes()[startIdx].size(); i++) {
        int nodeIdx = graph.getAdjNodes()[startIdx].at(i);
        if (!visited[nodeIdx]) {
            float cost = computeCost(graph, startIdx, nodeIdx, endIdx);
            liveNodes.push_back(nodeIdx);  // Simpan simpul hidup
            if (cost < minCost) {
                minCost = cost;
                minIdx = nodeIdx;
                finalCost = minCost;
            }
        }
    }
    // Tambahkan jarak ke totalDistance
    totalDistance += graph.getNodeDistance()[startIdx].at(minIdx);

    // Simpan informasi iterasi
    iterationTable.push_back({startIdx, liveNodes, finalCost});
    
    // Rekursi ke node dengan cost minimum
    AStarRecursive(graph, minIdx, endIdx, visited, pathSequence);
}

// Fungsi penerapan branch and bound
void AStar(Graph graph, int startIdx, int endIdx) {
    // Inisialisasi
    finalCost = 0.0f;
    iterationCount = 0;  // Reset iterasi
    totalDistance = 0.0f;  // Reset total jarak
    iterationTable.clear();  // Reset tabel iterasi
    bool* visited = new bool[graph.getNumNodes()];
    for (int i = 0; i < graph.getNumNodes(); i++)
        visited[i] = false;
    pathSequence.clear();
    // Terapkan rekursi dari BB
    AStarRecursive(graph, startIdx, endIdx, visited, pathSequence);
    // Cetak hasilnya
    cout << "Final solusi = ";
    for (int i = 0; i < pathSequence.size(); i++)
        cout << pathSequence.at(i) << " ";
    cout << "= " << finalCost << endl;
    // Cetak banyak iterasi dan total jarak
    cout << "Banyak iterasi = " << iterationCount << endl;
    cout << "Total jarak = " << totalDistance << endl;

    // Cetak tabel iterasi
    cout << "Tabel Iterasi:" << endl;
    cout << "Iterasi | Simpul Diekspan | Simpul Hidup | f(n) = g(n) + h(n)" << endl;
    for (int i = 0; i < iterationTable.size(); i++) {
        cout << i+1 << " | " << iterationTable[i].expandedNode << " | ";
        for (int j = 0; j < iterationTable[i].liveNodes.size(); j++) {
            cout << iterationTable[i].liveNodes[j] << " ";
        }
        cout << "| " << iterationTable[i].fValue << endl;
    }
}

// Kode tester
int main(int argc, char** argv) {
    // Inisialisasi graf
    graph.setIsDirected(false);
    graph.setNumLevels(4);
    graph.setNumNodes(7);
    // Tambahkan node
    graph.addNode(0, 0, 1.0f);
    graph.addNode(1, 0, 1.0f);
    graph.addNode(2, 1, 1.0f);    
    graph.addNode(3, 1, 1.0f);
    graph.addNode(4, 2, 1.0f);
    graph.addNode(5, 3, 1.0f);
    graph.addNode(6, 3, 1.0f);
    // Tambahkan edge
    graph.addEdge(0, 1, 3.0f);
    graph.addEdge(0, 3, 5.0f);
    graph.addEdge(0, 2, 4.0f);
    graph.addEdge(2, 4, 4.0f);
    graph.addEdge(1, 3, 2.0f);
    graph.addEdge(3, 4, 2.0f);
    graph.addEdge(4, 5, 1.0f);
    graph.addEdge(4, 6, 5.0f);
    graph.addEdge(5, 6, 3.0f);
    // Estimate node position
    graph.setNodePosition();
    graph.setAdjStatus();
    // Penerapan algoritma A*
    int startIdx = 0;
    int endIdx = 5;
    AStar(graph, startIdx, endIdx);
    // Inisialisasi jendela OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    // Set ukuran jendela tampilan dalam piksel
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    // Posisi jendela dilayar komputer dalam piksel
    glutInitWindowPosition(100, 100);
    // Judul jendela (isi dengan NAMA / NIM - PRAKTIKUM STIMA)
    glutCreateWindow("NAMA / NIM - PRAKTIKUM STIMA");
    // Panggil fungsi init untuk inisialisasi awal
    initView();
    // Event handler untuk display
    glutDisplayFunc(displayGraph);
    glutReshapeFunc(reshapeView);
    // Looping
    glutMainLoop();
    return 0;
}
