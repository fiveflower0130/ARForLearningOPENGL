#include <iostream>
#include <string>

#include <GL/glut.h>
#include "OpenGLAR.h"
#include "ParseQRCode.h"
#include "OpenGLDraw.h"
#include <vector>

using std::string;
using std::vector;

int winOpenGL;

double xRot=0.0,yRot=0.0;

int winWidth=800,winHeight=600;

extern int GridLine;
extern int showFlag;

extern vector<GLAR_Object> objectList;
extern vector<GLAR_LightSource> lightList;
extern Camera cam;

double modelview_after_gluLookAt[16];
double m_fovy=45.0,m_aspect=1.0,m_zNear=10.0,m_zFar=500.0;

void SetViewVolume(int width, int height)
{
	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	m_aspect = (double)width/(double)height;

	gluPerspective(m_fovy,m_aspect,m_zNear,m_zFar);
}

void GetModelviewMatrixAftergluLookAt(double *mat)
{
	for(int i=0; i<16; i++)
	{
		mat[i] = modelview_after_gluLookAt[i];
	}
}

void GetPerspectiveParameters(double &fovy, double &aspect, double &zNear, double &zFar)
{
	fovy = m_fovy;
	aspect = m_aspect;
	zNear = m_zNear;
	zFar = m_zFar;
}

void OpenGLInit(void)
{
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glEnable(GL_DEPTH_TEST);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void Display(void)
{
	glutSetWindow(winOpenGL);

	SetViewVolume(winWidth,winHeight);
	OpenGLInit();

	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,1.0,0.0);
	gluLookAt(cam.position[0],cam.position[1],cam.position[2],cam.look[0],cam.look[1],cam.look[2],cam.up[0],cam.up[1],cam.up[2]);

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_after_gluLookAt);

	//printf("Vector Size:%d\n",objectList.size());

	DrawOpenGLLight(lightList);

	if(showFlag==4) objectList.clear();
	
	DrawOpenGLObj(objectList,0,0);//¶×¥XOPENGL¹Ï§Î

	

	glutSwapBuffers();
	
}


void Reshape(int width, int height)
{
	winWidth = width;
	winHeight = height;
	SetViewVolume(winWidth,winHeight);
}

void SpecialKey(int key, int x, int y)
{
	
	switch(key)
	{
	case GLUT_KEY_UP:
		xRot+=3;
		break;
	case GLUT_KEY_DOWN:
		xRot-=3;
		break;
	case GLUT_KEY_LEFT:
		yRot+=3;
		break;
	case GLUT_KEY_RIGHT:
		yRot-=3;
		break;
				
	}
	glutPostRedisplay();
}


void CreateOpenGLWindow(void) 
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);

	winOpenGL = glutCreateWindow("OpenGL");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(SpecialKey);

	OpenGLInit();
}

void RefreshOpenGLWindow(void)
{
	int win = glutGetWindow();
	Display();
	
	glutSetWindow(win);
}