#include <iostream>
#include <GL/glut.h>
#include <cmath>

int winOpenGL;
double xRot=0.0,yRot=0.0;
int winWidth=800,winHeight=600;


void CalculateViewVolumeFromProjectionMatrix(double *proj, double &left, double &right, double &bottom, double &top, double &near, double &far)
{
	far = proj[14]/(proj[10]+1.0);
	near = -far*(1.0+proj[10])/(1.0-proj[10]);
	right = (proj[8]+1.0)*near/proj[0];
	left = (proj[8]-1.0)/(1.0+proj[8])*right;
	top = (proj[9]+1.0)*near/proj[5];
	bottom = (proj[9]-1.0)/(1.0+proj[9])*top;
}

void CalculateViewVolumeFromPerpestive(double fovy, double aspect, double near, double far, double &left, double &right, double &bottom, double &top)
{
	top = near * tan(0.5*fovy*3.1415926/180.0);
	bottom = -top;
	right = top*aspect;
	left = -right;
}

void SetViewVolume(int winWidth, int winHeight)
{
	glViewport(0,0,winWidth,winHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double fovy = 45.9;
	double aspect = (double)winWidth/(double)winHeight;
	double near = 1.0, far= 393.0;
	
	gluPerspective(fovy,aspect,near,far);

	double left,right,bottom,top;
	CalculateViewVolumeFromPerpestive(fovy, aspect,near,far,left,right,bottom,top);

	printf("left = %f right= %f bottom = %f top= %f near = %f far = %f\n",left, right, bottom, top, near, far);

	//glFrustum(-5.0,13.0,-3.0,11.0,0.1,101.5);
}

void MyInit(void)
{
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClearDepth(1.0f);
}

void Display(void)
{
	glutSetWindow(winOpenGL);

	MyInit();
	SetViewVolume(winWidth,winHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0,0.0,20.0,0.0,0.0,0.0,0.0,1.0,0.0);

	
	GLdouble modelview[16];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	
	printf("Modelview Matrix:\n");
	printf("%f %f %f %f\n",modelview[0],modelview[4],modelview[8],modelview[12]);
	printf("%f %f %f %f\n",modelview[1],modelview[5],modelview[9],modelview[13]);
	printf("%f %f %f %f\n",modelview[2],modelview[6],modelview[10],modelview[14]);
	printf("%f %f %f %f\n",modelview[3],modelview[7],modelview[11],modelview[15]);
	

	glRotated(xRot,1.0,0.0,0.0);
	glRotated(yRot,0.0,1.0,0.0);
	glutWireTeapot(4.0);

	glutSwapBuffers();

	GLdouble projection[16];

	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	double left,right,top,bottom,near,far;

	CalculateViewVolumeFromProjectionMatrix(projection,left,right,bottom,top,near,far);

	printf("left = %f right= %f bottom = %f top= %f near = %f far = %f\n",left, right, bottom, top, near, far);

	
	printf("Projection Matrix:\n");
	printf("%f %f %f %f\n",projection[0],projection[4],projection[8],projection[12]);
	printf("%f %f %f %f\n",projection[1],projection[5],projection[9],projection[13]);
	printf("%f %f %f %f\n",projection[2],projection[6],projection[10],projection[14]);
	printf("%f %f %f %f\n",projection[3],projection[7],projection[11],projection[15]);

}


void Reshape(int w, int h)
{
	//glutSetWindow(winOpenGL);
	winWidth = w;
	winHeight = h;
	SetViewVolume(winWidth,winHeight);
}

void SpecialKey(int key, int w, int h)
{
	//glutSetWindow(winOpenGL);
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

void RefreshOpenGLWindow(void)
{
	int win = glutGetWindow();
	Display();
	glutSetWindow(win);
}

void CreateOpenGLWindow(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(winWidth,winHeight);
	glutInitWindowPosition(100,100);
	winOpenGL = glutCreateWindow("OpenGL View");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(SpecialKey);

	MyInit();
}

void CloseOpenGLWindow(void)
{
	glutDestroyWindow(winOpenGL);
}