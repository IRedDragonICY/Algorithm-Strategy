// Praktikum 10: String Matching
// Penerapan contoh di slide kuliah materi string matching dengan KMP
#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include "draws.h"

using namespace std;

// global Graph
Graph graph;
char markText[1024];
string ptext;
string pattern;
int startIdx;

// fungsi untuk menandai jalur hasil pencarian
void drawResult()
{
    glPushMatrix();
    sprintf(markText, "%s", ptext.c_str());
    Vec3 position(-350.0f, 0.0f, 0.0f);
    Vec3 color1(1.0f, 1.0f, 1.0f);
    Vec3 color2(1.0f, 0.0f, 1.0f);
    if (startIdx >= 0)
        drawText(position, color1, color2, markText, 2.0f, startIdx, pattern.length());
    else
        drawText(position, color1, color1, markText, 2.0f, 0, pattern.length());
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
    drawResult();

    // tampilkan obyek ke layar
    glutSwapBuffers();
}

// cari kecocokan dalam pola itu sendiri
int* computeFail(string pattern)
{
    int* fail = new int[pattern.length()];
    fail[0] = 0;
    int m = pattern.length();
    int j = 0;
    int i = 1;
    while (i < m)
    {
        cout << "computeFail: Membandingkan pattern[" << j << "] = " << pattern.at(j)
             << " dengan pattern[" << i << "] = " << pattern.at(i) << endl;
        if (pattern.at(j) == pattern.at(i))
        {
            // karakter j+1 cocok
            fail[i] = j + 1;
            cout << "computeFail: Cocok, fail[" << i << "] = " << (j + 1) << endl;
            i++;
            j++;
        }
        else if (j > 0) // prefiks j
        {
            j = fail[j-1];
            cout << "computeFail: Tidak cocok, kembali ke fail[" << j << "] = " << fail[j] << endl;
        }
        else
        {
            // tidak cocok
            fail[i] = 0;
            cout << "computeFail: Tidak cocok, fail[" << i << "] = 0" << endl;
            i++;
        }
    }
    return fail;
}

// pencocokan string dengan KMP
int KMPMatch(string text, string pattern)
{
    int n = text.length();
    int m = pattern.length();
    int* fail = computeFail(pattern);
    int i = 0;
    int j = 0;
    while (i < n)
    {
        cout << "KMPMatch: Membandingkan text[" << i << "] = " << text.at(i)
             << " dengan pattern[" << j << "] = " << pattern.at(j) << endl;
        if (pattern.at(j) == text.at(i))
        {
            if (j == m - 1)
            {
                cout << "KMPMatch: Pola ditemukan pada indeks " << (i - m + 1) << endl;
                return i - m + 1; // cocok
            }
            cout << "KMPMatch: Cocok, lanjut ke text[" << (i+1) << "] dan pattern[" << (j+1) << "]" << endl;
            i++;
            j++;
        }
        else if (j > 0)
        {
            j = fail[j-1];
            cout << "KMPMatch: Tidak cocok, kembali ke fail[" << j << "] = " << fail[j] << endl;
        }
        else
        {
            cout << "KMPMatch: Tidak cocok, lanjut ke text[" << (i+1) << "]" << endl;
            i++;
        }
    }
    cout << "KMPMatch: Pola tidak ditemukan\n";
    return -1; // tidak cocok
}

// kode tester
int main(int argc, char** argv)
{
    // teks yang akan dicocokkan
    ptext = "abacaabacabacababa";
    // pola yang akan dicocokkan
    pattern = "acabaca";
    cout << "Cari: [" << pattern << "] di dalam teks: [" << ptext << "]\n";

    // penerapan algoritma KMP
    startIdx = KMPMatch(ptext, pattern);
    if (startIdx == -1)
        cout << "Pola tidak ditemukan pada teks\n";
    else
        cout << "Pola ditemukan pada posisi-" << startIdx << "\n";

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
