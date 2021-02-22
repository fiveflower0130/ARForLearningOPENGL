#include <iostream>//第一個要先寫這個
#include <vector>
#include <cmath>

#include <GL/glut.h>
#include "OpenGLAR.h"
#include "ParseQRCode.h"
#include "OpenGLWindow.h"


using std::string;
using std::vector;
using std::cout;//使用 std 目錄下的 cout
using std::endl;//

extern int selectedObj;
extern int showFlag ;
extern int GridLine;



void draw_axes(double size)//座標線
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

	

	int isEnableColorMaterial = 1;

	if(!glIsEnabled(GL_COLOR_MATERIAL))
	{
		isEnableColorMaterial = 0;
		glEnable(GL_COLOR_MATERIAL);
	}

	glEnable(GL_LIGHTING);

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

	if(isEnableLighting == 1) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);

	if(isEnableColorMaterial == 0) glDisable(GL_COLOR_MATERIAL);
	else glEnable(GL_COLOR_MATERIAL);
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
	bottom = -top;
	left = left * z_far/z_near;
	right = -left;

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

	glEnable(GL_LIGHTING);

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

	if(isEnableLighting == 1) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);


	if(isEnableColorMaterial == 0) glDisable(GL_COLOR_MATERIAL);
	else glEnable(GL_COLOR_MATERIAL);
	if(isEnableNormalize == 0) glDisable(GL_NORMALIZE);
	else glEnable(GL_NORMALIZE);

	glPopMatrix();
}



void DrawOpenGLLight(vector<GLAR_LightSource> &lightList)
{
	for(vector<GLAR_LightSource>::size_type i=0; i!=lightList.size(); ++i)
	{
		if(lightList[i].isEnable)
		{
			
			//float ambient[4]={0.1f,0.1f,0.1f,1.0f};
			//float specular[4]={1.0f,1.0f,1.0f,1.0f};
			//float diffuse[4];
			float ambient[4]={lightList[i].ambient[0],lightList[i].ambient[1],lightList[i].ambient[2],lightList[i].ambient[3]};
			float diffuse[4]={lightList[i].diffuse[0],lightList[i].diffuse[1],lightList[i].diffuse[2],lightList[i].diffuse[3]};
            float specular[4]={lightList[i].specular[0],lightList[i].specular[1],lightList[i].specular[2],lightList[i].specular[3]};

			float position[4]={lightList[i].position[0],lightList[i].position[1],lightList[i].position[2],lightList[i].position[3]};
			float direction[3]={lightList[i].direction[0],lightList[i].direction[1],lightList[i].direction[2]};
			float exponent = lightList[i].exponent;
			float cut_off = lightList[i].cut_off; 
			//printf("%f %f %f\n",lightList[i].R,lightList[i].G,lightList[i].B);

			glEnable(GL_LIGHT0+i);
		    glLightfv(GL_LIGHT0+i, GL_AMBIENT, lightList[i].ambient);
			glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lightList[i].diffuse);
			glLightfv(GL_LIGHT0+i, GL_SPECULAR, lightList[i].specular);

			glLightfv(GL_LIGHT0+i, GL_POSITION, lightList[i].position);
			glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, lightList[i].direction);
			glLightf(GL_LIGHT0+i, GL_SPOT_EXPONENT, lightList[i].exponent);
			glLightf(GL_LIGHT0+i, GL_SPOT_CUTOFF, lightList[i].cut_off);

		}
	}
}
/*
void DrawOpenGLLight(vector<GLAR_LightSource> &lightList)
{
	for(vector<GLAR_LightSource>::size_type i=0; i!=lightList.size(); ++i)
	{
		if(lightList[i].isEnable)
		{
			float ambient[4]={0.1f,0.1f,0.1f,1.0f};
			float specular[4]={1.0f,1.0f,1.0f,1.0f};
			float diffuse[4];
			switch(lightList[i].type)
			{

			
			case LIGHTTYPE_POINTLIGHT:
				glPushMatrix();


				diffuse[0] = lightList[i].R;
				diffuse[1] = lightList[i].G;
				diffuse[2] = lightList[i].B;
				diffuse[3] = lightList[i].A;

				printf("%f %f %f\n",lightList[i].R,lightList[i].G,lightList[i].B);

				glEnable(GL_LIGHT0+i);
				glLightfv(GL_LIGHT0+i, GL_AMBIENT, ambient);
				glLightfv(GL_LIGHT0+i, GL_DIFFUSE, diffuse);
				glLightfv(GL_LIGHT0+i, GL_DIFFUSE, specular);

				glPopMatrix();
				break;

			case LIGHTTYPE_PARALLELLIGHT:
				glPushMatrix();


				diffuse[0] = lightList[i].R;
				diffuse[1] = lightList[i].G;
				diffuse[2] = lightList[i].B;
				diffuse[3] = lightList[i].A;

				printf("%f %f %f\n",lightList[i].R,lightList[i].G,lightList[i].B);

				glEnable(GL_LIGHT0+i);
				glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, specular);

				glPopMatrix();
				break;

			case LIGHTTYPE_SPOTLIGHT:
				glPushMatrix();

				
				diffuse[0] = lightList[i].R;
				diffuse[1] = lightList[i].G;
				diffuse[2] = lightList[i].B;
				diffuse[3] = lightList[i].A;

				printf("%f %f %f\n",lightList[i].R,lightList[i].G,lightList[i].B);

				glEnable(GL_LIGHT0+i);
				glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, specular);

				glPopMatrix();
				break;
				
			}


		}
	}
}
*/

