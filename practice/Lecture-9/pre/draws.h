// Header untuk visualisasi graf menggunakan 
// library OpenGL, bisa untuk menggambar 
// nodes/vertices/points dan edges/links/lines dari graf
//
// Praktikum Strategi Algoritma, Teknik Informatika, UAD
// Adhi Prahara. 2020.

#include <iostream>
#include <list> 
#include <ctime> 
#include <math.h>
#include <GL/glut.h>
#include "graph.h"

using namespace std;

#define PHI 3.141592654	
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

char text[2];

// warna 0-18
Vec3 getColorTable(int colorIdx) 
{
	Vec3* colorTable = new Vec3[18];
	colorTable[0] = Vec3(1.0f,0.0f,0.0f); // merah
	colorTable[1] = Vec3(0.0f,1.0f,0.0f); // hijau
	colorTable[2] = Vec3(0.0f,0.0f,1.0f); // biru
	colorTable[3] = Vec3(1.0f,1.0f,0.0f); // kuning
	colorTable[4] = Vec3(0.0f,1.0f,1.0f); // cyan
	colorTable[5] = Vec3(1.0f,0.0f,1.0f); // magenta
	colorTable[6] = Vec3(1.0f,0.5f,0.0f); // 
	colorTable[7] = Vec3(0.5f,1.0f,1.0f); // 
	colorTable[8] = Vec3(1.0f,0.5f,1.0f); //  
	colorTable[9] = Vec3(1.0f,1.0f,0.5f); // 
	colorTable[10] = Vec3(1.0f,0.0f,0.5f); // 
	colorTable[11] = Vec3(1.0f,0.5f,0.5f); // 
	colorTable[12] = Vec3(0.5f,1.0f,0.0f); // 
	colorTable[13] = Vec3(0.0f,1.0f,0.5f); // 
	colorTable[14] = Vec3(0.5f,1.0f,0.5f); // 
	colorTable[15] = Vec3(0.5f,0.0f,1.0f); // 
	colorTable[16] = Vec3(0.0f,0.5f,1.0f); // 
	colorTable[17] = Vec3(0.5f,0.5f,1.0f); // 
	
	return colorTable[colorIdx];		
}

// fungsi untuk menggambar teks
void drawText(
	Vec3 position, 
	Vec3 color, 
	char* text, 
	float radius, 
	float weight) 
{
	glPushMatrix();
	
	glTranslatef(position.getX()-radius/2.0f, position.getY()-radius/2.0f, position.getZ());
	glScalef(0.15f, 0.15f, 0.15f);
	glLineWidth(weight);
	int textLength = (int)strlen(text);
	for (int i = 0; i < textLength; i++)
	{
		glColor3f(color.getX(), color.getY(), color.getZ());
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}
	
	glPopMatrix();
}

// fungsi untuk menggambar teks
void drawText(
	Vec3 position, 
	Vec3 color1,
	Vec3 color2, 
	char* text, 
	float weight,
	int startIdx,
	int charLength) 
{
	glPushMatrix();
	
	glTranslatef(position.getX(), position.getY(), position.getZ());
	glScalef(0.25f, 0.25f, 0.25f);
	glLineWidth(weight);
	int textLength = (int)strlen(text);
	for (int i = 0; i < textLength; i++)
	{
		if (i >= startIdx && i < startIdx + charLength)
			glColor3f(color2.getX(), color2.getY(), color2.getZ());
		else			
			glColor3f(color1.getX(), color1.getY(), color1.getZ());
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}
	
	glPopMatrix();
}

