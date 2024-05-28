// Praktikum 7: Backtracking
// Penerapan contoh di slide kuliah Strategi Algoritma - Backtracking hal 39
#include <windows.h>
#include <iostream>
#include <list>
#include "draws.h"
using namespace std;

// global Graph
Graph graph;
// hasil penerapan algoritma
bool colorExist;
int* colorList;

// fungsi untuk menandai hasil
void drawResult()
{
    glPushMatrix();
    float radius = 15.0f;
    if (colorExist)
    {
        // gambar nodes
        for (int i=0; i<graph.getNumNodes(); i++)
        {
            sprintf(text,"%d",i);
            drawCircle(graph.getNodePosition()[i], getColorTable(colorList[i]-1),
                       radius, 360, 3.0f);
            drawText(graph.getNodePosition()[i], Vec3(1.0f,1.0f,1.0f), text,
                     radius, 2.0f);
        }
    }
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
    if (colorExist)
        drawResult();
    else
        drawNodes();
    drawEdges();
    // tampilkan obyek ke layar
    glutSwapBuffers();
}

// cek apakah warna sudah dipakai atau belum di node tetangga
// kalau belum return true dan sebaliknya
bool assignColor(int nodeIdx, vector<bool> adjStatus[], int colorList[],
                 int colorIdx)
{
    for (int i=0; i<adjStatus[nodeIdx].size(); i++)
    {
        vector<bool> localAdjStatus = adjStatus[nodeIdx];
        if (localAdjStatus.at(i) && colorIdx == colorList[i])
            return false;
    }
    return true;
}

// rekursive pewarnaan graf dengan jumlah warna = numColors;
bool graphColoringRecursive(vector<bool> adjStatus[], int numNodes,
                            int numColors, int colorList[], int nodeIdx)
{
    // base case: jika semua node sudah diwarnai return true
    if (nodeIdx == numNodes)
        return true;
    // coba warna yang lain
    for (int colorIdx = 1; colorIdx <= numColors; colorIdx++)
    {
        // cek apakah warna bisa diterapkan pada node
        if (assignColor(nodeIdx, adjStatus, colorList, colorIdx))
        {
            colorList[nodeIdx] = colorIdx;
            // rekursi untuk mewarnai semua node
            if (graphColoringRecursive(adjStatus, numNodes, numColors,
                                       colorList, nodeIdx+1))
                return true;
            // jika warna tidak mungkin diterapkan maka dihapus
            colorList[nodeIdx] = 0;
        }
    }
    // jika tidak ada warna yang bisa diterapkan maka return false
    return false;
}

// fungsi untuk mewarnai node pada graf dengan jumlah warna = numColors
void graphColoring(vector<bool> adjStatus[], int numColors)
{
    // inisialisasi awal semua warna di node = 0
    colorList = new int[graph.getNumNodes()];
    for (int i = 0; i < graph.getNumNodes(); i++)
        colorList[i] = 0;
    // rekursi pewarnaan graf
    if (graphColoringRecursive(adjStatus, graph.getNumNodes(), numColors,
                               colorList, 0) == false)
    {
        printf("Tidak ada solusi pewarnaan yang mungkin");
        colorExist = false;
    }
    else
    {
        printf("Solusi pewarnaannya yaitu: \n");
        for (int i = 0; i < graph.getNumNodes(); i++)
            printf(" %d ", colorList[i]);
        printf("\n");
        colorExist = true;
    }
}

// kode tester
int main(int argc, char** argv)
{
    // inisialisasi graf
    graph.setIsDirected(false);
    graph.setNumLevels(3);
    graph.setNumNodes(5);
    // tambahkan node
    graph.addNode(3, 0, 1.0f);
    graph.addNode(4, 0, 1.0f);
    graph.addNode(0, 1, 1.0f);
    graph.addNode(1, 1, 1.0f);
    graph.addNode(2, 2, 1.0f);
    // tambahkan edge
	graph.addEdge(0,1);
	graph.addEdge(0,2);
	graph.addEdge(0,3);
	graph.addEdge(1,4);
	graph.addEdge(1,2);
	graph.addEdge(3,4);
	

    // estimate node position
    graph.setNodePosition();
    graph.setAdjStatus();
    // banyaknya warna
    int numColors = 3;
    graphColoring(graph.getAdjStatus(), numColors);
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
