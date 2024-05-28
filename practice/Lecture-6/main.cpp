// Praktikum 6: BFS dan DFS
// Penerapan contoh di slide kuliah Strategi Algoritma - BFS dan DFS hal 14
// header
#include <windows.h>
#include <iostream>
#include <list>
// header untuk menggambar graf dengan OpenGL
#include "draws.h"


using namespace std;

// deklarasi global Graph
Graph graph;

// buffer untuk simpan teks
char markText[10];

// hasil dari penerapan algoritma
vector<int>* pathResult;
vector<int> pathSequence;

// fungsi untuk menggambar jalur hasil pencarian dengan OpenGL
void drawResult() {
    glPushMatrix();
    // gambar garis dengan anak panah
    float radius = 15.0f;
    for (int i = 0; i < graph.getNumNodes(); i++) {
        for (int j = 0; j < pathResult[i].size(); j++) {
            int nodeIdx = pathResult[i].at(j);
            Vec3 color(1.0f, 0.0f, 1.0f);
            drawLine(graph.getNodePosition(), i, nodeIdx, color, radius, 2.0f, graph.getIsDirected());
        }
    }
    // gambar urutan hasil pencarian
    for (int i = 0; i < pathSequence.size(); i++) {
        int nodeIdx = pathSequence.at(i);
        if (i == 0)
            sprintf(markText, "%s", "start");
        else
            sprintf(markText, "%d", i);
        Vec3 position(
            graph.getNodePosition()[nodeIdx].getX() + 2.0f * radius,
            graph.getNodePosition()[nodeIdx].getY() + 2.0f * radius,
            0.0f);
        Vec3 color(1.0f, 0.0f, 1.0f);
        drawText(position, color, markText, radius, 2.0f);
    }
    glPopMatrix();
}

// taruh semua obyek yang akan digambar di fungsi display()
void displayGraph() {
    // bersihkan dan reset layar dan buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // posisikan kamera pandang
    gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    // panggil fungsi untuk menggambar obyek
    drawNodes();
    if (!pathSequence.empty())
        drawResult();
    drawEdges();
    // tampilkan obyek ke layar
    glutSwapBuffers();
}

// fungsi untuk menerapkan Breadth First Search (BFS) pada graf
// graph = graf
// startIdx = indeks node mulai
void BFS(Graph graph, int startIdx) {
    // tandai semua node yang belum dikunjungi
    bool* visited = new bool[graph.getNumNodes()];
    for (int i = 0; i < graph.getNumNodes(); i++)
        visited[i] = false;

    // buat antrian node
    list<int> queue;

    // tandai node sekarang sebagai node yang dikunjungi
    visited[startIdx] = true;
    queue.push_back(startIdx);
    pathSequence.clear();
    pathResult = new vector<int>[graph.getNumNodes()];

    while (!queue.empty()) {
        // keluarkan node dari antrian dan cetak indeksnya
        startIdx = queue.front();
        cout << startIdx << " ";
        pathSequence.push_back(startIdx);
        queue.pop_front();

        // ambil semua node yang bertetangga dengan node sekarang
        // dari daftar antrian node
        for (int i = 0; i < graph.getAdjNodes()[startIdx].size(); i++) {
            // jika node tetangganya belum dikunjungi maka tandai
            // telah dikunjungi dan keluarkan dari antrian
            int nodeIdx = graph.getAdjNodes()[startIdx].at(i);
            if (!visited[nodeIdx]) {
                visited[nodeIdx] = true;
                queue.push_back(nodeIdx);
                pathResult[startIdx].push_back(nodeIdx);
            }
        }
        // jika antrian kosong tapi masih ada node yang belum dikunjungi
        // maka buat node tersebut sebagai titik awal lagi
        if (queue.empty()) {
            int j = 0;
            // cari node yang belum dikunjungi
            while (visited[j] && j < graph.getNumNodes())
                j++;
            // bila ada node yang belum dikunjungi maka masukan di antrian
            if (!visited[j] && j < graph.getNumNodes()) {
                visited[j] = true;
                queue.push_back(j);
            }
        }
    }
}