// fungsi untuk menggambar panah
void drawArrow(
	Vec3 tail, 
	Vec3 head, 
	Vec3 color, 
	float length, 
	float weight,
	bool isDirected) 
{
	if (!isDirected)
		return;
		
	glPushMatrix();
	
	float theta = atan2(tail.getY() - head.getY(), tail.getX() - head.getX());
	Vec3 arrowTip1 = Vec3(
		head.getX() + length * cos(theta + PHI / 4),
        head.getY() + length * sin(theta + PHI / 4),
		0.0f);
	Vec3 arrowTip2 = Vec3(
		head.getX() + length * cos(theta - PHI / 4),
        head.getY() + length * sin(theta - PHI / 4),
		0.0f);
	glLineWidth(weight);
	glBegin(GL_LINES);
    glColor3f(color.getX(), color.getY(), color.getZ());
    glVertex3f(arrowTip1.getX(), arrowTip1.getY(), arrowTip1.getZ());
    glVertex3f(head.getX(), head.getY(), head.getZ());
    glVertex3f(arrowTip2.getX(), arrowTip2.getY(), arrowTip2.getZ());
    glVertex3f(head.getX(), head.getY(), head.getZ());
    glEnd();
    
    glPopMatrix();
}

// fungsi untuk menggambar lingkaran node
void drawCircle(
	Vec3 position, 
	Vec3 color, 
	float radius, 
	int numSegments, 
	float weight) 
{
	glPushMatrix();
	
	glLineWidth(weight);
    glBegin(GL_LINE_LOOP);//(GL_TRIANGLE_FAN);
    glColor3f(color.getX(), color.getY(), color.getZ());
    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * PHI * float(i) / float(numSegments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex3f(x + position.getX(), y + position.getY(), position.getZ());
    }
    glEnd();
    
    glPopMatrix();
}

// fungsi untuk menggambar seperempat lingkaran node
void drawQuarterCircle(
	Vec3 position, 
	Vec3 color, 
	float radius, 
	int numSegments, 
	float weight, 
	bool isDirected) 
{
	glPushMatrix();
	
	Vec3 tail = position;
	Vec3 head = position;
	bool endLine = false;
	glLineWidth(weight);
    glBegin(GL_LINES);
    glColor3f(color.getX(), color.getY(), color.getZ());
    for (int i = 1; i < numSegments; i++) {
        float theta1 = 2.0f * PHI * float(i-1) / float(numSegments);
		float theta2 = 2.0f * PHI * float(i) / float(numSegments);
        if (theta1 > 1.25f * PHI && theta1 < 1.75f * PHI) {
        	float x1 = 2.0f * radius * cosf(theta1);
	        float y1 = 2.0f * radius * sinf(theta1);
        	glVertex3f(x1 + position.getX(), y1 + position.getY(), position.getZ());
	        float x2 = 2.0f * radius * cosf(theta2);
	        float y2 = 2.0f * radius * sinf(theta2);
	        glVertex3f(x2 + position.getX(), y2 + position.getY(), position.getZ());
	        if (2.0f * PHI * float(i+1) / float(numSegments) >= 1.75f) {
	        	tail = Vec3(x1 + position.getX(), y1 + position.getY(), position.getZ());
	        	head = Vec3(x2 + position.getX(), y2 + position.getY(), position.getZ());
	        	endLine = true;
	        }
   	 	}
    }
    glEnd();
    if (endLine) {
    	drawArrow(tail, head, color, 10, weight, isDirected);
	}
	
    glPopMatrix();
}

// cek sudut 
int checkAngle(float theta) 
{
	float fraction = PHI/6.0f;
	// -1/6 phi < theta <= 1/6 phi
	if ((theta > -fraction && theta < 0) || (theta >= 0 && theta <= fraction)) { return 1; }
	// 1/6 phi < theta <= phi/2 - 1/6 phi
	else if (theta > fraction && theta <= PHI/2.0f - fraction) { return 2; }
	// phi/2 - 1/6 phi < theta <= phi/2 + 1/6 phi
	else if (theta > PHI/2.0f - fraction && theta <= PHI/2.0f + fraction) { return 3; }
	// phi/2 + 1/6 phi < theta < phi - 1/6 phi
	else if (theta > PHI/2.0f + fraction && theta <= PHI - fraction) { return 4; }
	// phi - 1/6 phi < theta <= phi
	else if ((theta > PHI - fraction && theta <= PHI + fraction) || (theta >= -PHI - fraction && theta <= -PHI + fraction)) { return 5; }
	// -phi + 1/6 phi < theta <= -phi/2 - 1/6 phi
	else if (theta > -PHI + fraction && theta <= -PHI/2.0f - fraction) { return 6; }
	// -phi - 1/6 phi < theta <= -phi/2 + 1/6 phi
	else if (theta > -PHI/2.0f - fraction && theta <= -PHI/2.0f + fraction) { return 7; }
	// -phi/2 + 1/6 phi < theta <= -1/6 phi
	else if (theta > -PHI/2.0f + fraction && theta <= -fraction) { return 8; }
}

