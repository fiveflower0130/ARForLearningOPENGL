#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <cmath>

#include "OpenGLWindow.h"

//
// Camera configuration.
//
#ifdef _WIN32
char			*vconf = "WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

int             xsize, ysize;
int             thresh = 50;
int             count = 0;

char           *cparam_name    = "camera_cinema3.dat";
ARParam         cparam;

char           *patt_name      = "patt.len";
int             patt_id;
double          patt_width     = 80.0;
double          patt_center[2] = {0.0, 0.0};
double          patt_trans[3][4];

int mode = 0;
int contF = 0;

static void   init(void);
static void   cleanup(void);
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop(void);
static void   draw( void );

int winAR=0;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	CreateOpenGLWindow();

	init();

    arVideoCapStart();
    argMainLoop( NULL, keyEvent, mainLoop );
	return (0);
}

static void   keyEvent( unsigned char key, int x, int y)
{
    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        cleanup();
        exit(0);
    }

    if( key == 'c' ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        count = 0;

        mode = 1 - mode;
        if( mode ) printf("Continuous mode: Using arGetTransMatCont.\n");
         else      printf("One shot mode: Using arGetTransMat.\n");
    }
}

/* main loop */
static void mainLoop(void)
{
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             j, k;

	glutSetWindow(winAR);

    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }
    if( count == 0 ) arUtilTimerReset();
    count++;

    argDrawMode2D();
    argDispImage( dataPtr, 0,0 );

    /* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }

    arVideoCapNext();

	//printf("Marker_num:%d\n",marker_num);

    /* check for object visibility */
    k = -1;
    for( j = 0; j < marker_num; j++ ) {
        if( patt_id == marker_info[j].id ) {
            if( k == -1 ) k = j;
            else if( marker_info[k].cf < marker_info[j].cf ) k = j;
        }
    }
    if( k == -1 ) {
		contF = 0;
        argSwapBuffers();
        return;
    }

    /* get the transformation between the marker and the real camera */
    if( mode == 0 || contF == 0 ) {
        arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);
    }
    else {
        arGetTransMatCont(&marker_info[k], patt_trans, patt_center, patt_width, patt_trans);
    }
    contF = 1;

	//printf("%f %f %f\n",patt_trans[0][3],patt_trans[1][3],patt_trans[2][3]);

    draw();

    argSwapBuffers();
}

static void init( void )
{
    ARParam  wparam;
	
    /* open the video path */
    if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );

    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    if( (patt_id=arLoadPatt(patt_name)) < 0 ) {
        printf("pattern load error !!\n");
        exit(0);
    }

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 0, 0, 0 );

	winAR = glutGetWindow();
	printf("winAR = %d\n",winAR);
}

/* cleanup function called when program exits */
static void cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
	CloseOpenGLWindow();
}

void draw_axes(double size)
{
	int isEnableLighting = 0;

	if(glIsEnabled(GL_LIGHTING))
	{
		isEnableLighting = 1;
		glDisable(GL_LIGHTING);
	}

	glBegin(GL_LINES);
	glColor3d(1.0,1.0,1.0);
	glVertex3d(0.0,0.0,0.0);
	glVertex3d(size,0.0,0.0);
	glVertex3d(0.0,0.0,0.0);
	glVertex3d(0.0,size,0.0);
	glVertex3d(0.0,0.0,0.0);
	glVertex3d(0.0,0.0,size);
	glEnd();

	if(isEnableLighting == 1) glEnable(GL_LIGHTING);

	int isEnableColorMaterial = 1;

	if(!glIsEnabled(GL_COLOR_MATERIAL))
	{
		isEnableColorMaterial = 0;
		glEnable(GL_COLOR_MATERIAL);
	}

	glColor3d(1.0,0.0,0.0);
	glPushMatrix();
	glTranslated(size,0.0,0.0);
	glRotated(90.0,0.0,1.0,0.0);
	glutSolidCone(0.05*size, 0.15*size, 16, 32);
	glPopMatrix();

	glColor3d(0.0,1.0,0.0);
	glPushMatrix();
	glTranslated(0.0,size,0.0);
	glRotated(-90.0,1.0,0.0,0.0);
	glutSolidCone(0.05*size, 0.15*size, 16, 32);
	glPopMatrix();

	glColor3d(0.0,0.0,1.0);
	glPushMatrix();
	glTranslated(0.0,0.0,size);
	glutSolidCone(0.05*size, 0.15*size, 16, 32);
	glPopMatrix();

	if(isEnableColorMaterial == 0) glDisable(GL_COLOR_MATERIAL);
}

// Inverse modelview matrix: This assumes translation and rotation only (with LookAt, that's a safe assumption).
void invertModelViewMat(double m[16], double i[16]) 
{
	double t[3],o[3];

	t[0] = m[12]; t[1]=m[13]; t[2]= m[14];
	o[0] = m[0]*t[0] + m[1]*t[1] + m[2]*t[2];
	o[1] = m[4]*t[0] + m[5]*t[1] + m[6]*t[2];
	o[2] = m[8]*t[0] + m[9]*t[1] + m[10]*t[2];

	i[0] =m[0]; i[1] =m[4]; i[2] =m[8]; i[3] =0;
	i[4] =m[1]; i[5] =m[5]; i[6] =m[9]; i[7] =0;
	i[8] =m[2]; i[9] =m[6]; i[10]=m[10];i[11]=0;
	i[12]=-o[0]; i[13]=-o[1]; i[14]=-o[2]; i[15]=1;
}