// fungsi rekursi untuk Depth First Search (DFS)
void DFSRecursive(Graph graph, int startIdx, int endIdx, bool visited[])
{
// tandai node sekarang sebagai node yang dikunjungi
visited[startIdx] = true;
cout << startIdx << " ";
pathSequence.push_back(startIdx);
// rekursi ke semua node yang bertetangga
for (int i=0; i<graph.getAdjNodes()[startIdx].size(); i++)
{
int nodeIdx = graph.getAdjNodes()[startIdx].at(i);
if (!visited[nodeIdx])
{
DFSRecursive(graph, nodeIdx, startIdx, visited);
pathResult[startIdx].push_back(nodeIdx);
}
}
// jika semua node cabang pada node awal sudah habis maka
// tentukan node baru yang belum dikunjungi sebagai node awal
if (startIdx == endIdx)
{
int j=0;
// cari node yang belum dikunjungi
while (visited[j] && j<graph.getNumNodes())
j++;
// bila ada node yang belum dikunjungi maka masukan dalam antrian
if (!visited[j] && j<graph.getNumNodes())
DFSRecursive(graph, j, j, visited);
}
}
// fungsi untuk menerapkan Depth First Search (DFS) pada graf
// graph = graf
// startIdx = indeks node mulai
void DFS(Graph graph, int startIdx)
{
// tandai semua node yang belum dikunjungi
bool* visited = new bool[graph.getNumNodes()];
for(int i = 0; i < graph.getNumNodes(); i++)
visited[i] = false;
pathSequence.clear();
pathResult = new vector<int>[graph.getNumNodes()];
// rekursi DFS
DFSRecursive(graph, startIdx, startIdx, visited);
}



// kode tester
int main(int argc, char** argv) {
//    // inisialisasi graf
//    graph.setIsDirected(true);
//    graph.setNumLevels(4);
//    graph.setNumNodes(7);
//
//    // tambahkan nodes
//    graph.addNode(0, 0, 1.1f);
//    graph.addNode(1, 0, 0.95f);
//    graph.addNode(2, 1, 0.9f);
//    graph.addNode(3, 1, 1.05f);
//    graph.addNode(4, 3, 1.1f);
//    graph.addNode(5, 2, 0.85f);
//    graph.addNode(6, 3, 0.95f);
//
//    // tambahkan edges
//    graph.addEdge(0, 1);
//    graph.addEdge(0, 3);
//    graph.addEdge(2, 0);
//    graph.addEdge(2, 3);
//    graph.addEdge(2, 4);
//    graph.addEdge(2, 6);
//    graph.addEdge(3, 1);
//    graph.addEdge(4, 5);
//    graph.addEdge(4, 6);
//    graph.addEdge(5, 2);
//    graph.addEdge(6, 3);

	// Post Test
    // inisialisasi graf
    graph.setIsDirected(true);
    graph.setNumLevels(4); // 3 distinct levels 
    graph.setNumNodes(6); 

    // tambahkan nodes 

    graph.addNode(1, 0, 1.0f);  // Top Left (level 2)
    graph.addNode(2, 0, 1.0f);  // Top Right (level 2)
    graph.addNode(0, 1, 1.0f);  // Center Node (level 1)
    graph.addNode(3, 2, 1.0f);  // Bottom Center (Level 0)
    graph.addNode(4, 3, 1.0f);  // Bottom Left (level 0)
    graph.addNode(5, 3, 1.0f);  // Bottom Right (level 0)

    // tambahkan edges
    graph.addEdge(2, 1); 
    graph.addEdge(1, 4);
    graph.addEdge(4, 5);
    graph.addEdge(5, 2); 
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(3, 4);
    graph.addEdge(3, 5);
	


    graph.setNodePosition();

    // terapkan BFS
    int startIdx = 0;
    cout << "BFS mulai dari node " << startIdx << "\n";
    BFS(graph, startIdx);
//    cout << "DFS mulai dari node " << startIdx << "\n";
//    DFS(graph, startIdx);


    // inisialisasi jendela OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // set ukuran jendela tampilan dalam piksel
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    // posisi jendela dilayar komputer dalam piksel
    glutInitWindowPosition(100, 100);

    // judul jendela (isi dengan NAMA / NIM - JUDUL PRAKTIKUM)
    glutCreateWindow("NAMA / NIM - PRAKTIKUM STIMA");

    // panggil fungsi init untuk inisialisasi awal
    initView();

    // event handler untuk display
    glutDisplayFunc(displayGraph);
    glutReshapeFunc(reshapeView);

    // looping
    glutMainLoop();

    return 0;
}