void DrawOpenGLObj(vector<GLAR_Object> &objectList, int hasBoundingBox,int hasGridLine)
{
	
	int isEnableColorMaterial = 1;

	if(!glIsEnabled(GL_COLOR_MATERIAL))
	{
		isEnableColorMaterial = 0;
		glEnable(GL_COLOR_MATERIAL);
	}

	glColor3d(0.0,0.5,0.5);
	for(vector<GLAR_Object>::size_type i=0; i!=objectList.size(); ++i)
	{
		switch(objectList[i].objectID)
		{
		case SPHERE:
			glPushMatrix();
			
			for(int j=objectList[i].actionList.size()-1; j>=0; --j)
			{
				
				switch(objectList[i].actionList[j].actionID)
				{
				case ACTION_TRANSLATE:
					
					glTranslated(objectList[i].actionList[j].x,objectList[i].actionList[j].y,objectList[i].actionList[j].z);
					
					if(showFlag==5) cout << "glTranslated(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_ROTATE:
					glRotated(objectList[i].actionList[j].angle,objectList[i].actionList[j].x,objectList[i].actionList[j].y,objectList[i].actionList[j].z);
					if(showFlag==1) cout << "glRotated(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_SCALE:
					glScaled(objectList[i].actionList[j].x,objectList[i].actionList[j].y,objectList[i].actionList[j].z);
					if(showFlag==1) cout << "glScaled(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
				    break;
				}
			}
			if(objectList[i].isWire)
			{
				glutWireSphere(objectList[i].size,32,64);
				if(showFlag==3) cout << "glutWireSphere(" << objectList[i].size << "," <<32 << "," <<64 << ");" << endl;
			}
			else 
			{
				glutSolidSphere(objectList[i].size,32,64);	
				if(showFlag==3) cout << "glutSolidSphere(" << objectList[i].size << "," <<32 << "," <<64 << ");" << endl;
			}
			glPopMatrix();
			break;

		case CUBE:
			glPushMatrix();
			for(int j=objectList[i].actionList.size()-1; j>=0; --j)
			{
				switch(objectList[i].actionList[j].actionID)
				{
				case ACTION_TRANSLATE:
					glTranslated(objectList[i].actionList[j].x,objectList[i].actionList[j].y,objectList[i].actionList[j].z);
					if(showFlag==1) cout << "glTranslated(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_ROTATE:
					glRotated(objectList[i].actionList[j].angle,objectList[i].actionList[j].x,objectList[i].actionList[j].y,objectList[i].actionList[j].z);
					if(showFlag==1) cout << "glRotated(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_SCALE:
					glScaled(objectList[i].actionList[j].x,objectList[i].actionList[j].y,objectList[i].actionList[j].z);
					if(showFlag==1) cout << "glScaled(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				}
			}
			if(objectList[i].isWire)
			{
				glutWireCube(objectList[i].size);
				if(showFlag==3) cout << "glutWireCube(" << objectList[i].size << ");" << endl;
			}
			else 
			{
				glutSolidCube(objectList[i].size);
				if(showFlag==3) cout << "glutSolidCube(" << objectList[i].size << ");" << endl;
			}
			glPopMatrix();
			
			break;

		case TEAPOT:
			glPushMatrix();

			for(int j=objectList[i].actionList.size()-1; j>=0; --j)
			{
				switch(objectList[i].actionList[j].actionID)
				{
				case ACTION_TRANSLATE:
					glTranslated(objectList[i].actionList[j].x,objectList[i].actionList[j].y,objectList[i].actionList[j].z);
					if(showFlag==1) cout << "glTranslated(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_ROTATE:
					glRotated(objectList[i].actionList[j].angle,objectList[i].actionList[j].x,objectList[i].actionList[j].y,objectList[i].actionList[j].z);
					if(showFlag==1) cout << "glRotated(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;
				case ACTION_SCALE:
					glScaled(objectList[i].actionList[j].x,objectList[i].actionList[j].y,objectList[i].actionList[j].z);
					if(showFlag==1) cout << "glScaled(" << objectList[i].actionList[j].x << "," <<objectList[i].actionList[j].y << "," <<objectList[i].actionList[j].z << ");" << endl;
					break;

				}
			}
			if(objectList[i].isWire)
			{
				glutWireTeapot(objectList[i].size);
				if(showFlag==3) cout << "glutWireTeapot(" << objectList[i].size << ");" << endl;
			}
			else 
			{
				glutSolidTeapot(objectList[i].size);
				if(showFlag==3) cout << "glutSolidTeapot(" << objectList[i].size << ");" << endl;
			}
			glPopMatrix();
			
			break;
		}

	}

	//選取
	if(selectedObj >= 0 && selectedObj < objectList.size() && hasBoundingBox == 1) 
	{
		int isEnableLighting = 0;

		if(glIsEnabled(GL_LIGHTING))
		{
			isEnableLighting = 1;
			glDisable(GL_LIGHTING);
		}
		glPushMatrix();

			glColor3d(1.0,0.0,0.0);
			for(int i=objectList[selectedObj].actionList.size()-1; i>=0; --i)
			{
				switch(objectList[selectedObj].actionList[i].actionID)
				{
					case ACTION_TRANSLATE:
					glTranslated(objectList[selectedObj].actionList[i].x,objectList[selectedObj].actionList[i].y,objectList[selectedObj].actionList[i].z);
					break;
					case ACTION_ROTATE:
					glRotated(objectList[selectedObj].actionList[i].angle,objectList[selectedObj].actionList[i].x,objectList[selectedObj].actionList[i].y,objectList[selectedObj].actionList[i].z);
					break;
					case ACTION_SCALE:
					glScaled(objectList[selectedObj].actionList[i].x,objectList[selectedObj].actionList[i].y,objectList[selectedObj].actionList[i].z);
					break;
				}
			}
			glutWireCube(objectList[selectedObj].size*2);
		glPopMatrix();

		if(isEnableLighting == 1) glEnable(GL_LIGHTING);
	}

	if(isEnableColorMaterial == 0) glDisable(GL_COLOR_MATERIAL);
	else glEnable(GL_COLOR_MATERIAL);


	//座標縣
	if(GridLine == 1 && hasGridLine == 1)
	{
		draw_axes(200.0);
	}

	double modelview[16];
	double fovy,aspect,zNear,zFar;

	GetModelviewMatrixAftergluLookAt(modelview);
	GetPerspectiveParameters(fovy,aspect,zNear,zFar);

	DrawViewVolume(modelview,fovy,aspect,zNear,zFar);
}