void Cylinder(double radius, double height, int slices, int stacks)
{
	double dt = 360.0 / slices, dh = height/stacks;
	double x, z;

	for(double h=0; h<height; h+=dh)
	{
		glBegin(GL_QUAD_STRIP);
		for (double theta = -180.0; theta <= 180.0 + dt*0.5; theta += dt)
		{
			x = radius * sin(theta * 3.1415926 / 180.0);
			z = radius * cos(theta * 3.1415926 / 180.0);
			glNormal3d(x/radius, 0, z/radius);
			glVertex3d(x, h+dh, z);
			glVertex3d(x, h, z);
		}
		glEnd();
	}

	// cylinder bottom cap
	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0.0, -1.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);
	for (double theta = 360.0; theta >= 0-dt*0.5; theta -= dt)
	{
		x = radius * sin(theta * 3.1415926 / 180.0);
		z = radius * cos(theta * 3.1415926 / 180.0);
		glVertex3d(x, 0.0, z);
	}
	glEnd();

	// cylinder top cap
	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0.0, 1.0, 0.0);
	glVertex3d(0.0, height, 0.0);
	for (double theta = 0.0; theta <= 360.0+dt*0.5; theta += dt)
	{
		x = radius * sin(theta * 3.1415926 / 180.0);
		z = radius * cos(theta * 3.1415926 / 180.0);
		glVertex3d(x, height, z);
	}
	glEnd();
}

void DrawViewVolume(double *modelview_after_gluLookAt, double fovy, double aspect, double z_near, double z_far)
{
	double left, right, top, bottom;
	double vertex[9][3];
	double invMat[16];

	top = z_near * tan(0.5*fovy*3.1415926/180.0);
	bottom = -top;
	right = top*aspect;
	left = -right;

	vertex[0][0] = 0.0;   vertex[0][1] = 0.0;    vertex[0][2] = 0.0;
	vertex[1][0] = left;  vertex[1][1] = top;    vertex[1][2] = -z_near;
	vertex[2][0] = right; vertex[2][1] = top;    vertex[2][2] = -z_near;
	vertex[3][0] = right; vertex[3][1] = bottom; vertex[3][2] = -z_near;
	vertex[4][0] = left;  vertex[4][1] = bottom; vertex[4][2] = -z_near;

	top = top * z_far/z_near;
	bottom = bottom * z_far/z_near;
	left = left * z_far/z_near;
	right = right * z_far/z_near;

	vertex[5][0] = left;  vertex[5][1] = top;    vertex[5][2] = -z_far;
	vertex[6][0] = right; vertex[6][1] = top;    vertex[6][2] = -z_far;
	vertex[7][0] = right; vertex[7][1] = bottom; vertex[7][2] = -z_far;
	vertex[8][0] = left;  vertex[8][1] = bottom; vertex[8][2] = -z_far;

	invertModelViewMat(modelview_after_gluLookAt, invMat);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixd(invMat);


	int isEnableLighting = 0;

	if(glIsEnabled(GL_LIGHTING))
	{
		isEnableLighting = 1;
		glDisable(GL_LIGHTING);
	}

	glBegin(GL_LINE_LOOP);
		// front plane;
		glColor3d(1.0,1.0,0.0);
		glVertex3dv(vertex[1]);
		glVertex3dv(vertex[2]);
		glVertex3dv(vertex[3]);
		glVertex3dv(vertex[4]);
	glEnd();


	glBegin(GL_LINE_LOOP);
		// back plane;
		//glColor3d(0.0,1.0,0.0);
		glVertex3dv(vertex[5]);
		glVertex3dv(vertex[6]);
		glVertex3dv(vertex[7]);
		glVertex3dv(vertex[8]);
	glEnd();

	glBegin(GL_LINES);
		//glColor3d(0.0,0.0,1.0);
		// side planes
		glVertex3dv(vertex[0]);
		glVertex3dv(vertex[5]);
		glVertex3dv(vertex[0]);
		glVertex3dv(vertex[6]);
		glVertex3dv(vertex[0]);
		glVertex3dv(vertex[7]);
		glVertex3dv(vertex[0]);
		glVertex3dv(vertex[8]);
	glEnd();

	if(isEnableLighting == 1) glEnable(GL_LIGHTING);

	// draw virtual camera

	int isEnableColorMaterial = 1;
	int isEnableNormalize = 1;

	if(!glIsEnabled(GL_COLOR_MATERIAL))
	{
		isEnableColorMaterial = 0;
		glEnable(GL_COLOR_MATERIAL);
	}

	if(!glIsEnabled(GL_NORMALIZE))
	{
		isEnableNormalize = 0;
		glEnable(GL_NORMALIZE);
	}

	glColor3ub(50,205,50);
	glPushMatrix();
	glScaled(0.8,0.6,0.3);
	glTranslated(0.0,0.0,0.5*25);
	glutSolidCube(25);
	glPopMatrix();

	glPushMatrix();
	glRotated(-90.0,1.0,0.0,0.0);
	Cylinder(6.0,4.0,16,1);
	glPopMatrix();

	if(isEnableColorMaterial == 0) glDisable(GL_COLOR_MATERIAL);
	if(isEnableNormalize == 0) glDisable(GL_NORMALIZE);

	glPopMatrix();
}

static void draw( void )
{
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
    
    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(patt_trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);

    //glTranslatef( 0.0, 0.0, 25.0 );
	//glRotated(90.0, 1.0, 0.0, 0.0);
	//glutSolidTeapot(50.0);
    //glutSolidCube(50.0);

	double modelview_after_gluLookAt[16];

	glPushMatrix();
	glLoadIdentity();
	gluLookAt(0.0,0.0,100.0,0.0,0.0,0.0, 0.0, 1.0, 0.0);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_after_gluLookAt);
	glPopMatrix();

	glRotated(90.0,1.0,0.0,0.0);
	draw_axes(50.0);
	DrawViewVolume(modelview_after_gluLookAt,45.0, 800.0/600.0, 50.0, 2000.0);

    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}