// fungsi untuk menggambar garis antar node
void drawLine(
		Vec3* nodePosition, 
		int sourceIdx, 
		int targetIdx,
		Vec3 color, 
		float radius,
		float weight,
		bool isDirected) 
{		
	glPushMatrix();
	
	bool sameNode = false;
	float sourceOffsetX = 0.0f;
	float sourceOffsetY = 0.0f;
	float targetOffsetX = 0.0f;
	float targetOffsetY = 0.0f;
	Vec3 sourceNode = nodePosition[sourceIdx];
	Vec3 targetNode = nodePosition[targetIdx];
	if (sourceIdx == targetIdx)
		sameNode = true;
	else 
	{
		float theta = atan2(
					targetNode.getY() - sourceNode.getY(), 
					targetNode.getX() - sourceNode.getX());
		switch(checkAngle(theta)) 
		{
			case 1:
				sourceOffsetX = radius;	
				targetOffsetX = -radius;
				if (isDirected) 	
	    			targetOffsetX = -1.5f*radius;
				break;
			case 2:
				sourceOffsetX = radius;	
				sourceOffsetY = radius;
				targetOffsetX = -radius;
				targetOffsetY = -radius;
				if (isDirected) 
					targetOffsetY = -1.5f*radius;
				break;
			case 3:
				sourceOffsetY = radius;
				targetOffsetY = -radius;
				if (isDirected) 
					targetOffsetY = -1.5f*radius;
				break;
			case 4:
				sourceOffsetX = -radius;	
				sourceOffsetY = radius;
				targetOffsetX = radius;
				targetOffsetY = -radius;
				if (isDirected) 
					targetOffsetY = -1.5f*radius;
				break;
			case 5:
				sourceOffsetX = -radius;	
				targetOffsetX = radius;
				if (isDirected) 
	    			targetOffsetX = 1.5f*radius;
				break;
			case 6:
				sourceOffsetX = -radius;	
				sourceOffsetY = -radius;
				targetOffsetX = radius;
				targetOffsetY = radius;
				if (isDirected) 
					targetOffsetY = 1.5f*radius;
				break;
			case 7:
				sourceOffsetY = -radius;
				targetOffsetY = radius;
				if (isDirected) 
					targetOffsetY = 1.5f*radius;
				break;
			case 8:
				sourceOffsetX = radius;	
				sourceOffsetY = -radius;
				targetOffsetX = -radius;
				targetOffsetY = radius;
				if (isDirected) 
					targetOffsetY = 1.5f*radius;
				break;
		}
		sourceNode = Vec3(sourceNode.getX()+sourceOffsetX, sourceNode.getY()+sourceOffsetY, sourceNode.getZ());
		targetNode = Vec3(targetNode.getX()+targetOffsetX, targetNode.getY()+targetOffsetY, targetNode.getZ());
				
		//cout << "Number " << checkAngle(theta) << ": " << sourceIdx << " to " << targetIdx << ": " << theta << "\n" << flush;
	}
	if (sameNode) 
		// gambar lingkaran 	
		drawQuarterCircle(sourceNode, color, radius, 360, weight, isDirected);
	else
	{
    	// gambar garis
		glLineWidth(weight);
		glBegin(GL_LINES);
    	glColor3f(color.getX(), color.getY(), color.getZ());
    	glVertex3f(sourceNode.getX(), sourceNode.getY(), sourceNode.getZ());
		glVertex3f(targetNode.getX(), targetNode.getY(), targetNode.getZ());
    	glEnd();
    	
    	// gambar panah
		drawArrow(sourceNode, targetNode, color, 10, weight, isDirected);
	}
	
    glPopMatrix();
}

// fungsi untuk menggambar node
void drawNodes()
{
	glPushMatrix();
	
	float radius = 15.0f;

	// gambar nodes
	for (int i=0; i<graph.getNumNodes(); i++) 
	{		
		sprintf(text,"%d",i); 
		drawCircle(graph.getNodePosition()[i], Vec3(0.0f,1.0f,0.0f), radius, 360, 3.0f);
		drawText(graph.getNodePosition()[i], Vec3(1.0f,1.0f,1.0f), text, radius, 2.0f);
	}

	glPopMatrix();
}

// fungsi untuk menggambar edge
void drawEdges()
{
	glPushMatrix();
	
	float radius = 15.0f;

	// gambar edges
	Vec3 sourceNode, targetNode;
	for (int i=0; i<graph.getNumNodes(); i++) {
		sourceNode = graph.getNodePosition()[i];
		vector<int> localNodes = graph.getAdjNodes()[i];
    	for (int j=0; j<localNodes.size(); j++) 
		{
    		int nodeIdx = localNodes.at(j);
    		targetNode = graph.getNodePosition()[nodeIdx];
    		drawLine(
				graph.getNodePosition(), 
				i, 
				nodeIdx,
				Vec3(1.0f,1.0f,1.0f), 
				radius, 2.0f, graph.getIsDirected());
    	}
	}
	
	glPopMatrix();
}

// fungsi untuk menggambar jarak di edge
void drawDistances()
{
	glPushMatrix();
	
	float radius = 0.0f;
	vector<bool>* adjStatus = new vector<bool>[graph.getNumNodes()];
	for (int i=0; i<graph.getNumNodes(); i++)
	{
		for (int j=0; j<graph.getAdjStatus()[i].size(); j++)
			adjStatus[i].push_back(graph.getAdjStatus()[i].at(j));	
	}

	// gambar edges
	Vec3 sourceNode, targetNode;
	for (int i=0; i<graph.getNumNodes(); i++) 
	{
		sourceNode = graph.getNodePosition()[i];
		vector<int> localNodes = graph.getAdjNodes()[i];
    	for (int j=0; j<localNodes.size(); j++) 
		{
			int nodeIdx = localNodes.at(j);
			if (adjStatus[i].at(nodeIdx))
			{
				float fraction = 1.0f/18.0f * PHI;
				targetNode = graph.getNodePosition()[nodeIdx];
	    		float theta = atan2(
					targetNode.getY() - sourceNode.getY(), 
					targetNode.getX() - sourceNode.getX());
				float distance = sqrt(
					powf(sourceNode.getX() - targetNode.getX(),2) +
					powf(sourceNode.getY() - targetNode.getY(),2));
	    		Vec3 position(
					sourceNode.getX() + distance/2.0f * cosf(theta + fraction),
					sourceNode.getY() + distance/2.0f * sinf(theta + fraction),
					0.0f);
				sprintf(text,"%.0f",graph.getNodeDistance()[i].at(nodeIdx)); 
				drawText(position, Vec3(1.0f,1.0f,1.0f), text, radius, 2.0f);
				//
				adjStatus[i].at(nodeIdx) = false;
				adjStatus[nodeIdx].at(i) = false;
			}
    	}
	}
	
	glPopMatrix();
}

// inisialisasikan variabel
void initView(void)
{
	// inisialisasi warna latar belakang layar
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);	
	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();
	// set proyeksi ke perspektif
	//gluPerspective(45.0, 1.0, 1.0, 100.0);
	// set proyeksi ke orthogonal
	glOrtho((GLfloat)-SCREEN_WIDTH / 2, (GLfloat)SCREEN_WIDTH / 2,
		(GLfloat)-SCREEN_HEIGHT / 2, (GLfloat)SCREEN_HEIGHT / 2, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);				
	glLoadIdentity();						
	
	// inisialisasi kamera pandang
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// fungsi ini digunakan bila layar akan diresize (default)
void reshapeView(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// set proyeksi ke perspektif
	//gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	// set proyeksi ke orthogonal
	glOrtho((GLfloat)-w / 2, (GLfloat)w / 2, (GLfloat)-h / 2, (GLfloat)h / 2, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}



